#include "header.h"

int main() {

printf("TEST RIMOZIONE COMMENTI\n\n");
    char test1[] = "int x; //this is a comment";
    printf("Test 1\n");
    printf("Prima: '%s'\n", test1);
    remove_comments(test1);
    printf("Dopo: '%s'\n\n", test1);
    char test2[] = "int x; /*comment*/ int y;";
    printf("Test 2\n");
    printf("Prima: '%s'\n", test2);
    remove_comments(test2);
    printf("Dopo: '%s'\n\n", test2);
    char test3[] = "int x; /*first comment*/ int y; //second comment";
    printf("Test 3\n");
    printf("Prima: '%s'\n", test3);
    remove_comments(test3);
    printf("Dopo: '%s'\n\n", test3);
    char test4[] = "int x; /*comment not closed";
    printf("Test 4\n");
    printf("Prima: '%s'\n", test4);
    remove_comments(test4);
    printf("Dopo: '%s'\n\n", test4);
    char test5[] = "int x;";
    printf("Test 5\n");
    printf("Prima: '%s'\n", test5);
    remove_comments(test5);
    printf("Dopo: '%s'\n\n", test5);
    char test6[] = "// solo un commento";
    printf("Test 6\n");
    printf("Prima: '%s'\n", test6);
    remove_comments(test6);
    printf("Dopo:'%s'\n\n", test6);

printf("TEST VERIFICA INPUT\n\n");

    // TEST 1: -i file.txt
    printf("TEST 1: -i file.txt\n");
    char *args1[] = {"programma", "-i", "file.txt"};
    main(3, args1);
    printf("\n");
    
    // TEST 2: -vi file.txt
    printf("TEST 2: -vi file.txt\n");
    char *args2[] = {"programma", "-vi", "file.txt"};
    main(3, args2);
    printf("\n");
    
    // TEST 3: -iv file.txt
    printf("TEST 3: -iv file.txt\n");
    char *args3[] = {"programma", "-iv", "file.txt"};
    main(3, args3);
    printf("\n");
    
    // TEST 4: -vio input.txt output.txt
    printf("TEST 4: -vio input.txt output.txt\n");
    char *args4[] = {"programma", "-vio", "input.txt", "output.txt"};
    main(4, args4);
    printf("\n");
    
    // TEST 5: -ivo input.txt output.txt
    printf("TEST 5: -ivo input.txt output.txt\n");
    char *args5[] = {"programma", "-ivo", "input.txt", "output.txt"};
    main(4, args5);
    printf("\n");
    
    // TEST 6: -voi input.txt output.txt
    printf("TEST 6: -voi input.txt output.txt\n");
    char *args6[] = {"programma", "-voi", "input.txt", "output.txt"};
    main(4, args6);
    printf("\n");
    
    // TEST 7: --in file.txt
    printf("TEST 7: --in file.txt\n");
    char *args7[] = {"programma", "--in", "file.txt"};
    main(3, args7);
    printf("\n");
    
    // TEST 8: -i (ERRORE)
    printf("TEST 8: -i (ERRORE - manca argomento)\n");
    char *args8[] = {"programma", "-i"};
    main(2, args8);
    printf("\n");
    
    // TEST 9: -v (ERRORE - manca input)
    printf("TEST 9: -v (ERRORE - manca input)\n");
    char *args9[] = {"programma", "-v"};
    main(2, args9);
    printf("\n");
    
    // TEST 10: file.txt senza opzione (ERRORE)
    printf("TEST 10: file.txt senza opzione (ERRORE)\n");
    char *args10[] = {"programma", "file.txt"};
    main(2, args10);
    printf("\n");

}