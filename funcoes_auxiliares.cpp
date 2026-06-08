#include "minirede.h";



/// FUNCAO AUXILIARES PARA LIBERAR_MINIREDE
void liberarListadeUsuarios(NoListaUsuario* inicio){
    NoListaUsuario* atual = inicio;
    while (atual != nullptr)
    {
        NoListaUsuario* proximo = atual->prox;
        delete atual;
        atual= proximo;
    }
    return;
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
//FUNCAO AUXILIARES DA ADD_USER
Usuario* buscarArvoreporID(NoArvoreUsuarios*raiz,int id){
    if (raiz == nullptr)
    {
        return nullptr;
    }
    if (raiz ->usuario != nullptr && raiz->usuario->id){
        return raiz->usuario;
    }
    if (id< raiz->usuario->id)
    {
       return buscarArvoreporID(raiz->esq,id);
    }else{
       return buscarArvoreporID(raiz->dir,id);
    }
    
}
int gerarIndiceHash(const char* username){
    int soma = 0;
    for(int i=0;username[i]!= '\0';i++){
        soma+= username[i];
    }
    return soma% TAM_HASH;
}
Usuario* buscarHashPorUsername(NoHashUsuario* tabela[],const char* username_procurado){
    int indice = gerarIndiceHash(username_procurado);

    NoHashUsuario* atual = tabela[indice];
    while(atual != nullptr){
        if(atual->usuario != nullptr && strcmp(atual->usuario->username,username_procurado))
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