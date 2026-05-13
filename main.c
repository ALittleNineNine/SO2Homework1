#include "header.h"

int main(int argc, char *argv[]) {

    // inizio ananas

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
    char *current_row = malloc(1024);
    int row = 0;
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

            if (!strcmp(words[0], "#")) continue;
            if (!strcmp(words[0], "\0")) continue;
            if (is_main(words)) continue;

            if (!start_statement_section ) {

                if (!end_variable_declaration(words[0])) {

                    // azzerare type e name
                    for (int i=0; i < 128; i++) {
                        type[i][0] = '\0';
                        name[i][0] = '\0';
                    }

                    int type_length = get_type(words, type);
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
                    for(int i=0; i < 128; i++) {
                        if (!strcmp(words[i], "\0")) break;
                        printf("%s ", words[i]);
                    }
                    printf("\nTipo riga %d: ", row);
                    for(int i=0; i < 128; i++) {
                        if (!strcmp(type[i], "\0")) break;
                        printf("%s ", type[i]);
                    }
                    printf("\nNomi riga %d: ", row);
                    for(int i=0; i < 128; i++) {
                        if (!strcmp(name[i], "\0")) break;
                        printf("%s ", name[i]);
                    }
                    printf("\n");
                    
                    // fine prova

                } else start_statement_section = true;

            } else {

                // parte verifica se la variabile è usato o no (parte dopo dichiarazione variabile) [DA IMPLEMENTARE]

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
    free(var_err_count);

    return 0;

    // fine NineNine

}











