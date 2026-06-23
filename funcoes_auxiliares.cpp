#include "minirede.h"
#include <cstring>


/// FUNCAO AUXILIARES PARA LIBERAR_MINIREDE
void liberarListadeUsuarios(NoListaUsuario* inicio){
    
    NoListaUsuario* atual = inicio;
    
    while (atual != nullptr)
    {
        NoListaUsuario* proximo = atual->prox;
        delete atual; //Deleta no da lista, nao o usuario.
        atual = proximo;
    }
}
void liberarListadeComentarios(NoListaComentarios* inicio){
    
    NoListaComentarios* atual = inicio;
    
    while (atual != nullptr)
    {
        NoListaComentarios* proximo = atual->prox;
        delete atual; //Deleta no da lista, nao o usuario.
        atual = proximo;
    }
}
void liberarpostGLobal(MiniRede& rede){

    NoLista_de_Post* atual = rede.todos_os_posts;

    while (atual != nullptr)
    {
        NoLista_de_Post* prox = atual->prox;

        if(atual->publicacao_atual != nullptr){
            liberarListadeUsuarios(atual->publicacao_atual->curtidas); //Passamos como parametro a lista de usuarios que CURTIRAM a publicação
            liberarListadeComentarios(atual->publicacao_atual->comentarios); //Passamos como parametro a lista de usuarios que COMENTARAM na publicação
            delete atual->publicacao_atual; //Depois de deletar lista de curtidas, deleta a struct da publicacao
        }
        delete atual; //Deleta o no de todas as publicações
        atual= prox;
    }
    rede.todos_os_posts = nullptr; //Libera o ponteiro global das publicações
}
void liberarListaPostsdoUsuario(NoLista_de_Post* inicio){
    NoLista_de_Post* atual = inicio;
    while (atual != nullptr)
    {
        NoLista_de_Post* prox = atual->prox;
        delete atual; //Deleta o no da lista, nao o post em si
        atual = prox;
    }
}
void liberarFilaNotificacoes(NoFilaNotificacoes* inicio){
    NoFilaNotificacoes* atual = inicio;

    //loop que passa pela lista de notificações
    while (atual != nullptr)
    {
        NoFilaNotificacoes* prox = atual->prox;
        delete atual;
        atual = prox;
    }
}

