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

    int flag = 0;       // posizione della parola in array
    int idx_char = 0;   // posizione del char in ogni parola

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

// dato un array di array di char contenente una riga di dichiarazione variabile, mantiene solo la parte type
// ritorna la lunghezza della parte type
int get_type(char words[64][64], char type[64][64]) {

    int length = 0; // lunghezza della parte type
    for (int i=0; i < 63; i++) {
        if (!strcmp(words[i+1], "=") || !strcmp(words[i+1], ",") || !strcmp(words[i+1], ";")) {
            break;
        }
        strcat(type[i], words[i]);
        length++;
    }
    return length;

}

// dato un array di array di char contenente una riga di dichiarazione variabile, mantiene solo la parte name
void get_name(char words[64][64], char name[64][64], int start_idx) {

    int j = 0;  // indice per il vettore name
    for (int i = start_idx; i < 64; i++) {
        if (!strcmp(words[i], ";")) break;
        if (!strcmp(words[i], ",")) continue;
        if (!strcmp(words[i], "=")) {
            i++;
            continue;
        }
        strcat(name[j], words[i]);
        j++;
    }

}

// data una word, restituisce true se word è un tipo base
bool is_basic_type(char word[]) {

    char *basic_type[] = {"char", "int", "float", "double", "void", "_Bool", "bool"};
    for (int i=0; i < 7; i++) {
        if (!strcmp(basic_type[i], word)) return true;
    }
    return false;

}

// dato un array type, ritornare true se è un type
bool verify_type(char type[64][64]) {

    int signed_count = 0;
    int unsigned_count = 0;
    int long_count = 0;
    int short_count = 0;
    char basic_type[8];
    bool exist_basic_type = false;
    
    char current_word[64];
    for (int i=0; i < 64; i++) {
        strcpy(current_word, type[i]);
        if (!strcmp(current_word, "\0")) break;
        if (!strcmp(current_word, "const") || !strcmp(current_word, "volatile")) continue;
        if (!strcmp(current_word, "signed")) {
            signed_count++;
            continue;
        }
        if (!strcmp(current_word, "unsigned")) {
            unsigned_count++;
            continue;
        }
        if (!strcmp(current_word, "long")) {
            long_count++;
            continue;
        }
        if (!strcmp(current_word, "short")) {
            short_count++;
            continue;
        }
        if (is_basic_type(current_word)) {
            if (!exist_basic_type) {
                strcpy(basic_type, current_word);
                exist_basic_type = true;
                continue; 
            } else return false;
        } else return false;
    }

    if (!strcmp(basic_type, "char")) {
        if (signed_count > 1 || unsigned_count > 1 || (signed_count > 0 && unsigned_count > 0) ||
            long_count > 0 || short_count > 0) return false;
    } else if (!strcmp(basic_type, "int")) {
        if (signed_count > 1 || unsigned_count > 1 || (signed_count > 0 && unsigned_count > 0) ||
            long_count > 2 || short_count > 1 || (long_count > 0 && short_count > 0)) return false;
    } else if (!exist_basic_type) {
        if (signed_count > 0 || unsigned_count > 1 ||
            long_count > 2 || short_count > 1 || (long_count > 0 && short_count > 0)) return false;
    } else if (!strcmp(basic_type, "double")) {
        if (signed_count > 0 || unsigned_count > 0 || long_count > 1 || short_count > 0) return false;
    } else if (!strcmp(basic_type, "float") || !strcmp(basic_type, "void") || !strcmp(basic_type, "_Bool") || !strcmp(basic_type, "bool")) {
        if (signed_count > 0 || unsigned_count > 0 || long_count > 0 || short_count > 0) return false;
    }

    return true;

}

// dato un array name, ritornare true se sono tutti nomi validi
bool verify_name(char name[64][64]) {
    
    return true;

}









