#include "minirede.h";



/// FUNCAO AUXILIAR PRA LIMPAR LISTA DE USUARIOS QUE CURTIRAR A PUBLICACAO;
void liberarListadeCurtidas(NoListaUsuario* inicio){
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
            liberarListadeCurtidas(atual->publicacao_atual->curtidas);
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