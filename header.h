#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// struct lista concatenata per memorizzare informazioni sulle variabili
typedef struct variable {
    char type[512];
    char name[128];
    bool used;
    int row;
    struct variable *next;
} variable;

// struct lista concatenata per memorizzare informazioni sugli errori
typedef struct error {
    bool wrong_type;
    bool wrong_name;
    int row;
    struct error *next;
} error;

// struct lista concatenata per memorizzare informazioni sui tipi creati con typedef
typedef struct newtype {
    char type[512];
    struct newtype *next;
} newtype;

// struct per memorizzare informazioni sulle statistiche di elaborazione
typedef struct {
    int var_count;
    int err_count;
    int var_unused_count;
    int wrong_var_name_count;
    int wrong_var_type_count;
} processing_statistics;

// crea un nuovo nodo variabile e lo collega in testa alla lista variabili
variable *add_var(variable *next_var, char type[], char name[], int row);

// crea un nuovo nodo errore e lo collega in testa alla lista errori
error *add_error(error *next_err, int row);

// data una riga di codice, li spezza in al massimo in 64 parole
void analyze_row(char *row, char **words);

// crea un nuovo nodo newtype e lo collega in testa alla lista newtype (riguardante typedef senza struct)
newtype *add_newtype_no_struct(newtype *newtypes, char **words);

// crea un nuovo nodo newtype e lo collega in testa alla lista newtype (riguardante typedef con struct)
newtype *add_newtype_struct(newtype *newtypes, char **words, int idx);

// data una word, restituisce true se word è un tipo creato con typedef
bool is_newtype(char word[], newtype *newtypes);

/*
    data un array di array di char contenente una riga di dichiarazione variabile, lo mantiene solo la parte type
    ritorna la lunghezza della parte type
*/
int get_type(char **words, char **type);

// data un array di array di char contenente una riga di dichiarazione variabile, mantiene solo la parte name
void get_name(char **words, char **name, int start_idx);

// aggiungere la/le variabile/i se non ci sono errori, ritorna la nuova testa della lista
variable *variables_management(variable *variables, newtype *newtypes, char **type, char **name, int row, bool *flag);

// aggiungere l'errore se esiste, ritorna la nuova testa della lista
error *errors_management(error *errors, newtype *newtypes, char **type, char **name, int row, bool flag);

// data una word, restituisce true se word è un tipo base
bool is_basic_type(char word[]);

// dato un array type, restituisce true se è un type
bool verify_type(char **type, newtype *newtypes);

// data una word, restituisce true se word è una keyword del linguaggio C
bool is_keyword(char word[]);

/* 
    dato un array name, restituisce true se sono tutti nomi validi
    il nome eventualmente non valido viene sostituito inplacemente con "!valid"
    quindi se la funzione restituisce false, non significa automaticamente che non ci siano nomi validi
*/
bool verify_name(char **name);

// data una lista concatenata contenenti varaibili e un nome, restituisce true se il nome appartiene alla lista
bool existing_var(variable *variables, char name[]);

// trasforma un array in una stringa inplacemente
void array_to_string(char **array, char string[]);

// date le liste concatenate variables e errors, li mette in ordine invertito
void reverse_linked_list(variable **variables, error **errors);

// calcola la statistica di elaborazione
void get_processing_statistics(processing_statistics *statistics, variable *variables, error *errors);

// ritorna true se in questa riga words contiene main
bool is_main(char **words);

// data la prima word di una riga, restituisce true se è finita la parte di dichiarazione variabile
bool end_variable_declaration(char word[], newtype *newtypes);

// estrae le variabili usate e aggiorna nella lista concatenata variable->used = true
void count_used_variables(char **words, variable *variables);

// TEST FOR IMPLEMENTATION
void test_array_of_array(char **words, char **type, char **name, int row);

// TEST FOR IMPLEMENTATION
void test_linked_lists(variable *variables, error *errors, newtype *newtypes);

// funzione per rimuovere commenti [from ananas]
char* remove_comments(char *line);

//mostra a utente compilazione corretta [from ananas]
void input();









