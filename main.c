#include "header.h"

int main(int argc, char *argv[]) {

    FILE *fp;
    fp = fopen(argv[1], "r");

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

            char words[64][64] = {0};
            analyze_row(current_row, words);

            if (!strcmp(words[0], "#")) continue;
            if (!strcmp(words[0], "/")) continue;
            if (is_main(words)) continue;

            if (!start_statement_section) {

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

}











