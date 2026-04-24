#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// struct lista concatenata per memorizzare informazioni sulle variabili
typedef struct variable {
    char type[64];
    char name[64];
    bool used;
    int row;
    struct variable *next;
} variable;

// crea un nuovo nodo variabile e lo collega in testa alla lista variabili
variable *add_var(variable *next_var, char type[], char name[], int row);

void analyze_row(char row[], char words[7][64]);

