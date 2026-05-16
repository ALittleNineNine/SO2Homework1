#include "header.h"

/* ____________________NineNine____________________ */

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
void analyze_row(char *row, char **words) {

    int flag = 0;       // posizione della parola in array
    int idx_char = 0;   // posizione del char in ogni parola

    bool in_string = false;     // true se siamo in una stringa
    bool in_char = false;     // true se siamo in un char

    for (int i=0; row[i] != '\0'; i++) {

        char current_char = row[i];
        if (current_char == '"') in_string = !in_string;    // gestione alternata quando si incontra "
        if (in_string) {
            words[flag][idx_char] = current_char;
            idx_char++;
            continue;
        }
        if (current_char == '\'') in_char = !in_char;       // gestione alternata quando si incontra '
        if (in_char) {
            words[flag][idx_char] = current_char;
            idx_char++;
            continue;
        }

        if (current_char != ' ' && current_char != '\t' && current_char != '\n') {      // char da ignorare
            words[flag][idx_char] = current_char;
            idx_char++;

            char next_char = row[i+1];
            if (current_char < 48 || (current_char > 57 && current_char < 65) || (current_char > 90 && current_char < 95) ||
                (current_char > 95 && current_char < 97) || current_char > 122 ||
                next_char < 48 || (next_char > 57 && next_char < 65) || (next_char > 90 && next_char < 95) ||
                (next_char > 95 && next_char < 97) || next_char > 122 || next_char == '\0') {
                
                words[flag][idx_char] = '\0';
                flag++;
                idx_char = 0;

            }
        }
        if (flag >= 128) break;

    }

}

// crea un nuovo nodo newtype e lo collega in testa alla lista newtype (riguardante typedef senza struct)
newtype *add_newtype_no_struct(newtype *newtypes, char **words) {

    int idx_type = 0;
    while (words[idx_type][0] != '\0') idx_type++;

    newtype *new_type = malloc(sizeof(newtype));
    strcpy(new_type->type, words[idx_type - 2]);
    new_type->next = newtypes;
    return new_type;

}

// crea un nuovo nodo newtype e lo collega in testa alla lista newtype (riguardante typedef con struct)
newtype *add_newtype_struct(newtype *newtypes, char **words, int idx) {

    newtype *new_type = malloc(sizeof(newtype));
    strcpy(new_type->type, words[idx + 1]);
    new_type->next = newtypes;
    return new_type;

}

// data una word, restituisce true se word è un tipo creato con typedef
bool is_newtype(char word[], newtype *newtypes) {

    while (newtypes != NULL) {
        if (!strcmp(newtypes->type, word)) return true;
        newtypes = newtypes->next;
    }
    return false;

}

/*
    data un array di array di char contenente una riga di dichiarazione variabile, lo mantiene solo la parte type
    ritorna la lunghezza della parte type
*/
int get_type(char **words, char **type) {

    int length = 0; // lunghezza della parte type
    for (int i=0; i < 127; i++) {
        if (!strcmp(words[i+1], "=") || !strcmp(words[i+1], ",") || !strcmp(words[i+1], ";")) {
            break;
        }
        strcat(type[i], words[i]);
        length++;
    }
    return length;

}

