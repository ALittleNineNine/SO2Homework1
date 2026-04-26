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
    char current_row[256];
    int row = 0;

    while (!feof(fp)) {

        row++;
        fgets(current_row, sizeof(current_row), fp);

        if (strcmp(current_row, new_line)) {

            char words[64][64] = {0};
            analyze_row(current_row, words);
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

        }

    }
    
    //inizio parte prova

    printf("\n");
    printf("---------VARIABLES---------\n\n");
    variable *current_var = variables;
    while (current_var != NULL) {
        printf("Tipo di riga %d: %s\n", current_var->row, current_var->type);
        printf("Nome di riga %d: %s\n", current_var->row, current_var->name);
        printf("\n");
        current_var = current_var->next;
    }

    printf("\n");
    printf("---------ERRORS---------\n\n");
    error *current_err = errors;
    while (current_err != NULL) {
        printf("Errore tipo in riga %d: %d\n", current_err->row, current_err->wrong_type);
        printf("Errore nome in riga %d: %d\n", current_err->row, current_err->wrong_name);
        printf("\n");
        current_err = current_err->next;
    }

    //fine parte prova

    fclose(fp);

    return 0;

}








