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
    // TODO
    liberarpostGLobal(rede);
    liberarArvoredeUsuarios(rede.raiz_id);
    rede.raiz_id = nullptr;
    liberarTabelaHash(rede.tabela_usernames);

}

void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida) {
    // TODO: ler comandos da entrada padrao ate END.
    // Para cada comando, chamar a funcao correspondente.
    // Nao imprimir menu, prompt ou texto extra.
}

void cadastrarUsuario(MiniRede& rede, int id, const char username[], const char nomeCompleto[], std::ostream& saida) {
    if (buscarArvoreporID(rede.raiz_id,id) != nullptr)
    {
        saida <<"ERROR USER_EXISTS"<< std::endl;
        return;
    }
    if (buscarHashPorUsername(rede.tabela_usernames,username)!=nullptr )
    {
        saida <<"ERROR USER_EXISTS"<< std :: endl;
        return;
    }
    Usuario* novo_usuario = new Usuario;

    novo_usuario->id = id;
    strcpy(novo_usuario->nome,nomeCompleto);
    strcpy(novo_usuario->username, username);
    
    novo_usuario->posts_do_usuario = nullptr;
    novo_usuario->inicio_notificacoes = nullptr;
    novo_usuario->fim_notificacoes = nullptr;
    novo_usuario->seguidos = nullptr;
    
    inserirnaArvore(rede.raiz_id,novo_usuario);
    inserirNaTabelaHash(rede.tabela_usernames,novo_usuario);

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

    if(seguidor == nullptr || seguido ==nullptr){saida<<"USER_NOT_FOUND"<<endl;return;}
    if (jaSegueUsuario(seguidor->seguidos,idSeguido))
    {
        saida <<"ERROR USER_ALREADY_FOLLOWING"<< std::endl;return;
    }

    InserirNalistadeSeguidos(seguidor->seguidos,idSeguido);
    enfileiraNotificacao(seguido,'F',idSeguidor,-1);

    saida << "USER_FOLLOWED" <<std:: endl;
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
    Usuario* autor_post = buscarArvoreporID(rede.raiz_id,idPost);
    if (autor_post == nullptr)
    {
         saida <<"ERROR USER_NOT_FOUND"<< std::endl;
        return;    
    }
    Publicacao *post_alvo = buscarPostNalista(autor_post->posts_do_usuario,idPost)
    if (post_alvo == nullptr)
    {
        saida << "ERROR POST_NOT_FOUND"<< std:: endl;
        return;
    }
    
    
    

}

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida) {
    Usuario*User = buscarArvoreporID(rede.raiz_id,idUsuario);
    
    if(User == nullptr)
    {
        saida << "ERROR USER_NOT_FOUND\n"; //Usuario nao encontrado
    }
    else if (User->inicio_notificacoes == nullptr)
    {
        saida << "ERROR USER_DON'T_HAVE_NOTIFICATIONS\n"; //Usuario nao possui notificações
    }
    else
    {
        int count = 0;
        
        saida << "NOTIFICATIONS_BEGIN\n";
        
        while(count < k && User->inicio_notificacoes != nullptr) //Ele deve parar quando chegar no fim da fila e/ou count superar k.
        {
            if(User->inicio_notificacoes->tipo == 'F')
            {
                saida << "NOTIFICATION FOLLOW " << User->de_usuario_id << "  \n";
            }
            if(User->inicio_notificacoes->tipo == 'L')    
            {
                saida << "NOTIFICATION LIKE " << User->de_post_ID << "  \n";
            }
            
            DesenfileirarNotificacao(User); // Aqui, ele altera o ponteiro da fila para a proxima notificação, e deleta a atual.
            count++;
        }
        
        saida << "NOTIFICATIONS_END\n";
    }
}

void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida) {
    // TODO
}

void listarTopPosts(MiniRede& rede, int k, std::ostream& saida) {
    // TODO
}

int main() {
    MiniRede rede;

    inicializarMiniRede(rede);
    processarComandos(rede, std::cin, std::cout);
    liberarMiniRede(rede);

    return 0;
}

