#include "header.h"

int main(int argc, char *argv[]) {

    char *file_input = NULL;
    char *file_output = NULL;
    int verbose = 0;    // modalità verbose

    // analizza gli argomenti del main
    if (analyze_arguments(argc, argv, &file_input, &file_output, &verbose) != 0) return 1;

    // apertura file input
    FILE *fp;
    fp = fopen(file_input, "r");

    if (fp == NULL) {
        fprintf(stderr, "Errore apertura file\n");
        return 1;
    }

    variable *variables = NULL;
    newtype *newtypes = NULL;
    char *current_row;
    char **words;
    char **type;
    char **name;
    processing_statistics *statistics;

    if (allocate_resources(&current_row, &words, &type, &name, &statistics) != 0) {
        fclose(fp);
        return 1;
    }

    int contenuto = 0;  // controllo se file è vuoto

    // analizza riga per riga il file .c in input
    analyze_file(fp, &variables, &newtypes, &current_row, &words, &type, &name, &contenuto);

    // controllo lettura file input
    if (ferror(fp) || !contenuto) {
        if (ferror(fp)) fprintf(stderr, "Errore: impossibile leggere il file '%s'\n", file_input);  // controllo file inleggibile
        if (!contenuto) fprintf(stderr, "Errore: il file '%s' è vuoto\n", file_input);              // controllo file vuoto
        fclose(fp);
        free_all(variables, newtypes, words, type, name, current_row, statistics);
        return 1;
    }

    // chiusura file input
    if (fclose(fp) != 0) {
        fprintf(stderr, "Errore: impossibile chiudere il file '%s'\n", file_input);
        free_all(variables, newtypes, words, type, name, current_row, statistics);
        return 1;
    }

    // variables e errors sono memorizzati nell'ordine decrescente, qui vengono reversed
    reverse_linked_list(&variables);
    
    // genera la statistica di esecuzione
    get_processing_statistics(statistics, variables);

    // apertura, scrittura e chiusura su file output
    if (output_file_control(&file_output, variables, statistics, verbose) != 0) return 1;

    // test_linked_lists(variables, newtypes);     // [TEST FOR IMPLEMENTATION]

    // pulizia memoria allocata
    free_all(variables, newtypes, words, type, name, current_row, statistics);

    return 0;

}



