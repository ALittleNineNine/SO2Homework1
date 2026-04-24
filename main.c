#include "header.h"

int main(int argc, char *argv[]) {

    char current_row[256];
    char new_line[] = "\n";

    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Errore apertura file.\n");
        exit(1);
    }

    while (!feof(fp)) {
        fgets(current_row, sizeof(current_row), fp);
        if (strcmp(current_row, new_line)) {
            char words[7][64] = {0};
            analyze_row(current_row, words);

            for (int i=0; i<7; i++) {
                printf("%s ", words[i]);
            }
            printf("\n");

        }
    }

    fclose(fp);

    return 0;

}