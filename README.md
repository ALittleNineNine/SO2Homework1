# SO2Homework1

## 1. Architettura Principale

Creare dei file:
- *header.h* che contiene:
    - Le **librerie standard** utilizzate nel codice;
    - Le definizioni delle strutture **struct** (**punto 2**);
    - I **prototipi** delle funzioni che verranno implementate in un file.c;
- *funzioni.c* che contiene il **codice effettivo** delle funzioni;
- *main.c* che contiene il **main** (**punto 3**).

## 2. Strutture Dati Dinamiche

Non sapendo la quantità di dati, variabili, errori, creare delle **struct**:
- Per esempio: per ogni definizione di variabile, contiene nome_var, tipo_var, utilizzato(*boolean*), ecc.;
- Contiene anche un **puntatore** *\*next* per collegarla al nodo successivo (**Lista Concatenata**) (che nell'esempio di prima, corrisponde alla prossima dichiarazione di variabile).

Queste strutture struct serviranno per stampare le **statistiche** e gli **errori**.

## 3. Argomenti Riga Comandi

Per ricevere **input** e **opzioni** si usa la firma completa del main:
```C
int main(int argc, char *argv[])
```
- Tramite un ciclo scorrere l'array argv per ottenere le opzioni e gli eventuali parametri.
- Creare anche **controlli** per verificare se le opzioni e gli parametri sono messi correttamente.

## 4. Gestione I/O

La **lettura** e **scrittura** dei dati su disco deve essere controllata per evitare crash:
- La lettura si fa per la lettura del codice C in input;
- La scrittura si fa per il salvataggio delle statistiche e la visualizzazione a video in stdout.

È consigliato farlo come l'esempio nel PDF.

## 5. Lettura e Analisi del File

Questa è la fase centrale di questo progetto, consiste in:
- **Estrazione del testo**: leggere i file riga per riga usando ad esempio *fgets* e memorizzarli;
- **Spezzatura**: le righe vengono spezzate in parole (usando funzioni di libreria o anche manualmente usando cicli);
- **Pulizia**: Ogni parola dopo estratto viene ripulito da eventuali spazi, tabulazioni, '\n', ecc.

## 6. Validazione

Le parole devono essere passati attraverso una serie di **filtri** per capire se sono **validi**:
- Ad esempio se la parola supera la lunghezza massima, presenza di caratteri vietati o è una parola chiave del linguaggio;
- In base al risultato vengono creati dei nodi o modifiche alla struttura dinamica (**punto 2**).

## 7. Verifica di Stato

Mentre si prosegue l'analisi del testo, si confrontano i nuovi dati estratti con quelli già memorizzati:
- Se viene trovata una corrispondenza, si aggiorna lo stato dell'elemento salvato (modificando i campi del nodo corrispondente).
- Alla fine viene scansionata l'intera lista concatenata per estrarre informazioni statistici.

## 8. Output e Pulizia

Infine bisogna **presentare** i risultati e fare la **pulizia** della memoria:
- Bansando sui campi dei nodi nelle liste concatenate, il programma deve inviare il **report finale** (sullo schermo o in un file in base all'opzione);
- Prima che il main restituisca 0, è **OBBLIGATORIO** richiamare una **funzione di pulizia** per liberare esplicitamente la memoria allocata iterando le strutture dinamiche nodo per nodo.

## Riepilogo finale

Esempio di contenuti:
- *header.h*:
    - **Include**: *<stdio.h>*, *<stdlib.h>*, *<string.h>*, *<stdbool.h>*;
    - **Strutture Dati**: *typedef struct* per variabili, errori e statistiche;
    - **Prototipi**: le firme delle funzioni da implementare;
- *funzioni.c*:
    - **Include**: *"header.h"*;
    - **Corpo delle funzioni**: quelle dichiarate nel header;
- *main.c*:
    - **Include**: *"header.h"*;
    - **Funzione main**: *int main(int argc, char \*argv[])*;
    - **Ottenimento Argomenti**: usare un ciclo su *argv* per catturare le opzioni e gli argomenti;
    - **Chiamate funzioni**: tutte le chiamate delle funzioni scritte in *funzioni.c*.



