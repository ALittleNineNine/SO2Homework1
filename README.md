# SO2Homework1

## 1. Architettura Principale

Creare dei file:
- *header.h* che contiene:
    - Le **librerie standard** utilizzate nel codice;
    - Le definizioni delle strutture **struct** (spiegate nel punto 2);
    - I **prototipi** delle funzioni che verranno implementate in un file.c;
- *funzioni.c* che contiene il **codice effettivo** delle funzioni;
- *main.c* che contiene il **main** (spiegate nel punto 3).

## 2. Strutture Dati Dinamiche

Non sapendo la quantità di dati, variabili, errori, creare delle **struct**
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

