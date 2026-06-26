#include "functions.c"
#include <stdio.h>

int main()
{
    account user;
    int opcao;
    int tipoLogado;

    iniTable();
    loadUsers();

    tst *raiz = NULL;

    FILE *file;
    char palavra[40];

    file = fopen("ficheiros/dicionarioAuto.txt", "r");
    printf("yh\n");

    if(file)
    {
        while (fscanf(file, "%s", palavra) != EOF) 
        {
            raiz = insertWord(raiz, palavra, 0);
        }
        fclose(file);
    }
    printf(" %i \n", SearchWord(raiz, "arroz", 0));

    
    return 0;
}
