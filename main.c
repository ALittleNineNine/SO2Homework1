#include "header.h"

int main(int argc, char *argv[]) {

/* ____________________ananas____________________ */

    char *file_input = NULL;
    char *file_output = NULL;
    int verbose = 0; //modalità verbose

    for (int i = 1; i < argc; i++) {
        //opzione input
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--in") == 0) {
            if (i + 1 < argc) {
                file_input = argv[++i]; //prima incremento e poi viene dato l'argomento 
            } else {
                printf("Errore: %s necessario un argomento\n", argv[i]);
                input();
                return 1;
            }
        }
        //opzione output
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0) {
            if (i + 1 < argc) {
                file_output = argv[++i];
            } else {
                printf("Errore: %s necessario un argomento\n", argv[i]);
                input();
                return 1;
            }
        }
        //opzione verbose
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        }
        //opzioni raggruppate
        else if (argv[i][0] == '-' && argv[i][1] != '-' && argv[i][1] != '\0') {
            int len = strlen(argv[i]);
            for (int k = 1; k < len; k++) {
                char current_k = argv[i][k];
                if (current_k == 'v') {
                    verbose = 1;
                }
                else if (current_k == 'i') {
                    if (i + 1 < argc) {
                        file_input = argv[++i];
                    } else {
                        printf("Errore: -i necessita di un argomento\n");
                        input();
                        return 1;
                    }
                }
                else if (current_k == 'o') {
                    if (i + 1 < argc) {
                        file_output = argv[++i];
                    } else {
                        printf("Errore: -o necessita di un argomento\n");
                        input();
                        return 1;
                    }
                }
                else {
                    printf("Errore: opzione errata '-%c'\n", current_k);
                }
            }
        }
        else {
            printf("Errore: opzione errata %s\n", argv[i]);
            input();
            return 1;
        }
    }
    //verifica file input
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

/* ____________________ananas____________________ */

/* ____________________NineNine____________________ */

    FILE *fp;
    fp = fopen(file_input, "r");

    if (fp == NULL) {
        printf("Errore apertura file.\n");
        exit(1);
    }
    
/* ____________________Inizio inizializzazione variabili____________________ */

    variable *variables = NULL;
    error *errors = NULL;
    newtype *newtypes = NULL;

    char new_line[] = "\n";
    
    char *current_row = malloc(1024);
    int row = 0;
    bool row_finished = true;               // viene assegnato false quando inizia un'istruzione a più righe, true normalmente
    int brace_level = 0;                    // variabile accessoria per verificare se la graffa è chiusa bene
    bool in_brace = false;                  // indica se è dentro nelle graffe
    
    bool start_statement_section = false;   // indica se è iniziata la parte delle istruzioni (fine dichiarazione variabili)

    // array di array di char che contiene le righe spezzate, i tipi e i nomi delle variabili
    char **words = malloc(128 * sizeof(char *));
    char **type = malloc(128 * sizeof(char *));
    char **name = malloc(128 * sizeof(char *));
    for (int i=0; i < 128; i++) {
        words[i] = malloc(128 * sizeof(char));
        type[i] = malloc(128 * sizeof(char));
        name[i] = malloc(128 * sizeof(char));
    }

