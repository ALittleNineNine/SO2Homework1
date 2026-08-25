# Esempi di test

## File input usati:
- `test_input_corto.c`: test file con pochi casi semplici, usato per l'introduzione del codice;
- `test_input_lungo.c`: test file abbondante con tantissimi casi prevedibili, per verificare la robustezza del codice;
- `test_input_vuoto.c`: test file da 0 byte, tutto vuoto;
- `test_input_spazi.c`: test file contenente solo spazi (`' '`, `'\t'`, `'\n'`).

## Compilazione:
```bash
gcc -Wall main.c functions.c -o myPrecompiler
```

## Esempi d'uso:
```bash
./myPrecompiler -i test_input_corto.c
./myPrecompiler -vi test_input_corto.c
```
- Elabora il file `test_input_corto.c` e stampa su `stdout`.

<br>

```bash
./myPrecompiler -i test_input_corto.c -o output.txt
```
- Elabora il file `test_input_corto.c` e stampa su `output.txt`.

<br>

```bash
./myPrecompiler -i test_input_corto.c -o output.txt -v
./myPrecompiler --in test_input_corto.c --out output.txt --verbose
```
- Elabora il file `test_input_corto.c` e stampa sia su `output.txt` che su `stdout`.

<br>

```bash
./myPrecompiler -i test_input_corto.c test_input_lungo.c -o output.txt
```
- Elabora i file `test_input_corto.c` e `test_input_lungo.c` separatamente e stampa su `output.txt`, con indicazione su intestazione il nome del file.

<br>

```bash
./myPrecompiler -io test_input_corto.c test_input_lungo.c output.txt
```
- **Notazione raggruppata** con `i` che precede `o`, tutti gli argomenti tranne l'ultimo sono file input, l'ultimo è file output;
- **Equivalente** al comando precedente.

<br>

```bash
./myPrecompiler -oi output.txt test_input_corto.c test_input_lungo.c
```
- **Notazione raggruppata** con `o` che precede `i`, il primo argomento è file output, tutti quelli dopo sono file input;
- **Equivalente** al comando precedente.

### Nota:
- L'opzione `v` può stare dove vuole, non influisce le opzioni `i` e `o`;
- Quindi `-vio`, `-ivo`, `-iov` hanno lo stesso significato;
- E anche `-voi`, `-ovi`, `-oiv` hanno lo stesso significato.

## Esempi errori attesi:
```bash
./myPrecompiler
```
- **Errore**: nessun parametro specificato.

<br>

```bash
./myPrecompiler -i
```
- **Errore**: `-i` necessario un argomento.

<br>

```bash
./myPrecompiler -i test_input_corto.c -o
```
- **Errore**: `-o` necessario un argomento.

<br>

```bash
./myPrecompiler -z test_input_corto.c
```
- **Errore**: opzione errata `-z`.

<br>

```bash
./myPrecompiler -i file_non_esiste.c
```
- **Errore**: impossibile aprire il file `'file_non_esiste.c'`.

<br>

```bash
./myPrecompiler -i test_input_vuoto.c
```
- **Errore**: il file `'test_input_vuoto.c'` è vuoto.

### Casi particolari:
```bash
./myPrecompiler -i test_input_spazi.c
```
- Abbiamo deciso di **non** trattarlo come errore file vuoto, poiché ha qualcosa all'interno (spazi vuoti);
- In questo caso l'elaborazione avverrà normalmente con statistica tutti `0`.

<br>

```bash
./myPrecompiler -i test_input_corto.c file_non_esiste.c test_input_lungo.c
```
- **Errore**: impossibile aprire il file `'file_non_esiste.c'`;
- **Attenzione**: quando esistono più file input, errori legati a un singolo file input fanno terminare solo l'elaborazione di quel file, non l'intero programma;
- Quindi il programma elabora i file input validi normalmente, quando incontra un file input con errore, stampa l'errore su `stderr` e si salta sul prossimo file.