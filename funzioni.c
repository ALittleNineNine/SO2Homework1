#include "header.h"

// crea un nuovo nodo variabile e lo collega in testa alla lista variabili
variabile *add_var(variabile *next_var, char tipo[], char nome[], int riga) {
    variabile *new_var = malloc(sizeof(variabile));
    strcpy(new_var->tipo, tipo);
    strcpy(new_var->nome, nome);
    new_var->utilizzato = false;
    new_var->riga = riga;
    new_var->next = next_var;
    return new_var;
}

