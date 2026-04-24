#include "header.h"

int main(int argc, char *argv[]) {

    char current_row[256];
    char new_line[] = "\n";

    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Errore apertura file.\n");
        exit(1);
    }


    while (!feof(fp)) {
        fgets(current_row, sizeof(current_row), fp);
        if (strcmp(current_row, new_line)) {
            char words[7][64] = {0};
            analyze_row(current_row, words);

            for (int i=0; i<7; i++) {
                printf("%s ", words[i]);
            }
            printf("\n");

            // printf("%s", current_row);
        }
    }

    fclose(fp);



    /*
    printf("Numero argomenti: %d\nContenuto argomenti: ", argc);
    for (int i=1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
    */

    /*
    char tipo[] = "TipoNineNine";
    char nome[] = "NomeNineNine";
    int riga = 1;
    variabile *testa = add_var(NULL, tipo, nome, riga);
    printf("Tipo: %s, Nome: %s, È utilizzato: %d, Riga: %d\n", testa->tipo, testa->nome, testa->utilizzato, testa->riga);
    */

    return 0;

}