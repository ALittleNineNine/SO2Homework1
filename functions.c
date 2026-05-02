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
        if (current_char != ' ' && current_char != '\t' && current_char != '\n') {      // char da ignorare
            words[flag][idx_char] = current_char;
            idx_char++;

            char next_char = row[i+1];
            if (current_char < 48 || (current_char > 57 && current_char < 65) || (current_char > 90 && current_char < 95) ||
                (current_char > 95 && current_char < 97) || current_char > 122 ||
                next_char < 48 || (next_char > 57 && next_char < 65) || (next_char > 90 && next_char < 95) ||
                (next_char > 95 && next_char < 97) || next_char > 122) {
                flag++;
                idx_char = 0;
            }
        }
        if (flag > 64) break;

    }

}

/*
    data un array di array di char contenente una riga di dichiarazione variabile, lo mantiene solo la parte type
    ritorna la lunghezza della parte type
*/
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

// dato un array type, restituisce true se è un type
bool verify_type(char type[64][64]) {

    int signed_count = 0;
    int unsigned_count = 0;
    int long_count = 0;
    int short_count = 0;
    char basic_type[8];
    bool exist_basic_type = false;
    
    char current_word[64];
    /*
        questo ciclo for itera su tutte le parole in array type, ne estrae le seguenti informazioni:
        - quantità di modificatori: "signed", "unsigned", "long", "short";
        - il tipo di base (che può esistere uno solo o anche non esistere secondo standard);
        - un bool che indica se esiste il tipo di base.
        se il tipo di base non esiste, o che esiste più di una, restituisce direttamente false
    */
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

    // questa marea di if-else serve per verificare se le quantità di modificatore estratti precedentemente sono validi
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

// data una word, restituisce true se word è una keyword del linguaggio C
bool is_keyword(char word[]) {

    char *keywords[] = {"auto"  , "break" , "case"    , "char"  , "const"   , "continue", "default" , "do"    ,
                        "double", "else"  , "enum"    , "extern", "float"   , "for"     , "goto"    , "if"    ,
                        "int"   , "long"  , "register", "return", "short"   , "signed"  , "sizeof"  , "static",
                        "struct", "switch", "typedef" , "union" , "unsigned", "void"    , "volatile", "while"};
    for (int i=0; i < 32; i++) {
        if (!strcmp(keywords[i], word)) return true;
    }
    return false;

}

/* 
    dato un array name, restituisce true se sono tutti nomi validi
    il nome eventualmente non valido viene sostituito inplacemente con "!valid"
    quindi se la funzione restituisce false, non significa automaticamente che non ci siano nomi validi
*/
bool verify_name(char name[64][64]) {
    
    bool flag = true;   // false se almeno un nome non è valido

    char current_word[64];
    // itera su tutte le parole in array name, se è una keyword, restituisce false
    for (int i=0; i < 64; i++) {
        strcpy(current_word, name[i]);
        if (!strcmp(current_word, "\0")) break;
        if (is_keyword(current_word)) {
            strcpy(name[i], "!valid");  // la stringa "!valid" indica nome non valido, da ignorare quando si memorizzerà
            flag = false;
        }

        char current_char;
        // itera su tutti i char di ogni nome e restituisce false se il nome non è valido
        for (int j=0; j < 64; j++) {
            current_char = current_word[j];
            if (current_char == '\0') break;
            if (j == 0 && current_char >= 48 && current_char <= 57) {
                strcpy(name[i], "!valid");
                flag = false;
            }
            if (current_char < 48 || (current_char > 57 && current_char < 65) || (current_char > 90 && current_char < 95) ||
                (current_char > 95 && current_char < 97) || current_char > 122) {
                strcpy(name[i], "!valid");
                flag = false;
               }
        }

    }

    return flag;

}

// data una lista concatenata contenenti varaibili e un nome, restituisce true se il nome appartiene alla lista
bool existing_var(variable *variables, char name[]) {

    variable *current_var = variables;
    while (current_var != NULL) {
        if (!strcmp(current_var->name, name)) {
            return true;
        }
        current_var = current_var->next;
    }
    return false;

}

// trasforma un array in una stringa inplacemente
void array_to_string(char array[64][64], char string[]) {
    for (int i=0; i < 64; i++) {
        if (!strcmp(array[i], "\0")) break;
        if (i != 0) strcat(string, " ");
        strcat(string, array[i]);
    }
}

// calcola il numero di celle nelle due liste variables e errors
void linked_list_count(int *var_err_count, variable *variables, error *errors) {

    variable *current_var = variables;
    while (current_var != NULL) {
        var_err_count[0]++;
        current_var = current_var->next;
    }

    error *current_err = errors;
    while (current_err != NULL) {
        var_err_count[1]++;
        current_err = current_err->next;
    }

}

// ritorna true se in questa riga words contiene main
bool is_main(char words[64][64]) {
    for (int i=0; i < 64; i++) {
        if (!strcmp(words[i], "\0")) break;
        if (!strcmp(words[i], "main")) return true;
    }
    return false;
}

// data la prima word di una riga, restituisce true se è finita la parte di dichiarazione variabile
bool end_variable_declaration(char word[]) {

    char *keywords[] = {"const", "volatile", "restrict", "signed", "unsigned", "long", "short",
                        "char", "int", "double", "float", "void", "_Bool", "bool"};
    for (int i=0; i < 14; i++) {
        if (!strcmp(keywords[i], word)) return false;
    }
    return true;

} // !!! DA RIVEDERE (TIPI ERRONEI NON ESISTENTI FANNO FINIRE DIRETTAMENTE LA PARTE DICHIARAZIONE VARIABILE) !!!









