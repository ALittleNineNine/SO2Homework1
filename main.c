#include "header.h"

int main(int argc, char *argv[]) {

    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Errore apertura file.\n");
        exit(1);
    }
    
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
            get_type(words, type);



            printf("Parole di riga %d: ", row);
            for (int i=0; i<64; i++) {
                if (!strcmp(words[i], "\0")) break;
                printf("%s ", words[i]);
            }
            printf("\n");

            printf("Tipi di riga %d: ", row);
            for (int i=0; i<64; i++) {
                if (!strcmp(type[i], "\0")) break;
                printf("%s ", type[i]);
            }
            printf("\n\n");

        }
    }

    fclose(fp);

    return 0;

}