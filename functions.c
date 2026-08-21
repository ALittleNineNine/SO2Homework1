#include "header.h"

// crea un nuovo nodo variabile e lo collega in testa alla lista variabili
variable *add_var(variable *next_var, char type[], char name[], int row) {

    variable *new_var = (variable *) malloc(sizeof(variable));
    strcpy(new_var->type, type);
    strcpy(new_var->name, name);
    new_var->used = false;
    new_var->row = row;
    new_var->next = next_var;
    return new_var;

}

// crea un nuovo nodo errore e lo collega in testa alla lista errori
error *add_error(error *next_err, int row) {

    error *new_err = (error *) malloc(sizeof(error));
    new_err->wrong_type = false;
    new_err->wrong_name = false;
    new_err->row = row;
    new_err->next = next_err;
    return new_err;

}

/*
    data una riga di codice, li spezza in al massimo in parole:
    - se start_statement_section == true: le parole vengono spezzate in base anche a simboli speciali;
    - altrimenti: le parole vengono spezzate solo mediante ' ', '\t', '\n', ';', '=', '\0' e '*';
    - quando si incontrano '...' (char), "..." (array di char), [...] e {...} (dichiarazione e inizializzazione di array, verificato se lo è davvero)
        vengono uniti in un unica word.
*/
void analyze_row(char *row, char **words, bool start_statement_section) {

    int flag = 0;       // posizione della parola in array
    int idx_char = 0;   // posizione del char in ogni parola

    bool in_string = false;     // true se siamo in una stringa
    bool in_char = false;       // true se siamo in un char

    bool in_brace = false;      // true se siamo in una graffa (per inizializzazione array)
    bool in_bracket = false;    // true se siamo in una quadra (per definizione array)

    for (int i=0; row[i] != '\0' && flag < 128; i++) {

        char current_char = row[i];

        if (current_char == '"') in_string = !in_string;    // gestione alternata quando si incontra "
        if (in_string) {
            if (idx_char < 127) {
                words[flag][idx_char] = current_char;
                idx_char++;
            } else {
                words[flag][127] = '\0';
                flag++;
                idx_char = 0;
                if (flag >= 128) break;
            }
            continue;
        }
        if (current_char == '\'') in_char = !in_char;       // gestione alternata quando si incontra '
        if (in_char) {
            if (idx_char < 127) {
                words[flag][idx_char] = current_char;
                idx_char++;
            } else {
                words[flag][127] = '\0';
                flag++;
                idx_char = 0;
                if (flag >= 128) break;
            }
            continue;
        }

        // gestione parentesi graffe (per inizializzazione array)
        if (!start_statement_section && current_char == '{' && flag > 0 && !strcmp(words[flag-1], "=")) {
            for (int j=0; j<flag; j++) {
                if (words[j][0] == '[') {
                    in_brace = true;
                    break;
                }
            }
        }
        if (in_brace) {
            if (idx_char < 127) {
                words[flag][idx_char] = current_char;
                idx_char++;
            }
            if (current_char == '}') {
                in_brace = false;
                if (idx_char < 127) {
                    words[flag][idx_char] = '\0';
                } else {
                    words[flag][127] = '\0';
                }
                flag++;
                idx_char = 0;
            } else {
                if (idx_char >= 127) {
                    words[flag][127] = '\0';
                    flag++;
                    idx_char = 0;
                    if (flag >= 128) break;
                }
            }
            continue;
        }

        // gestione parentesi quadre (per definizione array)
        if (!start_statement_section && current_char == '[') in_bracket = true;
        if (in_bracket) {
            if (idx_char < 127) {
                words[flag][idx_char] = current_char;
                idx_char++;
            }
            if (current_char == ']') {
                in_bracket = false;
                if (idx_char < 127) {
                    words[flag][idx_char] = '\0';
                } else {
                    words[flag][127] = '\0';
                }
                flag++;
                idx_char = 0;
            } else {
                if (idx_char >= 127) {
                    words[flag][127] = '\0';
                    flag++;
                    idx_char = 0;
                    if (flag >= 128) break;
                }
            }
            continue;
        }

        // gestione principale
        if (current_char != ' ' && current_char != '\t' && current_char != '\n') {      // char da ignorare
            if (idx_char < 127) {
                words[flag][idx_char] = current_char;
                idx_char++;
            } else {
                words[flag][127] = '\0';
                flag++;
                idx_char = 0;
                if (flag >= 128) break;
                if (idx_char < 127) {
                    words[flag][idx_char] = current_char;
                    idx_char++;
                }
            }

            char next_char = row[i+1];
            if (start_statement_section) {      // gestione se andare alla prossima word

                if (current_char < 48 || (current_char > 57 && current_char < 65) || (current_char > 90 && current_char < 95) ||
                    (current_char > 95 && current_char < 97) || current_char > 122 ||
                    next_char < 48 || (next_char > 57 && next_char < 65) || (next_char > 90 && next_char < 95) ||
                    (next_char > 95 && next_char < 97) || next_char > 122 || next_char == '\0') {
                    
                    words[flag][idx_char] = '\0';
                    flag++;
                    idx_char = 0;

                }

            } else {

                if (next_char == ' ' || next_char == '\t' || next_char == '\n' || next_char == ';' || next_char == '\0' || next_char == '(' ||
                    current_char == '*' || next_char == '*' || current_char == '=' || next_char == '=' || current_char == ',' || next_char == ',' ||
                    next_char == '{' || current_char == '}' || next_char == '[' || current_char == ']') {

                    words[flag][idx_char] = '\0';
                    flag++;
                    idx_char = 0;

                }

            }
        }
        if (flag >= 128) break;

    }

}

