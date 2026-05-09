#include "header.h"

int main(int argc, char *argv[]) {
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
        return 0;
}
