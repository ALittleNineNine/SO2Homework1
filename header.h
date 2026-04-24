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

typedef struct error {
    bool wrong_type;
    bool wrong_name;
    int row;
    struct error *next;
} error;

// tipi base
// const char *basic_type[] = {"char", "int", "float", "double", "void", "_Bool", "bool"};

// crea un nuovo nodo variabile e lo collega in testa alla lista variabili
variable *add_var(variable *next_var, char type[], char name[], int row);

// crea un nuovo nodo errore e lo collega in testa alla lista errori
error *add_error(error *next_err, int row);

// data una riga di codice, li spezza in al massimo in 64 parole
void analyze_row(char row[], char words[64][64]);

// data un array di array di char contenente una riga di dichiarazione variabile, lo mantiene solo la parte type
void get_type(char words[64][64], char type[64][64]);

