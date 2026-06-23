#include "minirede.h"
#include <cstring>
void inicializarMiniRede(MiniRede& rede) {
    rede.raiz_id = nullptr;
    rede.todos_os_posts = nullptr;
    for (int i = 0; i < TAM_HASH; i++)
    {
        rede.tabela_usernames[i] = nullptr;
    }
    
}

void liberarMiniRede(MiniRede& rede) {
    liberarpostGLobal(rede); //Antes de deletar os posts do usuario, deletamos os ponteiros de usuarios que curtiram e comentara naqueles posts
    liberarArvoredeUsuarios(rede.raiz_id); //Faz o papel de chamar as funçoes para deletar ponteiros da struct do usuario e o usuario em si.
    rede.raiz_id = nullptr;
    liberarTabelaHash(rede.tabela_usernames);
}

void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida) {
    std::string mnemonico;
    bool primeira_leitura = true;

    while(entrada >> mnemonico)
    {
        //Limpa o BOM se ele aparecer na primeira palavra do arquivo
        if (primeira_leitura) {
            primeira_leitura = false;
            
            // Verifica se os 3 primeiros bytes são o sinal do BOM UTF-8
            if (mnemonico.size() >= 3 && 
                (unsigned char)mnemonico[0] == 0xEF && 
                (unsigned char)mnemonico[1] == 0xBB && 
                (unsigned char)mnemonico[2] == 0xBF) {
                
                // Corta os 3 caracteres invisíveis e mantém apenas o comando real
                mnemonico = mnemonico.substr(3); 
            }
        }

        if(mnemonico == "END"){
            break;
        } 
        else if(mnemonico == "ADD_USER")
        {
            int id;
            std::string username, nome;
            entrada>> id>>username;
            std::getline(entrada >>std::ws,nome);
            cadastrarUsuario(rede, id, username.c_str(), nome.c_str(), saida);
        }
        else if(mnemonico == "FIND_USER")
        {
            int id;
            entrada >> id;
            buscarUsuarioPorId(rede, id, saida);
        }
        else if(mnemonico == "FIND_USERNAME")
        {
            std::string username;
            entrada >> username;
            buscarUsuarioPorUsername(rede, username.c_str(), saida);
        }
        else if(mnemonico == "LIST_USERS")
        {
            listarUsuarios(rede, saida);
        }
        else if(mnemonico == "FOLLOW")
        {
            int idSeguidor;
            int idSeguido;
            entrada >> idSeguidor >> idSeguido;
            seguirUsuario(rede, idSeguidor, idSeguido, saida);
        }
        else if(mnemonico == "LIST_FOLLOWING")
        {
            int idUsuario;
            entrada >> idUsuario;
            listarSeguindo(rede, idUsuario, saida);
        }
        else if(mnemonico == "ADD_POST")
        {
            int idPost, idAutor, timestamp;
            std::string texto;
            entrada >> idPost >> idAutor >> timestamp;
            std::getline(entrada>>std::ws,texto);
            cadastrarPublicacao(rede, idPost, idAutor, timestamp, texto.c_str(), saida);
        }
        else if(mnemonico == "LIKE")
        {
            int idUsuario, idPost;
            entrada >> idUsuario >> idPost;
            curtirPublicacao(rede, idUsuario, idPost, saida);
        }
        else if(mnemonico == "GET_NOTIFICATIONS")
        {
            int idUsuario, k;
            entrada >> idUsuario >> k;
            consultarNotificacoes(rede, idUsuario, k, saida);
        }
        else if(mnemonico == "FEED")
        {
            int idUsuario, k;
            entrada >> idUsuario >> k;
            gerarFeed(rede,idUsuario, k, saida);
        }
        else if(mnemonico == "TOP_POSTS")
        {
            int k;
            entrada >> k;
            listarTopPosts(rede, k, saida);
        }
        else if(mnemonico == "ADD_COMMENT")
        {
            int idUsuario, idPost;
            std::string comentario;
            entrada >> idUsuario >> idPost;
            std::getline(entrada>>std::ws,comentario);
            ComentarPublicacao(rede, idUsuario, idPost, comentario.c_str(), saida);
        }
        else if(mnemonico == "COMMENTS")
        {
            int id_post;
            entrada >> id_post;
            ListarComentarios(rede, id_post, saida);
        }
        else if (mnemonico == "UNFOLLOW")
        {
            int idSeguidor, idSeguido;
            entrada >> idSeguidor >> idSeguido;
            UNFOLLOW(rede,idSeguidor,idSeguido,saida);
        }
        else if (mnemonico == "REMOVE_POST")
        {
            int idUsuario,idPost;
            entrada >> idUsuario >> idPost;
            RemoverPublicacao(rede,idUsuario,idPost,saida);
        }
        else
        {
            saida << "ERROR INVALID_COMMAND (Tamanho: " << mnemonico.size() 
                  << " | Conteudo: [" << mnemonico << "])" << std::endl;
            //saida << "ERROR INVALID_COMMAND" << std::endl;
            
            char c;
            while(entrada.get(c)) 
            {
                if (c == '\n') //Condição para nao quebrar o codigo caso tenha tido algum comando invalido/digitado errado
                {
                    break;
                }
            }
        }
    }

}

