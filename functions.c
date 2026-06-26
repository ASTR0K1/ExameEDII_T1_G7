#include "functions.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define tam 20
#define Userfree 0
#define Userpremium 1


struct trie
{
    char letra;
    struct trie *left, *center, *right;
    bool isKey;
};

struct user
{
	char username[30];
	char password[20];
	int type;  // 0- Conta free, 1- Conta premium
};

struct slot
{
	account conta;
	int close; // 0- para Free, 1- para busy
}; // tipo para armazenar conta e estado na tabela

slot table[20]; // tamanho da tabela hash de usuarios

tst *createNote(char letra)
{
    tst *newNode = (tst*) malloc(sizeof(tst));

    if (!newNode) 
        return NULL;
    
    newNode->letra = letra;
    newNode->left = NULL;
    newNode->center = NULL;
    newNode->right = NULL;
    newNode->isKey = false;

    return newNode;

}

tst *insertWord(tst *trie, char *palavra, int posicao)  // arvore, palavra que vamos inserir, e posição que é sempre 0)
{
    char letra = palavra[posicao];

    if(letra == '\0') return trie; //caso base nengues

    if(!trie)
    {
        trie = createNote(letra);
    }

    if(letra > trie->letra)
    {
        trie->right = insertWord(trie->right, palavra, posicao);
    }
    else if(letra < trie->letra)
    {
        trie->left = insertWord(trie->left, palavra, posicao);
    }
    else
    {
        if( palavra[posicao+1] == '\0')
        {
            trie->isKey = true;
        }
        else
        {
            trie->center = insertWord(trie->center, palavra, (posicao + 1) );
        }
    }
    
    return trie;
}

bool SearchWord(tst *trie, char *palavra, int posicao) // arvore, palavra que vamos procurar, e posição que é sempre 0)
{
 
    char letra = palavra[posicao];

    if(!trie)
        return false;
    if(letra == '\0')
        return false;
    
    if(letra > trie->letra)
    {
        return SearchWord(trie->right, palavra, posicao);
    }
    else if(letra < trie->letra)
    {
         return SearchWord(trie->left, palavra, posicao);
    }
    else
    {
        if( palavra[posicao+1] == '\0')
        {
            return trie->isKey;
        }
        else
        {
            return SearchWord(trie->center, palavra, (posicao + 1) );
        }
    }   
}


////


int funHash(char *username)
{
	int soma = 0;
	int i;
	
	for(i=0; username[i]!= '\0'; i++)
	{
		soma += username[i];
	}
	return soma % tam;
}

void iniTable()
{
	int i;
	
	for(i=0; i<tam; i++)
	{
		table[i].close= 0; //inicializei a tabela e preenchi tudo como free
	}
}

void insertUser(account user)
{
	int position = funHash(user.username);
	
	int tentativas = 0;

	while(table[position].close == 1 && tentativas < tam)
	{
		position = (position + 1) % tam;
		tentativas++;
	}

	if (tentativas == tam)
	{
		printf("Erro: tabela cheia, nao foi possivel inserir o usuario\n");
		return;
	}

		table[position].conta = user;
		table[position].close = 1;
}

int searchUser(char *username)
{
    int position = funHash(username);
    int tentativas = 0;

    while (tentativas < tam)
    {
        if (table[position].close == 0)
        {
            // slot vazio, nao existe user na tabela
            return -1;
        }
        else if (strcmp(table[position].conta.username, username) == 0)
        {
    
            return position;
        }
        else
        {
            //sondagem linear, caso no slot tiver um user diferente do que encontramos
            
            position = (position + 1) % tam;
            tentativas++;
        }
    }

    // user nao existe, o slot existe, mas o user nao
    return -1;
}

void createUser()
{
    account newUser;
    int typeAccount;

    printf("Digite o username: ");
    scanf("%29s", newUser.username);

    printf("Digite a password: ");
    scanf("%19s", newUser.password);

    if (searchUser(newUser.username) != -1) 
    {
        printf("Erro: usuario ja existe\n");
        return;
    }

    do
    {
        printf("Tipo de conta (0-Free, 1-Premium): ");
        scanf("%d", &typeAccount);
    } while (typeAccount != 0 && typeAccount != 1);

    newUser.type = typeAccount;

    insertUser(newUser);
    saveUser();
    printf("Usuario criado com sucesso\n");
}

int login(char *usernameOut)
{
    char usernameDigitado[30];
    char passwordDigitada[20];
    int position;

    printf("Digite o username: ");
    scanf("%29s", usernameDigitado);

    position = searchUser(usernameDigitado);

    if (position == -1)
    {
        printf("Erro: usuario nao encontrado\n");
        return -1;
    }

    printf("Digite a password: ");
    scanf("%19s", passwordDigitada);

    if (strcmp(table[position].conta.password, passwordDigitada) == 0)
    {
        printf("Login bem-sucedido\n");
        return table[position].conta.type;
    }
    else
    {
        printf("Erro: senha incorreta\n");
        return -1;
    }
}

