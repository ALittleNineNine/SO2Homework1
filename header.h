#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// struct lista concatenata per memorizzare informazioni sulle variabili
typedef struct variabile {
    char tipo[64];
    char nome[64];
    bool utilizzato;
    int riga;
    struct variabile *next;
} variabile;

// crea un nuovo nodo variabile e lo collega in testa alla lista variabili
variabile *add_var(variabile *next_var, char tipo[], char nome[], int riga);

