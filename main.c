#include "header.h"

int main(int argc, char *argv[]) {

    char riga_attuale[512];

    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Errore apertura file.\n");
        exit(1);
    }

    char new_line[] = "\n";

    while (!feof(fp)) {
        fgets(riga_attuale, sizeof(riga_attuale), fp);
        if (strcmp(riga_attuale, new_line)) {
        printf("%s", riga_attuale);
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