void cadastrarUsuario(MiniRede& rede, int id, const char username[], const char nomeCompleto[], std::ostream& saida) {
    if (buscarArvoreporID(rede.raiz_id,id)!=nullptr) //Verifica se já nao existe
    {
        saida <<"ERROR USER_EXISTS"<< std::endl;
        return;
    }
    if (buscarHashPorUsername(rede.tabela_usernames,username)!=nullptr) //Verifica se hash já nao existe
    {
        saida <<"ERROR USER_EXISTS"<< std :: endl;
        return;
    }
    Usuario* novo_usuario = new Usuario;

    novo_usuario->id = id;
    strcpy(novo_usuario->nome,nomeCompleto); //Copia a string de nome completo para o endereço do nodo.
    strcpy(novo_usuario->username, username); //Copia a string de username para o endereço do nodo.
    
    novo_usuario->posts_do_usuario = nullptr;
    novo_usuario->inicio_notificacoes = nullptr;
    novo_usuario->fim_notificacoes = nullptr;
    novo_usuario->seguidos = nullptr;
    
    inserirnaArvore(rede.raiz_id,novo_usuario); //Guarda usuario na arvore
    inserirNaTabelaHash(rede.tabela_usernames,novo_usuario); //Gera hash e guarda o ponteiro na lista de hashs

    saida <<"USER_ADDED"<< std::endl;
    
}

void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida) {
    Usuario *user_found = buscarArvoreporID(rede.raiz_id,id);

    if (user_found ==nullptr)
    {
        saida <<"ERROR USER_NOT_FOUND" <<std::endl;
        return;
    }
    saida<< "USER_FOUND "
    <<user_found->id<<" "
    <<user_found->username<<" "
    <<user_found->nome<< std:: endl;
    return;
    
}

void buscarUsuarioPorUsername(MiniRede& rede, const char username[], std::ostream& saida) {
        Usuario* user_found = buscarHashPorUsername(rede.tabela_usernames,username);

        if (user_found == nullptr)
        {
            saida<< "ERROR USER_NOT_FOUND" <<std:: endl;
            return;
        }

    saida<< "USER_FOUND "
    <<user_found->id<<" "
    <<user_found->username<<" "
    <<user_found->nome<< std:: endl;
    return;
        
}

void listarUsuarios(MiniRede& rede, std::ostream& saida) {
    saida <<"USERS_BEGIN" << std:: endl;

    if (rede.raiz_id != nullptr)
    {
        percorrerArvoreEmOrdem(rede.raiz_id,saida);
    }

    saida << "USERS_END"<< std::endl;
    
}

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida) {
    Usuario*seguidor = buscarArvoreporID(rede.raiz_id,idSeguidor);
    Usuario *seguido = buscarArvoreporID(rede.raiz_id,idSeguido);

    if(seguidor == nullptr || seguido ==nullptr)
    {saida<<"ERROR USER_NOT_FOUND"<<std::endl;
        return;
    }
    if (idSeguidor == idSeguido)
    {
        saida << "ERROR CANNOT_FOLLOW_SELF"<< std::endl;
        return;
    }
    
    if (jaSegueUsuario(seguidor->seguidos,idSeguido))
    {
        saida <<"ERROR USER_ALREADY_FOLLOWING"<< std::endl;return;
    }

    InserirNalistadeSeguidos(seguidor->seguidos,idSeguido);
    enfileirarNotificacao(seguido,'F',idSeguidor,-1,"");

    saida << "FOLLOWED" <<std:: endl;
}

void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida) {
    Usuario* usuario_principal = buscarArvoreporID(rede.raiz_id,idUsuario);
    if (usuario_principal ==nullptr)
    {
        saida <<"ERROR USER_NOT_FOUND"<< std:: endl;
        return;
    }
    saida <<"FOLLOWING BEGIN" << std::endl;

    NoListaUsuario*atual= usuario_principal->seguidos;

    while (atual != nullptr)
    {
        Usuario*seguido = buscarArvoreporID(rede.raiz_id,atual->id);

        if (seguido!= nullptr)
        {
            saida << "USER " 
            << seguido->id << " " 
            << seguido->username << " " 
            << seguido->nome << std::endl;
        }
        atual = atual->prox;
    }
    saida <<"FOLLOWING_END"<<std::endl;
    
}

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream& saida) {
    Usuario *autor_da_publi = buscarArvoreporID(rede.raiz_id,idAutor);
    if (autor_da_publi== nullptr)
    {
        saida << "ERROR USER_NOT_FOUND"<< std:: endl;
        return;
    }
    inserirNalistaPosts(autor_da_publi->posts_do_usuario,idPost,texto,idAutor,timestamp);

    saida <<"POST_ADDED"<< std::endl;
    
}