// dato un array di array di char contenente una riga di dichiarazione variabile, mantiene solo la parte name
void get_name(char **words, char **name, int start_idx) {

    int j = 0;  // indice per il vettore name
    for (int i = start_idx; i < 128; i++) {
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

// aggiungere la/le variabile/i se non ci sono errori, ritorna la nuova testa della lista
variable *variables_management(variable *variables, newtype *newtypes, char **type, char **name, int row, bool *flag) {

    char current_type[512] = {0};
    array_to_string(type, current_type);
    char current_name[128] = {0};

    for (int i=0; i < 128; i++) {
        strcpy(current_name, name[i]);
        if (!strcmp(current_name, "\0")) break;
        if (!strcmp(current_name, "!valid")) continue;

        if (existing_var(variables, current_name)) {
            *flag = true;
            continue;
        } else if (verify_type(type, newtypes) && verify_name(name)) {
            variables = add_var(variables, current_type, current_name, row);
        }
    }

    return variables;

}

// aggiungere l'errore se esiste, ritorna la nuova testa della lista
error *errors_management(error *errors, newtype *newtypes, char **type, char **name, int row, bool flag) {

    if (!verify_type(type, newtypes) || !verify_name(name) || flag == true) {
        error *current_err = add_error(errors, row);
        if (!verify_type(type, newtypes)) current_err->wrong_type = true;
        if (!verify_name(name) || flag == true) current_err->wrong_name = true;
        errors = current_err;
    }
    return errors;

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
bool verify_type(char **type, newtype *newtypes) {

    int signed_count = 0;
    int unsigned_count = 0;
    int long_count = 0;
    int short_count = 0;
    char basic_type[8];
    bool exist_basic_type = false;
    bool exist_newtype = false;
    
    char current_word[128];
    /*
        questo ciclo for itera su tutte le parole in array type, ne estrae le seguenti informazioni:
        - quantità di modificatori: "signed", "unsigned", "long", "short";
        - il tipo di base (che può esistere uno solo o anche non esistere secondo standard);
        - un bool che indica se esiste il tipo di base.
        se il tipo di base non esiste, o che esiste più di una, restituisce direttamente false
    */
    for (int i=0; i < 128; i++) {
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

        if (is_basic_type(current_word)) {                  // caso word è tipo base
            if (!exist_basic_type) {
                strcpy(basic_type, current_word);
                exist_basic_type = true;
                continue; 
            } else return false;
        } else if (is_newtype(current_word, newtypes)) {    // caso word è tipo creato con typedef
            if (!exist_newtype) {
                exist_newtype = true;
                continue; 
            } else return false;
        } else return false;
    }

    if (exist_basic_type && exist_newtype) return false;

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
    } else if (exist_newtype) {
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
bool verify_name(char **name) {
    
    bool flag = true;   // false se almeno un nome non è valido

    char current_word[128];
    // itera su tutte le parole in array name, se è una keyword, restituisce false
    for (int i=0; i < 128; i++) {
        strcpy(current_word, name[i]);
        if (!strcmp(current_word, "\0")) break;
        if (is_keyword(current_word)) {
            strcpy(name[i], "!valid");  // la stringa "!valid" indica nome non valido, da ignorare quando si memorizzerà
            flag = false;
        }

        char current_char;
        // itera su tutti i char di ogni nome e restituisce false se il nome non è valido
        for (int j=0; j < 128; j++) {
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
void array_to_string(char **array, char string[]) {
    for (int i=0; i < 128; i++) {
        if (!strcmp(array[i], "\0")) break;
        if (i != 0) strcat(string, " ");
        strcat(string, array[i]);
    }
}

// calcola la statistica di elaborazione
void get_processing_statistics(processing_statistics *statistics, variable *variables, error *errors) {

    variable *current_var = variables;
    while (current_var != NULL) {
        statistics->var_count++;
        if (!current_var->used) statistics->var_unused_count++;
        current_var = current_var->next;
    }

    error *current_err = errors;
    while (current_err != NULL) {
        statistics->err_count++;
        if (current_err->wrong_type) statistics->wrong_var_type_count++;
        if (current_err->wrong_name) statistics->wrong_var_name_count++;
        current_err = current_err->next;
    }

}

// ritorna true se in questa riga words contiene main
bool is_main(char **words) {
    for (int i=0; i < 128; i++) {
        if (!strcmp(words[i], "\0")) break;
        if (!strcmp(words[i], "main")) return true;
    }
    return false;
}

// data la prima word di una riga, restituisce true se è finita la parte di dichiarazione variabile
bool end_variable_declaration(char word[], newtype *newtypes) {

    char *keywords[] = {"const", "volatile", "restrict", "signed", "unsigned", "long", "short",
                        "char", "int", "double", "float", "void", "_Bool", "bool", "typedef"};
    for (int i=0; i < 15; i++) {
        if (!strcmp(keywords[i], word)) return false;
    }
    if (is_newtype(word, newtypes)) return false;
    return true;

} // !!! DA RIVEDERE (TIPI ERRONEI NON ESISTENTI FANNO FINIRE DIRETTAMENTE LA PARTE DICHIARAZIONE VARIABILE) !!!

// estrae le variabili usate e aggiorna nella lista concatenata variable->used = true
void count_used_variables(char **words, variable *variables) {

    for (int i=0; words[i][0] != '\0'; i++) {

        variable *head_vars = variables;        // copy di variables per ogni iterazione (per non perdere la testa)
        while (head_vars != NULL) {
            if (!strcmp(words[i], head_vars->name)) {
                head_vars->used = true;
                break;
            }
            head_vars = head_vars->next;
        }
    }

}

/* ____________________NineNine____________________ */

/* ____________________ananas____________________ */

// funzione per rimuovere commenti
char* remove_comments(char *line) {
    if (line == NULL) {
        return NULL;
    }
    //1. Gestione commento //
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '/' && line[i + 1] == '/') {
            line[i] = '\0';
            break;
        }
    }
    //2. Gestione commento /*
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '/' && line[i + 1] == '*') {
            int start = i;
            int end = -1;
            // cerca fine commento
            for (int j = i + 2; line[j] != '\0'; j++) {
                if (line[j] == '*' && line[j + 1] == '/') {
                    end = j;
                    break;
                }
            }
            //copia resto stringa
            if (end != -1) {
                int k;
                for (k = 0; line[end + 2 + k] != '\0'; k++) {
                    line[start + k] = line[end + 2 + k];
                }
                line[start + k] = '\0';
            }
            else {
                line[start] = '\0';
            }
            break;
        }
    }
    return line;
}

// mostra a utente compilazione corretta
void input() {
    printf("myPrecompiler -i <file input> [-o <file output>] [-v]\n");
    printf("L'ordine degli argomenti è definito dall'ordine delle opzioni\n");
}

/* ____________________ananas____________________ */









