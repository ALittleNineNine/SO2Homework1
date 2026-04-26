#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// struct lista concatenata per memorizzare informazioni sulle variabili
typedef struct variable {
    char type[512];
    char name[64];
    bool used;
    int row;
    struct variable *next;
} variable;

typedef struct error {
    bool wrong_type;
    bool wrong_name;
    int row;
    struct error *next;
} error;

// crea un nuovo nodo variabile e lo collega in testa alla lista variabili
variable *add_var(variable *next_var, char type[], char name[], int row);

// crea un nuovo nodo errore e lo collega in testa alla lista errori
error *add_error(error *next_err, int row);

// data una riga di codice, li spezza in al massimo in 64 parole
void analyze_row(char row[], char words[64][64]);

/*
    data un array di array di char contenente una riga di dichiarazione variabile, lo mantiene solo la parte type
    ritorna la lunghezza della parte type
*/
int get_type(char words[64][64], char type[64][64]);

// data un array di array di char contenente una riga di dichiarazione variabile, mantiene solo la parte name
void get_name(char words[64][64], char name[64][64], int start_idx);

// data una word, restituisce true se word è un tipo base
bool is_basic_type(char word[]);

// dato un array type, restituisce true se è un type
bool verify_type(char type[64][64]);

// data una word, restituisce true se word è una keyword del linguaggio C
bool is_keyword(char word[]);

/* 
    dato un array name, restituisce true se sono tutti nomi validi
    il nome eventualmente non valido viene sostituito inplacemente con "!valid"
    quindi se la funzione restituisce false, non significa automaticamente che non ci siano nomi validi
*/
bool verify_name(char name[64][64]);

// data una lista concatenata contenenti varaibili e un nome, restituisce true se il nome appartiene alla lista
bool existing_var(variable *variables, char name[]);

// trasforma un array in una stringa inplacemente
void array_to_string(char array[64][64], char string[]);

// calcola il numero di celle nelle due liste variables e errors
void linked_list_count(int *var_err_count, variable *variables, error *errors);

// ritorna true se in questa riga words contiene main
bool is_main(char words[64][64]);

// data la prima word di una riga, restituisce true se è finita la parte di dichiarazione variabile
bool end_variable_declaration(char word[]);