void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida) {
    Usuario* usuario_curtiu = buscarArvoreporID(rede.raiz_id,idUsuario);
    if(usuario_curtiu == nullptr){
        saida <<"ERROR USER_NOT_FOUND"<< std::endl;
        return;
    }
    Usuario* autor_post = buscarDonodoPost(rede.raiz_id,idPost);
    if (autor_post == nullptr)
    {
         saida <<"ERROR POST_NOT_FOUND"<< std::endl;
        return;    
    }
    Publicacao *post_alvo = buscarPostNalista(autor_post->posts_do_usuario,idPost);
    if (post_alvo== nullptr)
    {
        saida << "ERROR POST_NOT_FOUND"<< std::endl;
    }
    
    if (jaCurtiuPost(post_alvo->curtidas,idUsuario))
    {
        saida << "ERROR ALREADY_LIKED"<< std:: endl;
        return;
    }

    post_alvo->qtd_likes++;
    inserirNaListaCurtidas(post_alvo->curtidas,idUsuario);

    enfileirarNotificacao(autor_post,'L',idUsuario,idPost,"");

    saida << "LIKED" << std:: endl;  
}

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida) {
    Usuario*User = buscarArvoreporID(rede.raiz_id,idUsuario);
    
    if(User == nullptr)
    {
        saida << "ERROR USER_NOT_FOUND" << std:: endl; //Usuario nao encontrado
    }
    else if (User->inicio_notificacoes == nullptr)
    {
        saida << "USER_DON'T_HAVE_NOTIFICATIONS" << std:: endl; //Usuario nao possui notificações
    }
    else
    {
        int count = 0;
        
        saida << "NOTIFICATIONS_BEGIN" << std:: endl;
        
        while(count < k && User->inicio_notificacoes != nullptr) //Ele deve parar quando chegar no fim da fila e/ou count superar k.
        {
            if(User->inicio_notificacoes->tipo == 'F')
            {
                saida << "NOTIFICATION FOLLOW " << User->inicio_notificacoes->de_usuario_id << std:: endl;
            }
            else if(User->inicio_notificacoes->tipo == 'L')    
            {
                saida << "NOTIFICATION LIKE " << User->inicio_notificacoes->de_usuario_id <<" "<< User->inicio_notificacoes->post_ID << std:: endl;
            }
            else if(User->inicio_notificacoes->tipo == 'C')
            {
                saida << "NOTIFICATION COMMENT " << User->inicio_notificacoes->de_usuario_id <<" "<< User->inicio_notificacoes->post_ID << " "<< User->inicio_notificacoes->comentario<<std:: endl;
            }
            DesenfileirarNotificacao(User); // Aqui, ele altera o ponteiro da fila para a proxima notificação, e deleta a atual.
            count++;
        }
        
        saida << "NOTIFICATIONS_END" << std:: endl;
    }
}

void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida) {
    Usuario*User = buscarArvoreporID(rede.raiz_id,idUsuario);

    if(User == nullptr)
    {
        saida << "ERROR USER_NOT_FOUND" << std:: endl; //Usuario nao encontrado
    }
    else
    {
        Publicacao** Posts = new Publicacao*[k](); //Aloca um vetor de publicações com o tamanho k para auxiliar na ordenação
        TimestampSort(rede,User, k,Posts); 
        
        saida << "FEED_BEGIN" << std::endl;
     
        for(int i = 0; i < k; i++)
        {
            if (Posts[i]==nullptr)break;
           saida << "POST " 
           <<Posts[i]->id_da_publicacao<<" "
           <<Posts[i]->autor_id<<" "
           <<Posts[i]->timestamp<<" "
           <<Posts[i]->qtd_likes<<" "
           <<Posts[i]->texto_da_publicacao<<std::endl;
        }
    
        saida <<"FEED_END"<<std::endl;
        delete []Posts; //Deleta o vetor de ponteiros, liberando memoria.
    }
}

