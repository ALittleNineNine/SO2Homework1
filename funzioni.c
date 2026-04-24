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
   


    /*
    char first_word[64] = {0};
    char second_word[64] = {0};
    char third_word[64] = {0};
    int flag = 0;
    int idx_char = 0;

    for (int i=0; i < strlen(row); i++) {

        char current_char = row[i];
        if (current_char != ' ' && current_char != '\b' && current_char != '\n' && current_char != ';') {
            if (flag == 0) {
                first_word[idx_char] = current_char;
            } else if (flag == 1) {
                second_word[idx_char] = current_char;
            } else if (flag == 2) {
                third_word[idx_char] = current_char;
            }
            idx_char++;

            char next_char = row[i+1];
            if (next_char == ' ' || next_char == '\b' || next_char == '\n') {
                flag++;
                idx_char = 0;
            }
        }

    }

    printf("[%s, %s, %s]\n", first_word, second_word, third_word);
    */

}

