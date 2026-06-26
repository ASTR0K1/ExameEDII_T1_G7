#include <stdbool.h>

typedef struct trie tst;
tst *createNote(char letra);
tst *insertWord(tst *trie, char *palavra, int posicao); //Free
bool SearchWord(tst *trie, char *palavra, int posicao); //Free
tst *carregarDicionario(tst *raiz, char *nomeFicheiro);


typedef struct user account;
typedef struct slot slot;
int funHash(char *username);
void iniTable();
void insertUser(account user);
int searchUser(char *username);
void createUser();
int login(char *usernameOut);
void saveUser();
void loadUsers();
int temPermissao(int tipoConta, int funcionalidade);
void mostrarTabela();
void menuDicionario(tst *raiz, char *username, int tipoConta);


void listarPalavras(tst *raiz, char *palavraAtual, int nivel); //Free
void pesquisarPorPrefixo(tst *raiz, char *prefixo); //Free
void guardarHistorico(char *utilizador, char *pesquisa); //Free
void adicionarFavorito(char *utilizador, char *palavra); //Premium
void listarFavoritos(char *utilizador); //Premium 
void listarHistorico(char *utilizador); //Free
void removerFavorito(char *utilizador, char *palavraRemover); //Premium

tst *doingSum(tst* trie);


