#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// struct lista concatenata per memorizzare informazioni sulle variabili
typedef struct variable {
    char type[512];     // tipo della variabile
    char name[128];     // nome della variabile
    bool type_valid;    // true se il tipo è valido
    bool name_valid;    // true se il nome è valido
    bool used;          // true se la variabile viene usata         [se (type_valid || name_valid) == false, non si tiene conto di questa var]
    bool declared;      // true se la variabile è già dichiarata
    int row;            // numero di riga dove è dichiarata
    struct variable *next;  // puntatore al prossimo valore (lista concatenata)
} variable;

// struct lista concatenata per memorizzare informazioni sui tipi creati con typedef
typedef struct newtype {
    char type[512];         // nome del tuoi
    struct newtype *next;   // puntatore al prossimo tipo
} newtype;

// struct per memorizzare informazioni sulle statistiche di elaborazione
typedef struct {
    int var_count;              // numero totale di variabili valide
    int err_count;              // numero totale di errori
    int var_unused_count;       // variabili dichiarate ma non usate
    int wrong_var_name_count;   // variabili con nome non valido
    int wrong_var_type_count;   // variabili con tipo non valido
} processing_statistics;

// crea un nuovo nodo variabile e lo collega in testa alla lista variabili
variable *add_var(variable *next_var, char type[], char name[], int row);

// crea un nuovo nodo newtype e lo collega in testa alla lista newtype (riguardante typedef senza struct)
newtype *add_newtype_no_struct(newtype *newtypes, char **words);

// crea un nuovo nodo newtype e lo collega in testa alla lista newtype (riguardante typedef con struct)
newtype *add_newtype_struct(newtype *newtypes, char **words, int idx);

// analizza gli argomenti del main, ritorna 1 se c'è errore
int analyze_arguments(int argc, char *argv[], char **file_input, char **file_output, int *verbose);

// allocazione dinamica della memoria per le risorse usate per analizzare il file .c
int allocate_resources(char **current_row, char ***words, char ***type, char ***name, processing_statistics **statistics);

/*
    analizza tutte le variabili presenti e controlla se sono state usate
    le informazioni ottenute vengono salvate in variables e newtypes
*/
void analyze_file(FILE *fp, variable **variables, newtype **newtypes, char **current_row, char ***words, char ***type, char ***name, int *contenuto);

/*
    data una riga di codice, li spezza in al massimo in 64 parole:
    - se start_statement_section == true: le parole vengono spezzate in base anche a simboli speciali;
    - altrimenti: le parole vengono spezzate solo mediante ' ', '\t', '\n', ';', '=', '\0' e '*';
    - quando si incontrano '...' (char), "..." (array di char), [...] e {...} (dichiarazione e inizializzazione di array, verificato se lo è davvero)
        vengono uniti in un unica word.
*/
void analyze_row(char *row, char **words, bool start_statement_section);

// data una word, restituisce true se word è un tipo creato con typedef
bool is_newtype(char word[], newtype *newtypes);

/*
    data un array di array di char contenente una riga di dichiarazione variabile, lo mantiene solo la parte type
    ritorna la lunghezza della parte type
*/
int get_type(char **words, char **type);

// data un array di array di char contenente una riga di dichiarazione variabile, mantiene solo la parte name
void get_name(char **words, char **name, int start_idx);

// aggiungere la/le variabile/i con eventuali errori, ritorna la nuova testa della lista
variable *variables_management(variable *variables, newtype *newtypes, char **type, char **name, int row);

// data una word, restituisce true se word è un tipo base
bool is_basic_type(char word[]);

// dato un array type, restituisce true se è un type
bool verify_type(char **type, newtype *newtypes);

// data una word, restituisce true se word è una keyword del linguaggio C
bool is_keyword(char word[]);

// dato una stringa che rappresenta un nome, se il nome non è valido ritorna false
bool verify_name(char *name);

// data una lista concatenata contenenti varaibili e un nome, restituisce true se il nome appartiene alla lista
bool existing_var(variable *variables, char name[]);

// trasforma un array in una stringa inplacemente
void array_to_string(char **array, char string[]);

// date le liste concatenate variables e errors, li mette in ordine invertito
void reverse_linked_list(variable **variables);

// calcola la statistica di elaborazione
void get_processing_statistics(processing_statistics *statistics, variable *variables);

// printa la statistica di elaborazione
int print_processing_statistics(FILE *out, processing_statistics *statistics, variable *variables);

// ritorna true se in questa riga words contiene main
bool is_main(char **words);

// data la prima word di una riga, restituisce true se è finita la parte di dichiarazione variabile
bool end_variable_declaration(char word[], variable *variables);

// estrae le variabili usate e aggiorna nella lista concatenata variable->used = true
void count_used_variables(char **words, variable *variables);

// controlla il file output, se c'è errore lo stampa su stderr, e ritorna 1
int output_file_control(char **file_output, variable *variables, processing_statistics *statistics, int verbose);

// pulisce tutta la memoria allocata precedentemente
void free_all(variable *variables, newtype *newtypes, char **words, char **type, char **name, char *current_row, processing_statistics *statistics);

// TEST FOR IMPLEMENTATION
void test_array_of_array(char **words, char **type, char **name, int row);

// TEST FOR IMPLEMENTATION
void test_linked_lists(variable *variables, newtype *newtypes);

// mostra a utente compilazione corretta [from ananas]
void input();

// funzione per rimuovere commenti [from ananas]
char* remove_comments(char *line);



