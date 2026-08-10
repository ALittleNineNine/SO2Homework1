#include <stdio.h>
#include <string.h>

/* __________SEZIONE TYPEDEF__________ */

typedef int Intero;
typedef float Reale;
typedef char Carattere;
typedef unsigned int InteroSenzaSegno;
typedef long InteroLungo;

/* __________SEZIONE TYPEDEF CON STRUCT__________ */

typedef struct {
    int x;
    int y;
} Punto;                    // typedef struct senza nome

typedef struct Retta {
    Punto inizio;
    Punto fine;
} Retta;                    // typedef struct con nome

/* __________SEZIONE VARIABILI GLOBALI (TIPI STANDARD)__________ */

int contatore;                          // type valido     | name valido     | non verrà usata
float 1numeroIniz;                      // type valido     | name non valido | --
double _underscoreIniz;                 // type valido     | name valido     | non verrà usata
char carattere;                         // type valido     | name valido     | non verrà usata
long contiene-trattino;                 // type valido     | name non valido | --
short interoCorto;                      // type valido     | name valido     | non verrà usata
unsigned int positivo;                  // type valido     | name valido     | non verrà usata
byte byteNonEsiste;                     // type non valido | name valido     | --
floatt floattNonEsiste;                 // type non valido | name valido     | --
int 123numeriIniz;                      // type valido     | name non valido | --
int at@nonValido;                       // type valido     | name non valido | --
tipoErrato 2doppioErrore;               // type non valido | name non valido | --

/* __________SEZIONE VARIABILI GLOBALI (TIPI UTENTE)__________ */

Intero mioIntero;                       // type valido     | name valido     | non verrà usata
Punto mioPunto;                         // type valido     | name valido     | non verrà usata
Retta miaRetta;                         // type valido     | name valido     | verrà usata
InteroSenzaSegno mioPositivo;           // type valido     | name valido     | non verrà usata

/* __________SEZIONE VARIABILI GLOBALI (TIPI ARRAY E PUNTATORI)__________ */

int vettoreIntero[5];                   // type valido     | name valido     | non verrà usata
char *puntatoreCarattere;               // type valido     | name valido     | verrà usata
Punto *** **** ** puntatoreMioPunto;    // type valido     | name valido     | non verrà usata
int matriceIntero[3] [3];               // type valido     | name valido     | non verrà usata
int array1[9], array2[9];               // type valido     | name valido     | non verrà usata
int *puntatore1, ***puntatore2;         // type valido     | name valido     | non verrà usata
float *2numeroIniz;                     // type valido     | name non valido | --
unsignedd int arrTipoErrato [3];        // type non valido | name valido     | --

/* __________SEZIONE VARIABILI GLOBALI (DICHIARAZIONE CON SPAZI VUOTI)__________ */
   
int	            varConTab   ;			// type valido     | name valido     | non verrà usata
int        varConSpaziMultipli;         // type valido     | name valido     | non verrà usata
int ;                                   // type valido     | name non valido | --

/* __________SEZIONE VARIABILI GLOBALI (TIPI ULTRA MEGA LUNGHI)__________ */

const volatile unsigned const long const long volatile int volatile const const volatile ultraMegaTipo;   // type valido     | name valido     | non verrà usata
const volatile unsigned const long const long float tipoIllegale;                                         // type non valido | name valido     | --
 
/* __________SEZIONE VARIABILI GLOBALI (DICHIARAZIONE E INIZIALIZZAZIONE)__________ */

int interoConInizializ = 100;           // type valido     | name valido     | non verrà usata
Intero mioInteroConInizializ=99;        // type valido     | name valido     | non verrà usata
float 4erroreConInit = 3.14;            // type valido     | name non valido | --
byte doppio$errore = 1;                 // type non valido | name non valido | --

/* __________SEZIONE MAIN__________ */