//Função principal do liberar rede
void liberarArvoredeUsuarios(NoArvoreUsuarios* raiz){
    if (raiz == nullptr) return;

    //Percorre a arvore em pos ordem, para deletar as folhas primeiro
    liberarArvoredeUsuarios(raiz->esq);
    liberarArvoredeUsuarios(raiz->dir);

    if (raiz->usuario != nullptr)
    {
        liberarListadeUsuarios(raiz->usuario->seguidos); //Deleta a lista de seguindo do usuario
        liberarListaPostsdoUsuario(raiz->usuario->posts_do_usuario); //Deleta a lista de posts do usuario
        liberarFilaNotificacoes(raiz->usuario->inicio_notificacoes); //Deleta as notificações do usuario

        delete raiz->usuario; //Deletamos o usuario
    }
    delete raiz; //Deletamos o seu ponteiro
}
void liberarTabelaHash(NoHashUsuario* inicio[]){
    for(int i = 0 ; i< TAM_HASH;i++){
        NoHashUsuario *atual = inicio[i];
        while (atual != nullptr)
        {
            NoHashUsuario*prox = atual->prox;
            delete atual; //Deletamos o conteudo no indice i do vetor
            atual = prox;
        }
        inicio[i]= nullptr; //Apos deletado, limpamos o vetor
    } 
}
//FUNCAO AUXILIARES DA ADD_USER
Usuario* buscarArvoreporID(NoArvoreUsuarios* raiz, int id){
    if (raiz == nullptr)
    {
        return nullptr;
    }
    if (raiz->usuario != nullptr && raiz->usuario->id == id){
        return raiz->usuario;
    }
    
    if (id< raiz->usuario->id)
    {
       return buscarArvoreporID(raiz->esq,id);
    }
    else
    {
       return buscarArvoreporID(raiz->dir,id);
    }
}
int gerarIndiceHash(const char* username){
    int soma = 0;
    
    //Coleta cada caracter do username do usuario, e então soma o valor ASCII dos caracteres
    for(int i=0; username[i] != '\0'; i++){
        soma += username[i];
    }
    return soma%TAM_HASH; //Divide a soma pelo modulo, que sera o indice do nosso vetor de hashs
}
Usuario* buscarHashPorUsername(NoHashUsuario* tabela[],const char* username_procurado){
    int indice = gerarIndiceHash(username_procurado);

    NoHashUsuario* atual = tabela[indice];
    //Loop while até certificar de que o username procurado coincide
    while(atual != nullptr){
        if(atual->usuario != nullptr && !strcmp(atual->usuario->username,username_procurado))
        {
            return atual->usuario;
        }
        atual = atual ->prox;
    }
    return nullptr;
}
void inserirnaArvore(NoArvoreUsuarios*& raiz, Usuario* novo_usuario){
    if (raiz == nullptr){
        raiz = new NoArvoreUsuarios{novo_usuario,nullptr,nullptr};
        return;
    }
    if(novo_usuario->id < raiz->usuario->id){
        inserirnaArvore(raiz->esq,novo_usuario);
    }else{
        inserirnaArvore(raiz->dir,novo_usuario);
    }
}
void inserirNaTabelaHash(NoHashUsuario* tabela[], Usuario* novo_usuario){
    int indice = gerarIndiceHash(novo_usuario->username);
    NoHashUsuario* novo_no = new NoHashUsuario;

    novo_no->usuario = novo_usuario;

    novo_no->prox = tabela[indice];

    tabela[indice] = novo_no;

}
//FUNCAO AUXILIAR LISTAR USUARIO ORDEM CRESCENTE ID
void percorrerArvoreEmOrdem(NoArvoreUsuarios* raiz,std::ostream& saida){
    if(raiz == nullptr) return;

    percorrerArvoreEmOrdem(raiz->esq,saida);

    if (raiz->usuario != nullptr)
    {
        saida << "USER " 
              << raiz->usuario->id << " " 
              << raiz->usuario->username << " " 
              << raiz->usuario->nome << std::endl;
    }
    percorrerArvoreEmOrdem(raiz->dir,saida);
}
//FUNCAO AUXILIARES PARA FOLLOW_USER
bool jaSegueUsuario(NoListaUsuario* inicio_lista,int id_alvo){
    NoListaUsuario* atual = inicio_lista;
    while (atual != nullptr)
    {
        if (atual->id == id_alvo)
        {
            return true;
        }
        atual = atual->prox;
    }
    return false;
}
void InserirNalistadeSeguidos(NoListaUsuario*& inicio_lista,int id_a_seguir){
    NoListaUsuario *novo_no = new NoListaUsuario;

    novo_no->id = id_a_seguir;
    novo_no->prox = inicio_lista;

    inicio_lista = novo_no;
}
void enfileirarNotificacao(Usuario* usuario_recebe,char tipo, int id_de_origem, int id_do_post, const char comment[]){
    NoFilaNotificacoes*novo_no = new NoFilaNotificacoes();

    novo_no->tipo = tipo;
    novo_no->de_usuario_id = id_de_origem;
    novo_no->post_ID = id_do_post;
    novo_no->prox = nullptr;
    strcpy(novo_no->comentario, comment);
    if(usuario_recebe->inicio_notificacoes == nullptr){
        usuario_recebe->inicio_notificacoes = novo_no;
        usuario_recebe->fim_notificacoes = novo_no;
    }else{
        usuario_recebe->fim_notificacoes->prox = novo_no;
        usuario_recebe->fim_notificacoes = novo_no;
    }
    return;
}
///FUNCOES AUXILIAR CADASTRAR PUBLICACAO
void inserirNalistaPosts(NoLista_de_Post*& inicio_lista,int post_id,const char texto[],int autor_id,int timestamp){
    Publicacao* nova_publicacao = new Publicacao();
    nova_publicacao->id_da_publicacao = post_id;
    nova_publicacao->autor_id= autor_id;
    nova_publicacao->timestamp = timestamp;
    strcpy(nova_publicacao->texto_da_publicacao,texto);
    nova_publicacao->qtd_likes = 0;
    nova_publicacao->curtidas = nullptr;
    NoLista_de_Post*novo = new NoLista_de_Post();
    novo->publicacao_atual = nova_publicacao;
    novo->prox = inicio_lista;
    inicio_lista = novo;
}
//FUNCAO AUXILIAR CURTIR PUBLICACAO
Publicacao* buscarPostNalista(NoLista_de_Post*inicio_lista,int id_procurado){
    NoLista_de_Post* atual = inicio_lista;
    while (atual != nullptr)
    {
        if(atual ->publicacao_atual != nullptr && atual->publicacao_atual->id_da_publicacao == id_procurado){
            return atual->publicacao_atual;
        }
        atual = atual->prox;
    }
    return nullptr;
}
void inserirNaListaCurtidas(NoListaUsuario*& inicio_lista,int id_quem_curitu){
    NoListaUsuario* novo_no = new NoListaUsuario();
    novo_no->id = id_quem_curitu;
    novo_no-> prox = inicio_lista;
    inicio_lista=novo_no;
}
Usuario * buscarDonodoPost(NoArvoreUsuarios*raiz, int id_pos_procurado){
    if (raiz == nullptr) return nullptr;
    
    Usuario *encontrado_esq = buscarDonodoPost(raiz->esq,id_pos_procurado);
    if (encontrado_esq != nullptr) return encontrado_esq;

    NoLista_de_Post* atual_post = raiz->usuario->posts_do_usuario;
        while (atual_post!= nullptr)
        {
            if (atual_post->publicacao_atual != nullptr && atual_post->publicacao_atual->id_da_publicacao==id_pos_procurado)
            {
                return raiz->usuario;
            }
            atual_post = atual_post->prox;
        }
        return buscarDonodoPost(raiz->dir,id_pos_procurado);
    
}
bool jaCurtiuPost(NoListaUsuario* inicio_lista,int id_usuario){
    NoListaUsuario *atual = inicio_lista;
    while (atual != nullptr)
    {
        if (atual->id==id_usuario)
        {
            return true;
        }
        atual = atual->prox;
    }
    return false;
    
}
//FUNCAO AUXILIAR CONSULTAR NOTIFICACOES
void DesenfileirarNotificacao(Usuario*& User){
    NoFilaNotificacoes* Notification = User->inicio_notificacoes; //Percorre a fila de notificações
    
    User->inicio_notificacoes = Notification->prox;
    if(User->inicio_notificacoes == nullptr) User->fim_notificacoes = nullptr; //Caso tenha chegado no fim da fila, atualiza os ponteiros
    delete Notification; //Deleta a notificacao
}
//FUNCAO AUXILIAR GERAR FEED
bool ComparaPost(Publicacao* atual, Publicacao* anterior){
    //Se os timestamps forem diferentes, a função retorna o atual apenas se for maior; com timestamps iguais, retorna atual apenas se tiver um id de post menor
    if(atual->timestamp != anterior->timestamp) 
    {
        return atual->timestamp > anterior->timestamp;
    }
    return atual->id_da_publicacao < anterior->id_da_publicacao;
}
void TimestampSort(MiniRede& rede,Usuario*& User, int k, Publicacao** Posts){
    NoListaUsuario* following = User->seguidos;
    int qtde_post = 0; //Como iremos criar loops para percorrer a lista, podemos criar uma variavel que nos auxilia para indexar corretamente o post no vetor.

    while(following != nullptr) //Percorre pela lista de usuarios seguidos do nosso usuario, para gerar o feed
    {
        Usuario* usuario_temp = buscarArvoreporID(rede.raiz_id,following->id); //Criamos um temporario do usuario seguido

        if (usuario_temp != nullptr) //Executa apenas se encontrou um usuario
        {
            NoLista_de_Post* post_temp = usuario_temp->posts_do_usuario; //Criamos um temporario para a lista de posts do usuario seguido
            while(post_temp != nullptr) //Percorre pela lista de posts do usuario seguido
            {
                Publicacao* post_atual = post_temp->publicacao_atual; //Criamos um temp da publicacao que sera injetada no vetor
            
                if(qtde_post < k) //enquanto nao estourou o limite do vetor k
                {
                    Posts[qtde_post] = post_atual; //inserimos na posicao qtde_post
                    qtde_post++; //Atualizamos quantidade de posts 
                
                    if(qtde_post > 1) //Apos mais de um post, começamos a comparar os posts entre si
                    {
                        for(int i = qtde_post - 1; i > 0; i--) //INSERTION SORT
                        {
                            if(ComparaPost(Posts[i], Posts[i-1])) //Retorna o maior post
                            {
                                Publicacao* swap = Posts[i];
                                Posts[i] = Posts[i-1];
                                Posts[i-1] = swap;
                            }
                        }
                    }
                
                }
                else //Apos quantidade de posts igualar e/ou ultrapassar o valor de k, começamos a fazer as comparaçoes diretas, sem precisar somar mais na qtde_post
                {
                    if(ComparaPost(post_atual, Posts[k-1])) //Executa apenas se o proximo post vence o menor post do vetor k, pois se nao, nao ha porque comparar com os outros.
                    {
                        Posts[k-1] = post_atual;
                        for(int i = k - 1; i > 0; i--) //INSERTION SORT
                        {
                            if(ComparaPost(Posts[i], Posts[i-1]))
                            {
                                Publicacao* swap = Posts[i];
                                Posts[i] = Posts[i-1];
                                Posts[i-1] = swap;
                            }
                        }
                    }
                }
            
                post_temp = post_temp->prox;
            }
        }
        following = following->prox;
    }
    
}
//FUNCAO AUXILIAR TOP POSTS
bool ComparaCurtidas(Publicacao* atual, Publicacao* anterior){
    //Mesma ideia do comparapost. Se a quantidade de curitdas for diferente, função retorna o atual apenas se tiver maior quantidade de likes
    if(atual->qtd_likes != anterior->qtd_likes)
    {
        return atual->qtd_likes > anterior->qtd_likes;
    }
    return atual->id_da_publicacao < anterior->id_da_publicacao;
}
void CurtidasSort(NoArvoreUsuarios* raiz, int k, int& qtde_post, Publicacao** Posts){
    //Como nao passamos um usuario especifico como no gerar feed, e queremos todos os posts globais, precisamos percorrer a arvore de usuarios e acessar cada lista de publicações
    if(raiz == nullptr) return;
    CurtidasSort(raiz->esq, k, qtde_post, Posts);
    CurtidasSort(raiz->dir, k, qtde_post, Posts);

    //Se tratando de uma função recursiva, criamos o int qtde_post fora da função auxiliar, e passando como parametro. Assim, evitamos que a variavel qtde_post se perca ou se anule.
    NoLista_de_Post* post_temp = raiz->usuario->posts_do_usuario;
    
    //Mesma ideia do TimestampSort, percorremos a lista de posts do usuario e vamos inserindo no vetor ate que qtde post iguale e/ou ultrapasse k
    while(post_temp != nullptr)
    {
        Publicacao* post_atual = post_temp->publicacao_atual; 
        
        if(qtde_post < k)
        {
            Posts[qtde_post] = post_atual; 
            qtde_post++;
            
            if(qtde_post > 1)
            {
                for(int i = qtde_post - 1; i > 0; i--) //INSERTION SORT
                {
                    if(ComparaCurtidas(Posts[i], Posts[i-1]))
                    {
                        Publicacao* swap = Posts[i];
                        Posts[i] = Posts[i-1];
                        Posts[i-1] = swap;
                    }
                }
            }   
        }
        else
        {
            if(ComparaCurtidas(post_atual, Posts[k-1]))
            {
                Posts[k-1] = post_atual;
                
                for(int i = k - 1; i > 0; i--) //INSERTION SORT
                {
                    if(ComparaCurtidas(Posts[i], Posts[i-1])) 
                    {
                        Publicacao* swap = Posts[i];
                        Posts[i] = Posts[i-1];
                        Posts[i-1] = swap;
                    }
                }
            }
        }
        post_temp = post_temp->prox;
    }
}
//FUNCAO AUXILIAR UNFOLLOW
void RemoverNoListaUsuario(NoListaUsuario*& inicio_lista,int id_remover){
    NoListaUsuario* atual = inicio_lista;
    NoListaUsuario* anterior = nullptr;

    while (atual != nullptr){
        if(atual->id == id_remover){
            if (anterior == nullptr){
                inicio_lista = atual->prox;
            }else{
                anterior->prox = atual->prox;
            }
            delete atual;
            break;
        }
        anterior = atual;
        atual= atual->prox;
    }
    
}
//FUNCAO AUXILIAR REMOVER PUBLICAÇÃO
Publicacao* RemoverNolistaPost(NoLista_de_Post*& inicio_lista, int id_post){
    NoLista_de_Post* atual = inicio_lista;
    NoLista_de_Post *anterior = nullptr;
    Publicacao* encontrado = nullptr;

    while (atual!= nullptr)
    {
        if (atual->publicacao_atual != nullptr && atual->publicacao_atual->id_da_publicacao == id_post)
        {
            encontrado = atual->publicacao_atual;
            if (anterior == nullptr)
            {
                inicio_lista = atual->prox;
            }else{
                anterior->prox = atual->prox;
            }
            delete atual;
            return encontrado;
    
        }
        
        anterior = atual;
        atual = atual->prox;    
    }
    return nullptr;
}
//FUNCAO AUXILIAR COMENTAR EM PUBLICAÇÃO
void InserirNaListaComentarios(NoListaComentarios*& inicio_lista, int id_user, const char comment[]){
    //Cria um nodo que armazena o comentario e o id de quem comentou naquele post
    NoListaComentarios* novo_no = new NoListaComentarios();
    novo_no->id = id_user;
    strcpy(novo_no->comentario, comment);
    novo_no->prox = inicio_lista; //inicio da lista é passado como parametro na função principal
    inicio_lista = novo_no;
}