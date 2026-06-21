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
    NoFilaNotificacoes* prox;
    };
struct Publicacao
{
    int id_da_publicacao;
    int autor_id;
    int timestamp;
    char texto_da_publicacao[TAM_TEXTO];
    int qtd_likes;
    NoListaUsuario* curtidas;//LISTA DE IDS QUEM CURTIU O POST
};struct NoLista_de_Post
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


// TODO: definir as structs principais do trabalho.
//
// Sugestao de structs que provavelmente serao necessarias:
// - Usuario
// - Publicacao
// - MiniRede
// - nos para lista encadeada
// - nos para arvore binaria de usuarios por id
// - nos para tabela hash de usernames
// - nos para fila de notificacoes
//
// Os campos de cada struct fazem parte do projeto dos alunos.

struct MiniRede {

    NoArvoreUsuarios* raiz_id; //USO No comando add_user,find_user e list_user
    NoHashUsuario* tabela_usernames[TAM_HASH]; // USO PARA INDEXAR OS USUARIOS SOBRE A STRING DO USERNAME
    NoLista_de_Post* todos_os_posts;

    // TODO: declarar aqui os ponteiros/estruturas principais da rede.
    //
    // Exemplos de responsabilidades:
    // - usuarios armazenados por id
    // - usuarios acessiveis por username
    // - publicacoes cadastradas
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


// TODO: declarar aqui as funcoes auxiliares escolhidas pelo grupo.
//
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
void enfileirarNotificacao(Usuario* usuario_recebe,char tipo, int id_de_origem,int id_do_post);
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
// Exemplos de responsabilidades auxiliares:
// - buscar usuario por id
// - buscar usuario por username
// - buscar publicacao por id
// - inserir/listar/liberar arvore
// - inserir/buscar/liberar tabela hash
// - enfileirar/desenfileirar notificacoes
// - manipular listas encadeadas
// - ordenar vetores auxiliares para feed e ranking

#endif
