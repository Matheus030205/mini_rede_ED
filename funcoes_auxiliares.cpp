#include "minirede.h";



/// FUNCAO AUXILIAR PRA LIMPAR LISTA DE USUARIOS QUE CURTIRAR A PUBLICACAO;
void liberarListadeUsuarios(NoListaUsuario* inicio){
    NoListaUsuario* atual = inicio;
    while (atual != nullptr)
    {
        NoListaUsuario* proximo = atual->prox;
        delete atual;
        atual= proximo;
    }
}
void liberarpostGLobal(MiniRede& rede){

    NoLista_de_Post* atual = rede.todos_os_posts;

    while (atual != nullptr)
    {
        NoLista_de_Post* prox = atual->prox;

        if(atual->publicacao_atual != nullptr){
            liberarListadeUsuarios(atual->publicacao_atual->curtidas);
            //Depois de deletar lista de curtidas deleta a struct da publicacao
            delete atual->publicacao_atual;
        }
        //DELETA NO DA LISTA DE TODAS AS PUBLICACOES
        delete atual;
        atual= prox;
    }
    rede.todos_os_posts = nullptr;
    //POR FINAL LIBERAR O PONTEIRO GLOBAL DAS PUBLICACOES
    
}
void liberarListaPostsdoUsuario(NoLista_de_Post* inicio){
    NoLista_de_Post *atual = inicio;
    while (atual != nullptr)
    {
        NoLista_de_Post *prox = atual->prox;
        delete atual;//DELETA NO DA LISTA, N O POST;
        atual =prox;
    }
}
void liberarFilaNotificacoes(NoFilaNotificacoes * inicio){
    NoFilaNotificacoes* atual= inicio;

    while (atual!= nullptr)
    {
        NoFilaNotificacoes *prox= atual->prox;
        delete atual;
        atual = prox;
    }
}
void liberarArvoredeUsuarios(NoArvoreUsuarios* raiz){
    if (raiz == nullptr) return;

    liberarArvoredeUsuarios(raiz->esq);
    liberarArvoredeUsuarios(raiz->dir);

    if (raiz->usuario != nullptr)
    {
        liberarListadeUsuarios(raiz->usuario->seguidos);
        liberarListaPostsdoUsuario(raiz->usuario->posts_do_usuario);
        liberarFilaNotificacoes(raiz->usuario->inicio_notificacoes);

        delete raiz->usuario;
    }
    delete raiz;
}
void liberarTabelaHash(NoHashUsuario* inicio[]){
    for(int i = 0 ; i< TAM_HASH;i++){
        NoHashUsuario *atual = inicio[i];
        while (atual != nullptr)
        {
            NoHashUsuario*prox = atual->prox;
            delete atual;
            atual = prox;
        }
        inicio[i]= nullptr;
    } 
}