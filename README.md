Trabalho final da cadeira de Estrutura de Dados[INF01203-B]: "MINI_REDE_ED"

Alunos: 
Matheus de Amador Tessmann (Matrícula: 00609594)
Rafael Droves Silveira     (Matrícula: 00609595)

Professor:
João Luiz Dihl Comba

Funções Principais e suas auxiliares:
LiberarMiniRede():

ProcessarComandos():

CadastrarUsuario():

buscarUsuarioPorId():

buscarUsuarioPorUsername():

listarUsuarios():

seguirUsuario():

listarSeguindo():

cadastrarPublicacao():

curtirpublicacao():

consultarNotificações(): Gerada pelo comando "GET_NOTIFICATIONS (user) (k)", irá mostrar as notificações de usuarios que o seguiram, likes e comentarios recebidos em seus posts.
Então iremos printar, de acordo com o tipo da notificação, detalhadamente quem seguiu, deu like e/ou tenha comentado no post.
Caso chamemos a função logo apos consultar, consultar novamente gerará na saida "USER_DON'T_HAVE_NOTIFICATIONS".
enfileirarNotificacao(): Chamada nas funçoes CurtirPublicacao(), SeguirUsuario() e ComentarPublicacao(). Ela vai receber como parametro quem recebeu a notificação, o tipo de notificação (sendo L para LIKES, F para FOLLOW e C para COMMENT), o id de quem realizou a ação, o id do post (apenas para LIKES e COMMENTS) e o comentario do post (Apenas para COMMENTS). 
Com isso, ira criar um novo nodo que receberá esses parametros e o alocara em uma fila de notificações.
DesenfileirarNotificacao(): Percorrerá a fila de notificações do usuario, liberando e deletando as notificações conforme elas sao lidas na função principal consultarNotificações().

gerarFeed(): Gerada pelo comando "FEED (user) (k)", irá mostrar os posts mais recentes (no caso, com os maiores timestamps) do usuario. Basicamente, iremos acessar a lista de posts de cada usuario seguido pelo nosso usuario, e iremos ordenar em um vetor os timestamps em ordem decrescente, utilizando insertion sort. Por fim, a função irá printar k posts no terminal.
TimestampSort(): Utilizada em GerarFeed(), é a responsável pela ordenação do vetor.
ComparaPost(): Auxiliar utilizada em timestampsort(), para comparar qual post tem o maior timestamp e, em caso de empate, decidimos que o escolhido será o post com o menor id.

listarTopPosts(): Gerada pelo comando "TOP_POSTS (k)", irá mostrar os posts com mais likes na rede, printando no final k posts no terminal.
CurtidasSort(): Utilizada em listarTopPosts(), é a função recursiva que ira percorrer a arvore de usuarios buscando todos os posts da rede e ordenando em um vetor, utilizando insertion sort.
ComparaCurtidas(): Auxiliar utilizada em curtidasSort, para comparar qual post tem maior quantidade de likes e, em caso de empate, decidimos que o escolhido será o post com o menor id.

ComentarPublicacao(): Gerada pelo comando "ADD_COMMENT (user_que_comentou)(id_post)(comment)", irá adicionar um comentario no respectivo post.
InserirNaListaComentarios(): auxiliar da ComentarPublicacao(), Cria um nodo que armazena o comentario e o id de quem comentou naquele post, alem de receber como parametro o inicio da lista de comentarios.

ListarComentarios(): Gerada pelo comando "ALL_POSTS (id_post)" serve para consultarmos os comentarios em um post especifico pois, se fossemos depender apenas da função de gerarnotificações para virmos os comentarios, nunca mais teriamos acesso àqueles comentarios, ja que as notificações sao deletadas apos consultarmos.

UNFOLLOW():

Remover Publicacao():