void saveUser()
{
    FILE *arquivo;
    int i;

    arquivo = fopen("ficheiros/usuarios.txt", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para salvar\n");
        return;
    }

    for (i = 0; i < tam; i++)
    {
        if (table[i].close == 1)
        {
            fprintf(arquivo, "%s %s %d\n", table[i].conta.username, table[i].conta.password, table[i].conta.type);
        }
    }

    fclose(arquivo);
}

void loadUsers()
{
    FILE *arquivo;
    account temp;

    arquivo = fopen("ficheiros/usuarios.txt", "r");

    if (arquivo == NULL)
    {
        // O arquivo nao existe
        return;
    }

    while (fscanf(arquivo, "%s %s %d", temp.username, temp.password, &temp.type) == 3)
    {
        insertUser(temp);
    }

    fclose(arquivo);
}

// -----------------------------------------------------------------------------------
// funcao para utilizar em opcoes do menu que sao apenas para usuarios premium

int temPermissao(int tipoConta, int funcionalidade)
{
    if (funcionalidade == Userpremium && tipoConta == Userfree)
    {
        return 0; // sem permissao
    }

    return 1; // tem permissao
}

//-------------------------------------------------------------------------------------
void mostrarTabela()
{
    int i;

    printf("\n===== TABELA HASH =====\n");

    for (i = 0; i < tam; i++)
    {
        if (table[i].close == 1)
        {
            printf("[%d] %s (%s) Tipo: %d\n",
                   i,
                   table[i].conta.username,
                   table[i].conta.password,
                   table[i].conta.type);
        }
        else
        {
            printf("[%d] Vazio\n", i);
        }
    }
}


void menuDicionario(tst *raiz, char *username, int tipoConta)
{
    int opcao;
    char palavra[100];

    do
    {
        system("pause");
        system("cls");
        printf("\n===== DICIONARIO=====\n");
        printf("Nome: %s\n", username);
        printf("1 - Pesquisar palavra\n");
        printf("2 - Pesquisar por prefixo\n");
        printf("3 - Listar todas as palavras\n");
        printf("4 - Adicionar aos favoritos (Premium)\n");
        printf("5 - Listar favoritos (Premium)\n");
        printf("6 - Remover favorito (Premium)\n");
        printf("0 - Logout\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
            {
                printf("Palavra a pesquisar: ");
                scanf("%99s", palavra);

                if (SearchWord(raiz, palavra, 0))
                {
                    printf("'%s' encontrada no dicionario.\n", palavra);
                }
                else
                {
                    printf("'%s' nao encontrada.\n", palavra);
                }

                guardarHistorico(username, palavra);
                break;
            }

            case 2:
            {

                printf("Prefixo a pesquisar: ");
                scanf("%99s", palavra);
                pesquisarPorPrefixo(raiz, palavra);
                guardarHistorico(username, palavra);
                break;   
            }

            case 3:
            {
                char buffer[100];
                listarPalavras(raiz, buffer, 0);
                break;
            }

            case 4:
            {
                do
                {
                    printf("Palavra a adicionar aos favoritos: ");
                    scanf("%99s", palavra);
                    
                    if(SearchWord(raiz, palavra, 0) == 0)
                    {
                        printf("ERRO: Palavra desejada nao existe no dicionario\n tente novamente");
                    }
                    
                } while( SearchWord(raiz, palavra, 0) == 0);
                
                
                if (!temPermissao(tipoConta, Userpremium))
                {
                    printf("Funcionalidade exclusiva para contas Premium.\n");
                    break;
                }
                adicionarFavorito(username, palavra);
                printf("Adicionado aos favoritos.\n");
                break;
            }

            case 5:
            { 
                if (!temPermissao(tipoConta, Userpremium))
                {
                    printf("Funcionalidade exclusiva para contas Premium.\n");
                    break;
                }
                listarFavoritos(username);
                break;
            }

            case 6:
            {
                do
                {
                    printf("Palavra a remover dos favoritos: ");
                    scanf("%99s", palavra);
                    
                    if(SearchWord(raiz, palavra, 0) == 0)
                    {
                        printf("ERRO: Palavra desejada nao existe no dicionario\n tente novamente");
                    }
                    
                } while( SearchWord(raiz, palavra, 0) == 0);
                
                
                if (!temPermissao(tipoConta, Userpremium))
                {
                    printf("Funcionalidade exclusiva para contas Premium.\n");
                    break;
                }
                removerFavorito(username, palavra);
                printf("Removido (se existia).\n");
                break;
            }

            case 0:
            {
                printf("A fazer logout...\n");
                break;
            }

            default:
                printf("Opcao invalida\n");
        }

    } while (opcao != 0);
}

////

