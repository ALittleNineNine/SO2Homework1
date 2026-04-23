#include "header.h"

int main(int argc, char *argv[]) {

    char tipo[] = "TipoNineNine";
    char nome[] = "NomeNineNine";
    int riga = 1;
    variabile *testa = inserisci_var(NULL, tipo, nome, riga);
    printf("Tipo: %s, Nome: %s, È utilizzato: %d, Riga: %d\n", testa->tipo, testa->nome, testa->utilizzato, testa->riga);
    return 0;

}