void listarTopPosts(MiniRede& rede, int k, std::ostream& saida) {
    Publicacao** Posts = new Publicacao*[k](); //Mesma ideia do gerarFeed
    int qtde_post = 0;
    
    CurtidasSort(rede.raiz_id, k, qtde_post, Posts); //Aqui, iremos passar um int qtde_post, para auxiliar na ordenação
    
    saida << "TOP_POSTS_BEGIN" << std:: endl;
    
    for(int i = 0; i < k; i++)
    {
        if (Posts[i]== nullptr)break;
        saida << "POST " 
        <<Posts[i]->id_da_publicacao<<" "
        <<Posts[i]->autor_id<<" "
        <<Posts[i]->timestamp<<" "
        <<Posts[i]->qtd_likes<<" "
        <<Posts[i]->texto_da_publicacao<<std::endl;
    }

    saida << "TOP_POSTS_END" << std:: endl;
    delete []Posts; //Deleta o vetor de ponteiros, liberando memoria.
}

void UNFOLLOW(MiniRede &rede, int Id_seguidor, int idSeguido, std ::ostream &saida)
{
    Usuario *seguidor = buscarArvoreporID(rede.raiz_id,Id_seguidor);
    Usuario* seguido = buscarArvoreporID(rede.raiz_id,idSeguido);

    if (seguido == nullptr || seguidor == nullptr)
    {
        saida << "ERROR USER_NOT_FOUND" <<std::endl;
        return;
    }
    if (!jaSegueUsuario(seguidor->seguidos, idSeguido))
    {
        saida << "ERROR USER NOT_FOLLOWING" <<std::endl;
        return;
    }
    RemoverNoListaUsuario(seguidor->seguidos, idSeguido);
    saida<< "USER_UNFOLLOWED"<< std::endl;
}

void RemoverPublicacao(MiniRede &rede, int id_usuario,int id_post,std::ostream &saida){
    Usuario* usuario = buscarArvoreporID(rede.raiz_id,id_usuario);
    if (usuario == nullptr)
    {
        saida << "ERROR USER_NOT_FOUND"<< std::endl;
        return;
    }
    Publicacao* post_deletar = RemoverNolistaPost(usuario->posts_do_usuario,id_post);

    if (post_deletar == nullptr)
    {
        saida << "ERROR POST_NOT_FOUND"<< std::endl;
        return;
    }
    
    RemoverNolistaPost(rede.todos_os_posts,id_post);

    liberarListadeUsuarios(post_deletar->curtidas);

    liberarListadeComentarios(post_deletar->comentarios);
    delete post_deletar;

    saida<< "POST_REMOVED" <<std::endl;
}

void ComentarPublicacao(MiniRede& rede, int id_usuario, int id_post, const char comentario[], std::ostream& saida){
    //O usuario x comenta na publicacao do usuario y.
    Usuario* comentarista = buscarArvoreporID(rede.raiz_id, id_usuario);
    if(comentarista == nullptr)
    {
        saida << "ERROR USER_NOT_FOUND" << std::endl;
        return;
    }
    
    Usuario* dono_do_post = buscarDonodoPost(rede.raiz_id, id_post);
    if(dono_do_post == nullptr)
    {
        saida << "ERROR POST_NOT_FOUND" << std::endl;
        return;
    }
    
    Publicacao* post_alvo = buscarPostNalista(dono_do_post->posts_do_usuario, id_post);
    if (post_alvo == nullptr)
    {
        saida << "ERROR POST_NOT_FOUND"<< std::endl;
        return;
    }

    InserirNaListaComentarios(post_alvo->comentarios, comentarista->id, comentario); //Passamos como parametro o ponteiro da lista de comentarios daquele post 
    enfileirarNotificacao(dono_do_post, 'C', id_usuario, id_post, comentario); //Dono do post recebe uma notificacao do comentário 

    saida<< "COMMENT_ADDED" << std::endl;
}

void ListarComentarios(MiniRede& rede, int id_post, std::ostream& saida){
    Usuario* autor_post = buscarDonodoPost(rede.raiz_id, id_post);
    if(autor_post == nullptr)
    {
        saida << "ERROR POST_NOT_FOUND" << std::endl;
        return;
    }

    Publicacao* post_alvo = buscarPostNalista(autor_post->posts_do_usuario, id_post);
    if (post_alvo == nullptr)
    {
        saida << "ERROR POST_NOT_FOUND"<< std::endl;
        return;
    }
    
    NoListaComentarios* temp = post_alvo->comentarios;

    saida <<"COMMENTS_BEGIN"<<std::endl;

    while (temp != nullptr) //Percorrremos a lista de comentarios daquele post, printando quem comentou e o que comentou
    {
        saida << "COMMENT "
        <<temp->id <<" "
        <<temp->comentario<<std::endl;
    
        temp = temp->prox;
    }
    saida <<"COMMENTS_END"<<std::endl;
}


int main() {
    MiniRede rede;

    inicializarMiniRede(rede);
    processarComandos(rede, std::cin, std::cout);
    liberarMiniRede(rede);

    return 0;
}