/* ____________________Fine inizializzazione variabili____________________ */

    while (!feof(fp)) {

        row++;
        fgets(current_row, 1024, fp);

        if (strcmp(current_row, new_line)) {

            remove_comments(current_row);

            // azzerare words
            for (int i=0; i < 128; i++) {
                words[i][0] = '\0';
            }

            analyze_row(current_row, words);

/* ____________________Inizio gestione typedef____________________ */

            // aggiungere il nuovo tipo creato con typedef, se è struct viene messo il flag row_finished a false
            if (!strcmp(words[0], "typedef")) {
                if (!strcmp(words[1], "struct")) {
                    row_finished = false;
                    brace_level = 0;
                } else {
                    newtypes = add_newtype_no_struct(newtypes, words);
                    continue;
                }
            }

            // trovare il tipo struct e metterlo in newtypes
            if (!row_finished) {
                int idx = 0;
                while ((words[idx][0] != '\0')) {
                    if (words[idx][0] == '{') {
                        brace_level++;
                        in_brace = true;
                    }
                    if (words[idx][0] == '}') brace_level--;
                    if (brace_level == 0 && in_brace) {
                        newtypes = add_newtype_struct(newtypes, words, idx);
                        row_finished = true;
                        in_brace = false;
                        break;
                    }
                    idx++;
                }
                continue;
            }

/* ____________________Fine gestione typedef____________________ */

            if (!strcmp(words[0], "#")) continue;
            if (!strcmp(words[0], "\0")) continue;
            if (is_main(words)) continue;


            if (!start_statement_section) {

                if (!end_variable_declaration(words[0], newtypes)) {

                    // azzerare type e name
                    for (int i=0; i < 128; i++) {
                        type[i][0] = '\0';
                        name[i][0] = '\0';
                    }

                    int type_length = get_type(words, type);
                    get_name(words, name, type_length);

                    bool flag = false;  // se true, esiste almeno un nome che esisteva già

                    // aggiungere la/le variabile/i se non ci sono errori, ritorna la nuova testa della lista
                    variables = variables_management(variables, newtypes, type, name, row, &flag);

                    // aggiungere l'errore se esiste, ritorna la nuova testa della lista
                    errors = errors_management(errors, newtypes, type, name, row, flag);
                    
                    // TEST FOR IMPLEMENTATION
                    // test_array_of_array(words, type, name, row);

                } else {
                    start_statement_section = true;
                    count_used_variables(words, variables);
                }

            } else {
                // parte verifica se la variabile è usato o no (parte dopo dichiarazione variabile)
                count_used_variables(words, variables);
            }

        }

    }

    fclose(fp);

    // variables e errors sono memorizzati nell'ordine decrescente, qui vengono reversed
    reverse_linked_list(&variables, &errors);
    
    processing_statistics *statistics = malloc(sizeof(processing_statistics));
    get_processing_statistics(statistics, variables, errors);

    printf("\n---------- STATISTICHE DI ELABORAZIONE -----------\n\n");

    printf("Numero totale di variabili valide:\t\t%d\n", statistics->var_count);
    printf("Numero totale di errori rilevati:\t\t%d\n", statistics->err_count);
    printf("Numero di variabili non utilizzate:\t\t%d\n", statistics->var_unused_count);
    printf("Numero di nomi di variabili non corretti:\t%d\n", statistics->wrong_var_name_count);
    printf("Numero di tipi di dato non corretti:\t\t%d\n", statistics->wrong_var_type_count);

    printf("\n--------------------------------------------------\n");
    
    printf("\n--- ERRORI RILEVATI ---\n\n");

    error *current_err = errors;
    while (current_err != NULL) {
        if (current_err->wrong_type) {
            printf("Errore tipo in riga %d\n", current_err->row);
        }
        if (current_err->wrong_name) {
            printf("Errore nome in riga %d\n", current_err->row);
        }
        current_err = current_err->next;
    }

    printf("\n-----------------------\n");
    
    printf("\n------ VARIABILI NON UTILIZZATE ------\n\n");

    variable *current_var = variables;
    while (current_var != NULL) {
        if (!current_var->used) {
            printf("%s\t\tdichiarata in riga %d\n", current_var->name, current_var->row);
        }
        current_var = current_var->next;
    }

    printf("\n--------------------------------------\n\n");

    // TEST FOR IMPLEMENTATION
    // test_linked_lists(variables, errors, newtypes);

/* ____________________Inizio pulizia memoria____________________ */

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

    // pulizia memoria newtypes
    newtype *next_newtype;
    while (newtypes != NULL) {
        next_newtype = newtypes->next;
        free(newtypes);
        newtypes = next_newtype;
    }

    // pulizie array di array words, type e name
    for (int i=0; i < 128; i++) {
        free(words[i]);
        free(type[i]);
        free(name[i]);
    }
    free(words);
    free(type);
    free(name);

    // pulizie memoria variabili
    free(current_row);
    free(statistics);

/* ____________________Fine pulizia memoria____________________ */

    return 0;

/* ____________________NineNine____________________ */

}