// crea un nuovo nodo newtype e lo collega in testa alla lista newtype (riguardante typedef senza struct)
newtype *add_newtype_no_struct(newtype *newtypes, char **words) {

    int idx_type = 0;
    while (words[idx_type][0] != '\0') idx_type++;

    newtype *new_type = (newtype *) malloc(sizeof(newtype));
    strcpy(new_type->type, words[idx_type-2]);
    new_type->next = newtypes;
    return new_type;

}

// crea un nuovo nodo newtype e lo collega in testa alla lista newtype (riguardante typedef con struct)
newtype *add_newtype_struct(newtype *newtypes, char **words, int idx) {

    if (words[idx + 1][0] == '\0') {
        return newtypes;
    }
    newtype *new_type = (newtype *) malloc(sizeof(newtype));
    if (new_type == NULL) {
        return newtypes;
    }
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

    if (!strcmp(words[1], ";")) {       // caso in cui non è stato indicato un nome per tale tipo
        strcat(type[0], words[0]);
        return 1;
    }

    int length = 0; // lunghezza della parte type
    for (int i=0; i < 127; i++) {
        if (!strcmp(words[i+1], "=") || !strcmp(words[i+1], ",") || !strcmp(words[i+1], ";") || words[i+1][0] == '[' || words[i][0] == '*') {
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
        if (words[i][0] == '[' || words[i][0] == '*') continue;
        strcat(name[j], words[i]);
        j++;
    }

}

// aggiungere la/le variabile/i se non ci sono errori, ritorna la nuova testa della lista
variable *variables_management(variable *variables, newtype *newtypes, char **type, char **name, int row, bool *flag) {

    char current_type[512] = {0};
    array_to_string(type, current_type);
    char current_name[128] = {0};

    if (current_type[0] == '\0') return variables;

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
    
    if (!strcmp(name[0], "\0")) {       // se non esiste nessun nome, rileva errore nome
        strcpy(name[0], "!valid");
        return false;
    }

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

// date le liste concatenate variables e errors, li mette in ordine invertito
void reverse_linked_list(variable **variables, error **errors) {

    variable *prev_var = NULL;
    variable *current_var = *variables;
    variable *next_var = NULL;

    while (current_var != NULL) {
        next_var = current_var->next;
        current_var->next = prev_var;
        prev_var = current_var;
        current_var = next_var;
    }

    *variables = prev_var;

    error *prev_err = NULL;
    error *current_err = *errors;
    error *next_err = NULL;

    while (current_err != NULL) {
        next_err = current_err->next;
        current_err->next = prev_err;
        prev_err = current_err;
        current_err = next_err;
    }

    *errors = prev_err;

}

// calcola la statistica di elaborazione
void get_processing_statistics(processing_statistics *statistics, variable *variables, error *errors) {

    statistics->var_count = 0;
    statistics->err_count = 0;
    statistics->var_unused_count = 0;
    statistics->wrong_var_name_count = 0;
    statistics->wrong_var_type_count = 0;

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

// printa la statistica di elaborazione
void print_processing_statistics(FILE *out, processing_statistics *statistics, variable *variables, error *errors) {

    fprintf(out, "\n---------- STATISTICHE DI ELABORAZIONE -----------\n\n");

    fprintf(out, "Numero totale di variabili valide:\t\t%d\n", statistics->var_count);
    fprintf(out, "Numero totale di errori rilevati:\t\t%d\n", statistics->wrong_var_type_count + 
                                                        statistics->wrong_var_name_count +
                                                        statistics->var_unused_count);
    fprintf(out, "Numero di variabili non utilizzate:\t\t%d\n", statistics->var_unused_count);
    fprintf(out, "Numero di nomi di variabili non corretti:\t%d\n", statistics->wrong_var_name_count);
    fprintf(out, "Numero di tipi di dato non corretti:\t\t%d\n", statistics->wrong_var_type_count);

    fprintf(out, "\n--------------------------------------------------\n");
    
    fprintf(out, "\n--- ERRORI RILEVATI ---\n\n");

    error *current_err = errors;
    while (current_err != NULL) {
        if (current_err->wrong_type) {
            fprintf(out, "Errore tipo in riga %d\n", current_err->row);
        }
        if (current_err->wrong_name) {
            fprintf(out, "Errore nome in riga %d\n", current_err->row);
        }
        current_err = current_err->next;
    }

    fprintf(out, "\n-----------------------\n");
    
    fprintf(out, "\n-------------- VARIABILI NON UTILIZZATE --------------\n\n");

    variable *current_var = variables;
    while (current_var != NULL) {
        if (!current_var->used) {
            fprintf(out, "%s", current_var->name);
            int padding = 32 - (int)strlen(current_var->name);
            if (padding > 0) {
                for (int i = 0; i < padding; i++) fprintf(out, " ");
            }
            fprintf(out, "dichiarata in riga %d\n", current_var->row);
        }
        current_var = current_var->next;
    }

    fprintf(out, "\n------------------------------------------------------\n\n");

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
bool end_variable_declaration(char word[], variable *variables) {

    char *keywords[] = {"if", "do", "while", "for", "switch", "return", "sizeof"};

    for (int i=0; i < 7; i++) {
        if (!strcmp(keywords[i], word)) return true;
    }
    while (variables != NULL) {
        if (!strcmp(variables->name, word)) return true;
        variables = variables->next;
    }
    return false;

}

// estrae le variabili usate e aggiorna nella lista concatenata variable->used = true
void count_used_variables(char **words, variable *variables) {

    for (int i=0; words[i][0] != '\0'; i++) {
        if (words[i+1][0] == '\0') continue;

        variable *head_vars = variables;        // copy di variables per ogni iterazione (per non perdere la testa)
        while (head_vars != NULL) {
            if (!strcmp(words[i], head_vars->name)) {

                // caso array
                if (words[i+1][0] != '\0' && !strcmp(words[i+1], "[")) {
                    int end_brackets_array = i+1;   // indice dopo l'ultimo "]" dopo il nome di variabile
                    while (words[end_brackets_array][0] != '\0' && !strcmp(words[end_brackets_array], "[")) {
                        while (words[end_brackets_array][0] != '\0' && strcmp(words[end_brackets_array], "]")) {
                            end_brackets_array++;
                            if (words[end_brackets_array][0] == '\0') break;
                        }
                        if (words[end_brackets_array][0] == '\0') break;
                        end_brackets_array++;
                    }
                    if (words[end_brackets_array][0] != '\0' && !strcmp(words[end_brackets_array], "=")) break;
                }

                // caso puntatore
                if (i>0 && !strcmp(words[i-1], "*")) {
                    head_vars->used = true;
                    break;
                }

                /*
                    caso assegnazione/riassegnazione:
                    - se la prossima word è "=", escludendo il caso "==", allora salta;
                    - quando è seguito da "=", la variabile subisce una riassegnazione, non usato.
                */
                if (words[i+1][0] != '\0' && !strcmp(words[i+1], "=") && words[i+2][0] != '\0' && strcmp(words[i+2], "=")) {
                    break;
                }
                
                head_vars->used = true;
                break;

            }
            head_vars = head_vars->next;
        }

    }

}

// TEST FOR IMPLEMENTATION
void test_array_of_array(char **words, char **type, char **name, int row) {

    printf("\nParole riga %d: ", row);
    for(int i=0; i < 128; i++) {
        if (!strcmp(words[i], "\0")) break;
        printf("<%s> ", words[i]);
    }
    printf("\nTipo riga %d: ", row);
    for(int i=0; i < 128; i++) {
        if (!strcmp(type[i], "\0")) break;
        printf("<%s> ", type[i]);
    }
    printf("\nNomi riga %d: ", row);
    for(int i=0; i < 128; i++) {
        if (!strcmp(name[i], "\0")) break;
        printf("<%s> ", name[i]);
    }
    printf("\n\n");

}

// TEST FOR IMPLEMENTATION
void test_linked_lists(variable *variables, error *errors, newtype *newtypes) {

    printf("\n--------- VARIABLES ---------\n\n");
    variable *current_var = variables;
    while (current_var != NULL) {
        printf("Tipo di riga %d: %s\n", current_var->row, current_var->type);
        printf("Nome di riga %d: %s\n", current_var->row, current_var->name);
        printf("Usato? %d\n", current_var->used);
        printf("\n");
        current_var = current_var->next;
    }

    printf("\n--------- ERRORS ---------\n\n");
    error *current_err = errors;
    while (current_err != NULL) {
        printf("Errore tipo in riga %d: %d\n", current_err->row, current_err->wrong_type);
        printf("Errore nome in riga %d: %d\n", current_err->row, current_err->wrong_name);
        printf("\n");
        current_err = current_err->next;
    }

    printf("\n--------- NEWTYPES ---------\n\n");
    newtype *current_newtype = newtypes;
    printf("Tipi typedefati: ");
    while (current_newtype != NULL) {
        printf("%s ", current_newtype->type);
        current_newtype = current_newtype->next;
    }

    printf("\n\n");

}

// mostra a utente compilazione corretta
void input() {
    printf("myPrecompiler -i <file input> [-o <file output>] [-v]\n");
    printf("L'ordine degli argomenti è definito dall'ordine delle opzioni\n");
}

// funzione per rimuovere commenti
static int block_comment = 0; // indica se siamo all'interno di un commento

char* remove_comments(char *line) {
    if (line == NULL) {
        return NULL;
    }

    char buffer[4096]; // buffer per copiare riga

    // cerca la chiusura del commento /* 
    if (block_comment) {
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '*' && line[i + 1] == '/') {
                strcpy(buffer, line + i + 2); // salva tutto dopo */ 
                strcpy(line, buffer); // copia all0inizio della riga
                block_comment = 0; // commento chiuso
                return remove_comments(line); // ricontrolla la riga per altri commenti (ricorsione)
            }
        }
        // se tutta la riga è un commento viene svuotata 
        line[0] = '\0';
        return line;
    }

    // commento singola riga
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '/' && line[i + 1] == '/') {
            line[i] = '\0'; // tronca la riga al commento
            break;
        }
    }

    // commenti nella stessa riga
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '/' && line[i + 1] == '*') {
            int start = i; // posizione di inizio commento
            // cerca la fine del commento 
            for (int j = i + 2; line[j] != '\0'; j++) {
                // caso in cui commento chiuso nella stessa riga
                if (line[j] == '*' && line[j + 1] == '/') {
                    strcpy(buffer, line + j + 2); // salva dopo la chiusura
                    strcpy(line + start, buffer); // copia sopra il commento
                    i = start - 1; // reset per ricontrollare da questa posizione
                    break;
                }
                // caso in cui commento non chiuso nella stessa riga
                if (line[j + 1] == '\0') {
                    line[start] = '\0'; // tronca la riga all'inizio del commento
                    block_comment = 1; // segnala commento che continua
                    return line;
                }
            }
        }
    }

    // commenti */ senza /*
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '*' && line[i + 1] == '/') {
            printf("Error: commento non aperto\n");
            strcpy(buffer, line + i + 2); // salva tutto dopo */
            strcpy(line + i, buffer);
            if (i > 0) {
                i = -1; // decrementa per ricontrollare da capo
            }
        }
    }
    return line;
} 
