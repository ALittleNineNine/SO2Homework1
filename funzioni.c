#include "header.h"

// crea un nuovo nodo variabile e lo collega in testa alla lista variabili
variable *add_var(variable *next_var, char type[], char name[], int row) {

    variable *new_var = malloc(sizeof(variable));
    strcpy(new_var->type, type);
    strcpy(new_var->name, name);
    new_var->used = false;
    new_var->row = row;
    new_var->next = next_var;
    return new_var;

}

// crea un nuovo nodo errore e lo collega in testa alla lista errori
error *add_error(error *next_err, int row) {

    error *new_err = malloc(sizeof(error));
    new_err->wrong_type = false;
    new_err->wrong_name = false;
    new_err->row = row;
    new_err->next = next_err;
    return new_err;

}

// data una riga di codice, li spezza in al massimo in 64 parole
void analyze_row(char row[], char words[64][64]) {

    int flag = 0;
    int idx_char = 0;

    for (int i=0; i < strlen(row); i++) {

        char current_char = row[i];
        if (current_char != ' ' && current_char != '\t' && current_char != '\n') {
            words[flag][idx_char] = current_char;
            idx_char++;

            char next_char = row[i+1];
            if (next_char == ' ' || next_char == '\t' || next_char == '\n' || next_char == ';' ||
                next_char == ',' || current_char == ',' || next_char == '=' || current_char == '=') {
                flag++;
                idx_char = 0;
            }
        }
        if (flag > 64) break;

    }

}

// data un array di array di char contenente una riga di dichiarazione variabile, mantiene solo la parte type
void get_type(char words[64][64], char type[64][64]) {

    for (int i=0; i < 63; i++) {
        if (!strcmp(words[i+1], "=") || !strcmp(words[i+1], ",") || !strcmp(words[i+1], ";")) {
            break;
        }
        strcat(type[i], words[i]);
    }
}


