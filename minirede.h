#ifndef MINI_REDE_H
#define MINI_REDE_H

#include <iostream>


const int TAM_USERNAME = 50;
const int TAM_NOME = 100;
const int TAM_TEXTO = 280;
const int TAM_COMANDO = 30;
const int TAM_HASH = 997;

struct NoListaUsuario
{
    int id;
    NoListaUsuario *prox;
};

struct NoFilaNotificacoes
{
    char tipo;
    int de_usuario_id;
    int post_ID;
    char comentario[TAM_TEXTO];
    NoFilaNotificacoes* prox;
};

struct NoListaComentarios
{
    int id; //Id do autor do comentario
    char comentario[TAM_TEXTO];
    NoListaComentarios* prox;
};

struct Publicacao
{
    int id_da_publicacao;
    int autor_id;
    int timestamp;
    char texto_da_publicacao[TAM_TEXTO];
    int qtd_likes;
    NoListaUsuario* curtidas;//LISTA DE IDS QUEM CURTIU O POST
    NoListaComentarios* comentarios; //no do inicio da LISTA DE COMENTARIOS 
};

struct NoLista_de_Post
{
    NoLista_de_Post*prox;
    Publicacao *publicacao_atual;
};


struct Usuario
{
    int id;
    char username [TAM_USERNAME];
    char nome [TAM_NOME];

    NoListaUsuario* seguidos;//NO DO INICIO DA LISTA DE PESSOAS SEGUIDAS
    NoLista_de_Post* posts_do_usuario;//No do inicio da lista de publicacoes
    NoFilaNotificacoes *inicio_notificacoes; //USADO PARA TIRAR NOTFICACOES MAIS RECENTES
    NoFilaNotificacoes *fim_notificacoes;// usado para adicionar NOTIFICACOES
};

struct NoArvoreUsuarios
{
    Usuario* usuario;
    NoArvoreUsuarios* esq;
    NoArvoreUsuarios* dir;
};

struct NoHashUsuario
{
    Usuario *usuario;
    NoHashUsuario *prox;
};

struct MiniRede {

    NoArvoreUsuarios* raiz_id; 
    NoHashUsuario* tabela_usernames[TAM_HASH]; 
    NoLista_de_Post* todos_os_posts;
};

void inicializarMiniRede(MiniRede& rede);
void liberarMiniRede(MiniRede& rede);
void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida);

void cadastrarUsuario(MiniRede& rede, int id, const char username[], const char nomeCompleto[], std::ostream& saida);
void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida);
void buscarUsuarioPorUsername(MiniRede& rede, const char username[], std::ostream& saida);
void listarUsuarios(MiniRede& rede, std::ostream& saida);

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida);
void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida);

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream& saida);
void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida);

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void listarTopPosts(MiniRede& rede, int k, std::ostream& saida);


// Funções auxiliares
void liberarListadeUsuarios(NoListaUsuario* inicio);
void liberarpostGLobal(MiniRede& rede);
void liberarListaPostdoUsuario(NoLista_de_Post* inicio);
void liberarFilaNotificacoes(NoFilaNotificacoes * inicio);
void liberarArvoredeUsuarios(NoArvoreUsuarios* raiz);
void liberarTabelaHash(NoHashUsuario* inicio[]);

Usuario* buscarArvoreporID(NoArvoreUsuarios*raiz,int id);
int gerarIndiceHash(const char* username);
Usuario* buscarHashPorUsername(NoHashUsuario* tabela[],const char* username_procurado);
void inserirnaArvore(NoArvoreUsuarios*& raiz, Usuario* novo_usuario);
void inserirNaTabelaHash(NoHashUsuario* tabela[], Usuario* novo_usuario);

bool jaSegueUsuario(NoListaUsuario* inicio_lista,int id_alvo);
void percorrerArvoreEmOrdem(NoArvoreUsuarios* raiz,std::ostream& saida);
void InserirNalistadeSeguidos(NoListaUsuario*& inicio_lista,int id_a_seguir);
void enfileirarNotificacao(Usuario* usuario_recebe,char tipo, int id_de_origem,int id_do_post, const char comment[]);
void inserirNalistaPosts(NoLista_de_Post*& inicio_lista,int post_id,const char texto[],int autor_id,int timestamp);

void inserirNaListaCurtidas(NoListaUsuario*& yinicio_lista,int id_quem_curitu);
Publicacao* buscarPostNalista(NoLista_de_Post*inicio_lista,int id_procurado);
bool jaCurtiuPost(NoListaUsuario* inicio_lista,int id_usuario);
Usuario * buscarDonodoPost(NoArvoreUsuarios*raiz, int id_pos_procurado);

void DesenfileirarNotificacao(Usuario*& User);

bool ComparaPost(Publicacao* atual, Publicacao* anterior);
void TimestampSort(MiniRede& rede,Usuario*& User, int k, Publicacao** Posts);

bool ComparaCurtidas(Publicacao* atual, Publicacao* anterior);
void CurtidasSort(NoArvoreUsuarios* raiz, int k,int& qtde_post, Publicacao** Posts);

void RemoverNoListaUsuario(NoListaUsuario*& inicio_lista, int id_remover);
Publicacao* RemoverNolistaPost(NoLista_de_Post*& inicio_lista,int idPost);

void UNFOLLOW(MiniRede& rede,int id_seguidor,int id_seguido,std :: ostream& saida);
void RemoverPublicacao(MiniRede& rede,int id_user,int id_post,std::ostream& saida);

void ComentarPublicacao(MiniRede& rede, int id_usuario, int id_post, const char comentario[], std::ostream& saida);
void ListarComentarios(MiniRede& rede, int id_post, std::ostream& saida);
void InserirNaListaComentarios(NoListaComentarios*& inicio_lista, int id_user, const char comment[]);
void liberarListadeComentarios(NoListaComentarios* inicio);

#endif
