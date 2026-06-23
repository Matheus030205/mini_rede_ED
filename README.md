Trabalho final da cadeira de Estrutura de Dados[INF01203-B]: "MINI_REDE_ED"

Alunos: 
Matheus de Amador Tessmann (Matrícula: 00609594)
Rafael Droves Silveira     (Matrícula: 00609595)

Professor:
João Luiz Dihl Comba

Funções Principais e suas auxiliares:
LiberarMiniRede():LiberarMiniRede(): Responsável pela desalocação completa de memória de todo o ecossistema da rede social antes do encerramento do programa. Ela realiza o caminhamento pós-ordem na árvore binária para destruir cada nó de usuário e invoca as funções auxiliares de limpeza para varrer e desalocar todas as listas encadeadas globais e locais.
ProcessarComandos():Atua como o núcleo de controle (loop principal) da aplicação. Ela lê continuamente o fluxo de dados do arquivo de entrada linha por linha, identifica o mnemônico (comando) digitado e faz o parsing (extração) dos parâmetros necessários. Com base no comando lido, ela direciona a execução para a respectiva função de negócio e gerencia erros de comandos inválidos ou corrompidos no buffer de entrada.

CadastrarUsuario():Gerada pelo comando "ADD_USER (username) (id)". É responsável por inserir um novo perfil na plataforma. A função verifica se o ID ou o Username já estão em uso (garantindo a unicidade) e, caso estejam livres, aloca dinamicamente a estrutura Usuario e a insere na árvore binária de busca ordenada por ID.

buscarUsuarioPorId():Função de busca que realiza o caminhamento de pesquisa binária na árvore raiz_id da rede social. Devido à propriedade de árvore binária de busca (BST).

buscarUsuarioPorUsername():Como a árvore principal é ordenada por ID, esta função realiza uma busca por texto para localizar um perfil com base no seu apelido (username). Ela percorre os nós da árvore comparando as strings até encontrar o usuário correspondente ou retornar nullptr.

listarUsuarios():Ativada pelo comando "SHOW_USERS". Realiza o percurso em-ordem (in-order traversal) na árvore binária de busca de usuários. Isso garante que todos os perfis cadastrados na rede sejam exibidos no arquivo de saída ordenados de forma crescente com base em seus IDs numéricos.

seguirUsuario():Gerada pelo comando "FOLLOW (id_seguidor) (id_seguido)". Estabelece uma relação de conexões orientada entre dois perfis. A função valida a existência de ambos os usuários na árvore, certifica-se de que a relação já não exista e, em seguida, insere o ID do usuário alvo no início da lista encadeada de seguidos do perfil de origem. Dispara também uma notificação do tipo 'F' para o usuário que ganhou o seguidor.

listarSeguindo():Gerada pelo comando "SHOW_FOLLOWING (id_usuario)". Localiza o usuário alvo na árvore e percorre de forma linear a sua lista encadeada interna de seguidos, imprimindo sequencialmente na saída todos os IDs dos perfis que aquele usuário específico acompanha na rede.

cadastrarPublicacao():Gerada pelo comando "ADD_POST (id_autor) (id_post) (timestamp) (conteúdo_texto)". Aloca dinamicamente uma nova estrutura Publicacao preenchendo seus metadados. Em seguida, ela vincula essa publicação na lista encadeada pessoal de postagens do autor e também na lista encadeada global de histórico da MiniRede.

curtirpublicacao():Gerada pelo comando "LIKE (id_quem_curtiu) (id_post)". Incrementa o contador de relevância de uma postagem. A função localiza o post (seja buscando linearmente na lista global ou na sublista do autor), verifica se o usuário já não curtiu aquela publicação anteriormente e insere o ID do utilizador na lista encadeada de curtidas do post. Dispara uma notificação do tipo 'L' na fila do autor da publicação.

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

UNFOLLOW():Gerada pelo comando "UNFOLLOW (id_seguidor) (id_seguido)", serve para cancelar o vínculo de acompanhamento entre dois usuários. A função busca ambos os perfis na árvore binária, valida se o usuário de origem realmente segue o usuário alvo e, em caso positivo, remove o registro correspondente.

Remover Publicacao():Gerada pelo comando "REMOVE_POST (id_usuario) (id_post)", é responsável por deletar uma publicação existente na rede. A função localiza o autor na árvore e remove o post tanto da lista cronológica interna do próprio usuário quanto da lista global da rede. Por fim, limpa o histórico de curtidas associado à publicação e desaloca sua estrutura física da memória.