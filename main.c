#include "header.h"

int main(int argc, char *argv[]) {

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

            // verifica che ci siano abbastanza argomenti
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

        // verifica argomenti
        if (argc < 2) {
            printf("Errore: nessun parametro specificato\n");
            input();
            return 1;
        }

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
        printf("\n");

    // apertura file input
    FILE *fp;
    fp = fopen(file_input, "r");

    if (fp == NULL) {
        printf("Errore apertura file\n");
        return 1;
    }
    
    /* ____________________Inizio inizializzazione variabili____________________ */

    variable *variables = NULL;
    error *errors = NULL;
    newtype *newtypes = NULL;
    
    char *current_row = (char *) malloc(1024);
    // controllo allocazione
    if (current_row == NULL) {
        printf("Errore: allocazione malloc fallita\n");
        fpclose(fp);
        return 1;
    }

    int row = 0;
    bool row_finished = true;               // viene assegnato false quando inizia un'istruzione a più righe, true normalmente
    int brace_level = 0;                    // variabile accessoria per verificare se la graffa è chiusa bene
    bool in_brace = false;                  // indica se è dentro nelle graffe
    
    bool start_statement_section = false;   // indica se è iniziata la parte delle istruzioni (fine dichiarazione variabili)

    // array di array di char che contiene le righe spezzate, i tipi e i nomi delle variabili
    char **words = (char **) malloc(128 * sizeof(char *));
    char **type = (char **) malloc(128 * sizeof(char *));
    char **name = (char **) malloc(128 * sizeof(char *));
    // controllo allocazione
    if (words == NULL || type == NULL || name == NULL) {
        printf("Errore: allocazione malloc fallita per gli array di array di char\n");
        free(current_row);
        fclose(fp);
        return 1;
    }

    for (int i=0; i < 128; i++) {
        words[i] = (char *) malloc(128 * sizeof(char));
        type[i] = (char *) malloc(128 * sizeof(char));
        name[i] = (char *) malloc(128 * sizeof(char));
        // controllo allocazione
        if (words[i] == NULL || type[i] == NULL || name[i] == NULL) {
            printf("Errore: allocazione malloc fallita per le parole '%d'\n", i);
            for (int j = 0; j < i; j++) {
                free(words[j]);
                free(type[j]);
                free(name[j]);
            }
            free(words);
            free(type);
            free(name);
            free(current_row);
            fclose(fp);
            return 1;
        }
    }

    // struct per memorizzare la statistica
    processing_statistics *statistics = (processing_statistics *) malloc(sizeof(processing_statistics));
    // controllo allocazione
    if (statistics == NULL) {
        printf("Errore: allocazione malloc fallita per statistiche\n");

        for (int i=0; i < 128; i++) {
        free(words[i]);
        free(type[i]);
        free(name[i]);
    }
        free(words);
        free(type);
        free(name);
        free(current_row);
        fclose(fp);
        return 1;
    }

    /* ____________________Fine inizializzazione variabili____________________ */

    int contenuto = 0; // controllo se file è vuoto

    while (fgets(current_row, 1024, fp) != NULL) {
        contenuto = 1;
        
        row++;

        if (current_row[0] != '\n' && current_row[0] != '\0') {

            remove_comments(current_row); 
            

            // azzerare words
            for (int i=0; i < 128; i++) {
                words[i][0] = '\0';
            }

            analyze_row(current_row, words, start_statement_section);


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

            if (!strcmp(words[0], "#include")) continue;
            if (!strcmp(words[0], "\0")) continue;
            if (is_main(words)) continue;

            if (!start_statement_section) {

                if (!end_variable_declaration(words[0], variables)) {

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

                    // TEST FOR IMPLEMENTATION
                    // test_array_of_array(words, type, name, row);

                }

            } else {

                // se incontro return 0, la lettura finisce
                if (!strcmp(words[0], "return") && !strcmp(words[1], "0")) break;

                // parte verifica se la variabile è usato o no (parte dopo dichiarazione variabile)
                count_used_variables(words, variables);

                // TEST FOR IMPLEMENTATION
                // test_array_of_array(words, type, name, row);

            }

        }

    }
    
    // controllo file vuoto
    if (!contenuto) {
        printf("Errore: il file '%s' è vuoto\n", file_input);
        fclose(fp);
        free_all(variables, errors, newtypes, words, type, name, current_row, statistics);
        return 1;
    }

    // chiusura file input
    if (fclose(fp) != 0) {
        printf("Errore: impossibile chiudere il file '%s'\n", file_input);
        return 1;
    }

    // variables e errors sono memorizzati nell'ordine decrescente, qui vengono reversed
    reverse_linked_list(&variables, &errors);
    
    if (statistics != NULL) {
        statistics->var_count = 0;
        statistics->err_count = 0;
        statistics->var_unused_count = 0;
        statistics->wrong_var_name_count = 0;
        statistics->wrong_var_type_count = 0;
    }
    get_processing_statistics(statistics, variables, errors);

    // apertura, scrittura e chiusura su file output
    if (file_output != NULL) {
        FILE *f_out = fopen(file_output, "w");
        if (f_out == NULL) {
            printf("Errore: impossibile creare il file '%s'\n", file_output);
            return 1;
        }
        if (print_processing_statistics(f_out, statistics, variables, errors) != 0) {
            printf("Errore: scrittura su file fallita\n");
            fclose(f_out);
            return 1;
        }

        if (fclose(f_out) != 0) {
            printf("Errore: impossibile chiudere il file '%s'\n", file_output);
            return 1;
        }
    } 

    if (verbose || file_output == NULL) {
        if (print_processing_statistics(stdout, statistics, variables, errors) != 0) {
            printf("Errore: scrittura su stdout fallita\n");
            return 1;
        }
    }

    // TEST FOR IMPLEMENTATION
    // test_linked_lists(variables, errors, newtypes);

    // pulizia memoria allocata
    free_all(variables, errors, newtypes, words, type, name, current_row, statistics);

    return 0;

}