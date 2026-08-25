#include <stdio.h>

int intero;                     // type valido     | name valido     | non verrà usata
char 9carattere;                // type valido     | name non valido | --
dabol virgola_mobile;           // type non valido | name valido     | --
floot 9virgola_mobile_corto;    // type non valido | name non valido | --

int main() {

    int a = 99;                 // type valido     | name valido     | non verrà usata
    int b = 100;                // type valido     | name valido     | verrà usata
    int c = 101;                // type valido     | name valido     | verrà usata

    a = 2*b;
    printf("%d\n", c);

    return 0;
    
}



