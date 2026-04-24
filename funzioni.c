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

// data una riga di codice, li spezza in al massimo in 7 parole
void analyze_row(char row[], char words[7][64]) {

    int flag = 0;
    int idx_char = 0;

    for (int i=0; i < strlen(row); i++) {

        char current_char = row[i];
        if (current_char != ' ' && current_char != '\t' && current_char != '\n') {
            words[flag][idx_char] = current_char;
            idx_char++;

            char next_char = row[i+1];
            if (next_char == ' ' || next_char == '\t' || next_char == '\n' ||
                next_char == ';' || next_char == '=' || current_char == '=') {
                flag++;
                idx_char = 0;
            }
        }
        if (flag > 7) break;

    }

}