void listarPalavras(tst *raiz, char *palavraAtual, int nivel) //nivel vai guardar a posi��o do buffer aonde vamos colocar a proxima letra
{
    if (raiz == NULL)
        return;

    listarPalavras(raiz->left, palavraAtual, nivel); // primeiro vamos pelo lado dos caracteres menores

    palavraAtual[nivel] = raiz->letra;

    if (raiz->isKey)
    {
        palavraAtual[nivel + 1] = '\0'; //indicar o final da palvra
        printf("%s\n", palavraAtual);
    }

    listarPalavras(raiz->center, palavraAtual, nivel + 1);// vai para o centro para a mesma palavra 

    listarPalavras(raiz->right, palavraAtual, nivel);// vai buscar uma letra maior no lado direito
}

void pesquisarPorPrefixo(tst *raiz, char *prefixo)
{
    int i = 0;
    char buffer[100];

    while (raiz != NULL)
    {
        if (prefixo[i] < raiz->letra)
        {
            raiz = raiz->left;
        }
        else if (prefixo[i] > raiz->letra)
        {
            raiz = raiz->right;
        }
        else
        {
            buffer[i] = raiz->letra;

            if (prefixo[i + 1] == '\0')
            {
                buffer[i + 1] = '\0';

                if (raiz->isKey)
                {
                    printf("%s\n", buffer);
                }

                listarPalavras(raiz->center, buffer, i + 1);
                return;
            }

            i++;
            raiz = raiz->center;
        }
    }

    printf("Prefixo nao encontrado.\n");
}

void guardarHistorico(char *utilizador, char *pesquisa) //AQUI QUEM TIVER A BATER O MENU S� PRECISA COLOCAR NOS CASES QUE ENVOLVEM PESQUISA EXACTA E PESQUISA POR PREFIXO E PASSAR CLARAMENTE OS PAR�METROS  
{
    FILE *ficheiro;

    ficheiro = fopen("ficheiros/historico.txt", "a");

    if(ficheiro == NULL)
    {
        printf("Erro ao abrir ficheiro");
        return;
    }

    fprintf(ficheiro, "%s;%s\n", utilizador, pesquisa);

    fclose(ficheiro);
}

void adicionarFavorito(char *utilizador, char *palavra) //O MESMO PARA ESTA
{
    FILE *ficheiro;

    ficheiro = fopen("ficheiros/favoritos.txt", "a");

    if (ficheiro == NULL)
    {
        printf("Erro ao abrir ficheiro.\n");
        return;
    }

    fprintf(ficheiro, "%s;%s\n", utilizador, palavra);

    fclose(ficheiro);
}

void listarFavoritos(char *utilizador)
{
    FILE *ficheiro;
    char nome[50];
    char palavra[100];

    ficheiro = fopen("ficheiros/favoritos.txt", "r");

    if (ficheiro == NULL)
    {
        printf("Nenhum favorito encontrado.\n");
        return;
    }

    printf("Favoritos de %s:\n", utilizador);

    while (fscanf(ficheiro, "%49[^;];%99[^\n]\n", nome, palavra) == 2)
    {
        if (strcmp(nome, utilizador) == 0)
        {
            printf("%s\n", palavra);
        }
    }

    fclose(ficheiro);
}

void removerFavorito(char *utilizador, char *palavraRemover)
{
    FILE *ficheiro;
    FILE *temporario;

    char nome[50];
    char palavra[100];

    ficheiro = fopen("ficheiros/favoritos.txt", "r");

    if (ficheiro == NULL)
    {
        printf("Erro ao abrir ficheiro.\n");
        return;
    }

    temporario = fopen("ficheiros/temp.txt", "w");

    if (temporario == NULL)
    {
        fclose(ficheiro);
        printf("Erro ao criar ficheiro tempor�rio.\n");
        return;
    }

    while (fscanf(ficheiro, "%49[^;];%99[^\n]\n", nome, palavra) == 2)
    {
        if (!(strcmp(nome, utilizador) == 0 && strcmp(palavra, palavraRemover) == 0)){
            fprintf(temporario, "%s;%s\n", nome, palavra);
        }
    }
    
     fclose(ficheiro);
    fclose(temporario);

    remove("ficheiros/favoritos.txt");
    rename("ficheiros/temp.txt", "ficheiros/favoritos.txt");
}

//

//////////////////////////////////////////////////////////////////////////////

tst *doingSum(tst* trie)
{
    FILE * file = fopen("ficheiros/dicionarioAuto.txt", "r");
    char palavra[40];

    if(!file){
        printf("Erro: Ficheiro ficheiros/dicionarioAuto.txt não encontra");
        return trie;
    }

    if(file)
    {
        while (fscanf(file, "%s", palavra) != EOF) 
        {
            trie = insertWord(trie, palavra, 0);
        }

        fclose(file);
    }
    
    return trie;
}

///////////////////////////////////////////////////////////////////



