#include "minirede.h";
#include <cstring>


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
void enfileirarNotificacao(Usuario* usuario_recebe,char tipo, int id_de_origem,int id_do_post){
    NoFilaNotificacoes*novo_no = new NoFilaNotificacoes();

    novo_no->tipo = tipo;
    novo_no->de_usuario_id = id_de_origem;
    novo_no->post_ID = id_do_post;
    novo_no->prox = nullptr;

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
void inserirNaListaCurtidas(NoListaUsuario* inicio_lista,int id_quem_curitu){
    NoListaUsuario* novo_no -= new NoListaUsuario();
    novo_no->id = id_quem_curitu;
    novo_no-> prox = inicio_lista;
    inicio_lista=novo_no;
}

