#include "header.h"

int main(int argc, char *argv[]) {
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

    return 0;
}