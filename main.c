#include "functions.c"
#include <stdio.h>
#include <string.h>

#define Userfree 0
#define Userpremium 1

int main()
{    
    account user;
    int opcao;
    int tipoLogado;
    char usernameLogado[30];
    char username[30];
    tst *raiz = NULL;

    iniTable();
    loadUsers();
    raiz = doingSum(raiz);
    
    do
    {
        system("cls");
        printf("\n===== MENU =====\n");
        printf("1 - Criar usuario\n");
        printf("2 - Login\n");
        // printf("3 - Procurar usuario\n");
        // printf("4 - Mostrar tabela\n");
        // printf("5 - Salvar usuarios\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
                printf("Username: ");
                scanf("%29s", user.username);
                printf("Password: ");
                scanf("%19s", user.password);
                printf("Tipo (0-Free, 1-Premium): ");
                scanf("%d", &user.type);

                if (searchUser(user.username) != -1)
                {
                    printf("Erro: usuario ja existe\n");
                }
                else
                {
                    insertUser(user);
                    saveUser();
                    printf("Usuario criado com sucesso\n");
                }
                system("pause");
                break;

            case 2:
            {
                tipoLogado = login(usernameLogado);
            
                
                if (tipoLogado == -1)
                {
                    printf("Login falhou\n");
                }
                else
                {
                    printf("Login ok. Tipo de conta: %d\n", tipoLogado);
                    menuDicionario(raiz, table[tipoLogado].conta.username, tipoLogado);
                }
                system("pause");
                break;
            }

            case 3:
            {
                char usernameBusca[30];
                printf("Username a procurar: ");
                scanf("%29s", usernameBusca);
                int pos = searchUser(usernameBusca);
                if (pos == -1)
                {
                    printf("Usuario nao encontrado\n");
                }
                else
                {
                    printf("Encontrado na posicao %d\n", pos);
                }
                system("pause");
                break;
            }

            case 4:
                mostrarTabela();
                system("pause");
                break;

            case 5:
                saveUser();
                printf("Usuarios salvos em arquivo\n");
                system("pause");
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida\n");
            system("pause");
        }

    } while (opcao != 0);

    return 0;
}