int main() {
 
    /* __________SEZIONE VARIABILI LOCALI (TIPI STANDARD)__________ */

    int a;                              // type valido     | name valido     | verrà usata
    int b;                              // type valido     | name valido     | verrà usata
    float risultato;                    // type valido     | name valido     | verrà usata
    char c;                             // type valido     | name valido     | verrà usata
    int interoNonUsato;                 // type valido     | name valido     | non verrà usata
    double 3numeroIniz;                 // type valido     | name non valido | --
    tipoErrato nome!errato;             // type non valido | name non valido | --
    unsigned int positivo2;             // type valido     | name valido     | verrà usata
    int i;                              // type valido     | name valido     | verrà usata
    long long while;                    // type valido     | name non valido | --
    float double doppioTipoBase;        // type non valido | name valido     | --
    
    /* __________SEZIONE VARIABILI LOCALI (TIPI UTENTE)__________ */
    
    Intero MioInteroLocale;             // type valido     | name valido     | non verrà usata
    InteroLungo mioLungoLocale;         // type valido     | name valido     | non verrà usata
    Retta miaRettaLocale;               // type valido     | name valido     | non verrà usata
    
    /* __________SEZIONE VARIABILI LOCALI (TIPI ARRAY E PUNTATORI)__________ */

    int arrayLocale[5];                 // type valido     | name valido     | verrà usata
    int *puntatoreLocale;               // type valido     | name valido     | verrà usata
    char stringLocale [20];             // type valido     | name valido     | verrà usata
    float matriceLocale[3][3];          // type valido     | name valido     | non verrà usata
    int contatoreLocale;                // type valido     | name valido     | verrà usata

    /* __________SEZIONE VARIABILI LOCALI (DICHIARAZIONE E INIZIALIZZAZIONE)__________ */
    
    int x = 5;                                  // type valido     | name valido     | verrà usata
    int y = 10, w = 20;                         // type valido     | name valido     | verrà usata
    int *puntatoreConInizializz = &x;           // type valido     | name valido     | verrà usata
    int arrayConInizializz[3] = {1, 2, 3};      // type valido     | name valido     | non verrà usata
 
    /* __________SEZIONE CORPO MAIN__________ */

    /* __________SEZIONE OPERAZIONI GENERALI__________ */

    a = 5;
    b = 10;
    risultato = a + b;
    c = 'x';

    positivo2 = 7;
    positivo = positivo2 + b;

    _underscoreIniz = risultato + a;

    MioInteroLocale = 99;
    mioLungoLocale = 999999;

    mioPositivo = positivo2;

    /* __________SEZIONE UTILIZZO STRUCT E PUNTATORI__________ */

    miaRetta.inizio.x = 0;
    miaRetta.inizio.y = 0;
    miaRetta.fine.x = 10;
    miaRetta.fine.y = 10;
    miaRetta.inizio.x += miaRetta.fine.y;

    puntatoreLocale = &a;
    *puntatoreLocale = *puntatoreLocale + 1;

    puntatoreCarattere = &c;
    *puntatoreCarattere = 'y';

    /* __________SEZIONE UTILIZZO FOR__________ */

    for (i = 0; i < 5; i++) {
        arrayLocale[i] = i * 2;
        vettoreIntero[i] = i;
    }

    varConTab = arrayLocale[0];

    /* __________SEZIONE UTILIZZO WHILE__________ */
    
    contatoreLocale = 0;
    while (contatoreLocale < 5) {
        risultato = risultato + arrayLocale[contatoreLocale];
        contatoreLocale++;
    }

    /* __________SEZIONE UTILIZZO IF ELSE__________ */

    if (a > b) {
        c = 'M';
    } else if (a == b) {
        c = 'E';
    } else {
        c = 'm';
    }

    /* __________SEZIONE UTILIZZO DO WHILE__________ */

    do {
        contatoreLocale--;
    } while (contatoreLocale > 0);

    /* __________SEZIONE UTILIZZO SWITCH__________ */

    switch (c) {
        case 'M':
            positivo = 1;
            break;
        case 'm':
            positivo = 2;
            break;
        default:
            positivo = 0;
    }

    /* __________SEZIONE UTILIZZO STRINGA__________ */

    strcat(stringLocale, "test");

    /* __________SEZIONE UTILIZZO VARIABILI INIZIALIZZATI__________ */

    x = x + y + w;
    *puntatoreConInizializz = x;

    return 0;

}



