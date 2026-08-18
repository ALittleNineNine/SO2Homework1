#include "header.h"

int main(int argc, char *argv[]) {

    // inizio ananas

    char *file_input = NULL;
    char *file_output = NULL;
    int verbose = 0; //modalità verbose

    for (int i = 1; i < argc; i++) {
        // opzione input
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--in") == 0) {
            if (i + 1 < argc) {
                file_input = argv[++i]; //prima incremento e poi viene dato l'argomento 
            } else {
                printf("Errore: %s necessario un argomento\n", argv[i]);
                input();
                return 1;
            }
        }
        // opzione output
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0) {
            if (i + 1 < argc) {
                file_output = argv[++i];
            } else {
                printf("Errore: %s necessario un argomento\n", argv[i]);
                input();
                return 1;
            }
        }
        // opzione verbose
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        }
        // opzioni raggruppate -vio -ivo -vi -ov ...
        else if (argv[i][0] == '-' && argv[i][1] != '-' && argv[i][1] != '\0') {
            int len = strlen(argv[i]);
            int num_arg = 0; // conta quanti argomenti sono consumati (gestisce offset per accedere corettamente agli argomenti delle opzioni)

            // veridica che ci siano abbastanza argomenti
            int argomenti = 0;
            for (int k = 1; k < len; k++) {
                if (argv[i][k] == 'i' || argv[i][k] == 'o') {
                    argomenti++;
                }
            }

            if (argomenti > 0 && i + argomenti >= argc) {
                printf("Errore: mancano argomenti\n");
                input();
                return 1;
            }

            //processare le opzioni
            for (int k = 1; k < len; k++) {
                char current_k = argv[i][k];
                if (current_k == 'v') {
                    verbose = 1;
                }
                else if (current_k == 'i') {
                    file_input = argv[i + 1 + num_arg];
                    num_arg++;
                }
                else if (current_k == 'o') {
                    file_output = argv[i + 1 + num_arg];
                    num_arg++;
                }
                else {
                    printf("Errore: opzione errata '-%c'\n", current_k);
                    input();
                    return 1;
                }
            }
            // salta argomenti già consumati
            if (num_arg > 0) {
                i += num_arg;
            }
        }
        else {
            printf("Errore: opzione errata %s\n", argv[i]);
            input();
            return 1;
        }
    }
        // verifica file input
        if (file_input == NULL) {
            printf("Errore: manca file input\n");
            input();
            return 1;
        } 
        printf("Input: %s\n", file_input);
        if (file_output) {
            printf("Output: %s\n", file_output);
        }
        if (verbose) {
            printf("Presente opzione verbose\n");
        }
        printf("\n\n\n");



    // fine ananas

    // inizio NineNine

    FILE *fp;
    fp = fopen(file_input, "r");

    if (fp == NULL) {
        printf("Errore apertura file.\n");
        exit(1);
    }
    
    variable *variables = NULL;
    error *errors = NULL;
    char new_line[] = "\n";
    char current_row[1024];
    int row = 0;
    bool start_statement_section = false;   // indica se è iniziata la parte delle istruzioni (fine dichiarazione variabili)

    while (!feof(fp)) {

        row++;
        fgets(current_row, sizeof(current_row), fp);

        if (strcmp(current_row, new_line)) {
            remove_comments(current_row);
            printf("%s", current_row);

            char words[64][64] = {0};
            analyze_row(current_row, words);

            if (!strcmp(words[0], "#")) continue;
            if (!strcmp(words[0], "\0")) continue;
            if (is_main(words)) continue;

            if (!start_statement_section ) {

                if (!end_variable_declaration(words[0])) {

                    char type[64][64] = {0};
                    int type_length = get_type(words, type);
                    char name[64][64] = {0};
                    get_name(words, name, type_length);

                    // aggiungere la/le variabile/i se non ci sono errori
                    bool flag = false;  // se true, esiste almeno un nome che esisteva già
                    char current_type[512] = {0};
                    array_to_string(type, current_type);

                    char current_name[64] = {0};

                    for (int i=0; i < 64; i++) {
                        strcpy(current_name, name[i]);
                        if (!strcmp(current_name, "\0")) break;
                        if (!strcmp(current_name, "!valid")) continue;

                        if (existing_var(variables, current_name)) {
                            flag = true;
                            continue;
                        } else if (verify_type(type) && verify_name(name)) {
                            variable *current_var = add_var(variables, current_type, current_name, row);
                            variables = current_var;
                        }
                    }

                    // aggiungere l'errore se esiste
                    if (!verify_type(type) || !verify_name(name) || flag == true) {
                        error *current_err = add_error(errors, row);
                        if (!verify_type(type)) current_err->wrong_type = true;
                        if (!verify_name(name) || flag == true) current_err->wrong_name = true;
                        errors = current_err;
                    }

                    // inizio prova
                    
                    printf("\nParole riga %d: ", row);
                    for(int i=0; i < 64; i++) {
                        if (!strcmp(words[i], "\0")) break;
                        printf("%s ", words[i]);
                    }
                    printf("\nTipo riga %d: ", row);
                    for(int i=0; i < 64; i++) {
                        if (!strcmp(type[i], "\0")) break;
                        printf("%s ", type[i]);
                    }
                    printf("\nNomi riga %d: ", row);
                    for(int i=0; i < 64; i++) {
                        if (!strcmp(name[i], "\0")) break;
                        printf("%s ", name[i]);
                    }
                    printf("\n");
                    
                    // fine prova

                } else start_statement_section = true;

            } else {

                // parte verifica se la variabile è usato o no (parte dopo dichiarazione variabile) [da implementare]

            }

        }

    }

    fclose(fp);
    
    int *var_err_count = calloc(2, sizeof(int));    // [var_count, err_count]
    linked_list_count(var_err_count, variables, errors);

    printf("\n--- STATISTICHE DI ELABORAZIONE ---\n\n");
    printf("Numero totale di variabili valide: %d\n", var_err_count[0]);
    printf("Numero totale di errori rilevati: %d\n", var_err_count[1]);



    // inizio parte prova

    printf("\n--------- VARIABLES ---------\n\n");
    variable *current_var = variables;
    while (current_var != NULL) {
        printf("Tipo di riga %d: %s\n", current_var->row, current_var->type);
        printf("Nome di riga %d: %s\n", current_var->row, current_var->name);
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
    
    // fine parte prova

    // pulizia memoria variables
    variable *next_var;
    while (variables != NULL) {
        next_var = variables->next;
        free(variables);
        variables = next_var;
    }

    // pulizia memoria errors
    error *next_err;
    while (errors != NULL) {
        next_err = errors->next;
        free(errors);
        errors = next_err;
    }

    // pulizia memoria var_err_count
    free(var_err_count);

    return 0;

    // fine NineNine

}













// [from ananas]
// mostra a utente compilazione corretta [from ananas]
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
                i--; // decrementa per ricontrollare la stessa posizione */
            }
        }
    }
    return line;
}

// funzione per elaborare un intero file riga per riga
void comment_file(FILE *file) {
    char line[4096];
    while (fgets(line, sizeof(line), file)) {
        remove_comments(line);
        if (line[0] != '\0') {
            printf("%s", line);
        } else {
            printf("\n");
        }
    }
    if (block_comment) {
        printf("Errore: commento non chiuso alla fine del file\n");
    }
}