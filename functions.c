#include "header.h"

//funzione per rimuovere commenti
char* remove_comments(char *line) {
    if (line == NULL) {
        return NULL;
    }

    //1. Gestione commento //
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '/' && line[i + 1] == '/') {
            line[i] = '\0';
            break;
        }
    }

    //2. Gestione commento /*
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '/' && line[i + 1] == '*') {
            int start = i;
            int end = -1;
            // cerca fine commento
            for (int j = i + 2; line[j] != '\0'; j++) {
                if (line[j] == '*' && line[j + 1] == '/') {
                    end = j;
                    break;
                }
            }

            //copia resto stringa
            if (end != -1) {
                int k;
                for (k = 0; line[end + 2 + k] != '\0'; k++) {
                    line[start + k] = line[end + 2 + k];
                }
                
                line[start + k] = '\0';
            } 

            else {
                line[start] = '\0';
            }

            break;
        }
    }

    return line;
}
