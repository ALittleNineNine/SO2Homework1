main:
aggiunta gestione delle opzioni 

rimossa:
char new_line[] = "\n";

if (fp == NULL) {
        printf("Errore apertura file.\n");
        exit(1);
    }
modificato in:
if (fp == NULL) {
        printf("Errore apertura file.\n");
        return 1;
    }
motivo: exit(1) termina immediatamente il programma senza pulizia, return 1 invece esce dalla funzione main() e permette di gestire la pulizia automaticamente.

while (!feof(fp)) {
        row++;
        fgets(current_row, 1024, fp);
        if (strcmp(current_row, new_line))
modificato in:
while (fgets(current_row, 1024, fp) != NULL) {
        row++;
        if (current_row[0] != '\n' && current_row[0] != '\0')
motivo: feof() diventa true solo dopo aver tentato di leggere oltre la fine del file (può essere che elabora una riga vuota o ripete l'ultima);
        strcmp() confronta intera stringa e può non riconosce righe con spazi/tab, righe vuote all'inizio del file

processing_statistics *statistics = (processing_statistics *) malloc(sizeof(processing_statistics));
    get_processing_statistics(statistics, variables, errors);
    print_processing_statistics(statistics, variables, errors);
modificato in:
processing_statistics *statistics = (processing_statistics *) malloc(sizeof(processing_statistics));
    if (statistics != NULL) {
        statistics->var_count = 0;
        statistics->err_count = 0;
        statistics->var_unused_count = 0;
        statistics->wrong_var_name_count = 0;
        statistics->wrong_var_type_count = 0;
    }
    get_processing_statistics(statistics, variables, errors);
    print_processing_statistics(statistics, variables, errors);

print_processing_statistics(statistics, variables, errors); 
modificato in:
if (file_output != NULL) {
        FILE *f_out = fopen(file_output, "w");
        if (f_out != NULL) {
            print_processing_statistics(f_out, statistics, variables, errors);
            fclose(f_out);
        }
    } 

    if (verbose || file_output == NULL) {
        print_processing_statistics(stdout, statistics, variables, errors);
    }
motivo: in questo modo con presenza opzioni o si stampa su file, con o e v si stampa sia su stdout che su file



functions
aggiunta funzione che gestisce commenti 


void count_used_variables:
if (!strcmp(words[i+1], "[")) { 
modificato in:
if (words[i+1][0] != '\0' && !strcmp(words[i+1], "[")) {
motivo: se words[i+1] è '\0' strcmp() tenta di leggere stringa vuota e ciò causa segmentation fault


void analyze_row:
for (int i=0; row[i] != '\0'; i++) { 
modificato in:
for (int i=0; row[i] != '\0' && flag < 128; i++) {
motivo: se flag raggiunge 128 il ciclo continua causando scrittura fuori dai limiti dell'array words e quindi causa buffer overflow

if (current_char == '"') in_string = !in_string;    // gestione alternata quando si incontra "
        if (in_string) {
            words[flag][idx_char] = current_char;
            idx_char++;
            continue;
        }
modificato in:
if (current_char == '"') in_string = !in_string;    // gestione alternata quando si incontra "
        if (in_string) {
            if (idx_char < 127) {
                words[flag][idx_char] = current_char;
                idx_char++;
            } else {
                words[flag][127] = '\0';
                flag++;
                idx_char = 0;
                if (flag >= 128) break;
            }
            continue;
        }
motivo: se la stringa è più lunga di 127 caratteri, idx_char supera 127 causando errori -> scrittura fuori dai limiti dell'array 

if (current_char == '\'') in_char = !in_char;       // gestione alternata quando si incontra '
        if (in_char) {
            words[flag][idx_char] = current_char;
            idx_char++;
            continue;
        }
modificato in:
if (current_char == '\'') in_char = !in_char;       // gestione alternata quando si incontra '
        if (in_char) {
            if (idx_char < 127) {
                words[flag][idx_char] = current_char;
                idx_char++;
            } else {
                words[flag][127] = '\0';
                flag++;
                idx_char = 0;
                if (flag >= 128) break;
            }
            continue;
        }
motivo: stesso motivo sopra

for (int j=0; j<128; j++) {
                if (words[j][0] == '[') in_brace = true;
            }
modificato in:
for (int j=0; j<flag; j++) {
                if (words[j][0] == '[') {
                    in_brace = true;
                    break;
                }
            }
motivo: controlla solo le parole effettivamente presenti (flag), usa break per uscire appena trova [ e non controlla parole vuote non inizializzate

if (in_brace) {
            words[flag][idx_char] = current_char;
            if (current_char == '}') {
                words[flag][idx_char] = current_char;
                in_brace = false;
                words[flag][idx_char+1] = '\0';
                flag++;
                idx_char = 0;
            } else {
                idx_char++;
            }
            continue;
        }
modificato in:
if (in_brace) {
            if (idx_char < 127) {
                words[flag][idx_char] = current_char;
                idx_char++;
            }
            if (current_char == '}') {
                in_brace = false;
                if (idx_char < 127) {
                    words[flag][idx_char] = '\0';
                } else {
                    words[flag][127] = '\0';
                }
                flag++;
                idx_char = 0;
            } else {
                if (idx_char >= 127) {
                    words[flag][127] = '\0';
                    flag++;
                    idx_char = 0;
                    if (flag >= 128) break;
                }
            }
            continue;
        }
motivo: senza controllo idx_char scrive oltre il limite -> buffer overflow, senza controllo idx_char+1 potrebbe diventare 128

if (in_bracket) {
            words[flag][idx_char] = current_char;
            if (current_char == ']') {
                in_bracket = false;
                words[flag][idx_char+1] = '\0';
                flag++;
                idx_char = 0;
            } else {
                idx_char++;
            }
            continue;
        }
modificato in:
if (in_bracket) {
            if (idx_char < 127) {
                words[flag][idx_char] = current_char;
                idx_char++;
            }
            if (current_char == ']') {
                in_bracket = false;
                if (idx_char < 127) {
                    words[flag][idx_char] = '\0';
                } else {
                    words[flag][127] = '\0';
                }
                flag++;
                idx_char = 0;
            } else {
                if (idx_char >= 127) {
                    words[flag][127] = '\0';
                    flag++;
                    idx_char = 0;
                    if (flag >= 128) break;
                }
            }
            continue;
        }
motivo: stesso motivo sopra

words[flag][idx_char] = current_char;
            idx_char++;
modificato in:
if (idx_char < 127) {
                words[flag][idx_char] = current_char;
                idx_char++;
            } else {
                words[flag][127] = '\0';
                flag++;
                idx_char = 0;
                if (flag >= 128) break;
                if (idx_char < 127) {
                    words[flag][idx_char] = current_char;
                    idx_char++;
                }
            }
motivo: senza controllo -> potrebbe causare buffer overflow


newtype *add_newtype_struct:
aggiunta:
if (words[idx + 1][0] == '\0') {
        return newtypes;
    }

if (new_type == NULL) {
        return newtypes;
    }
motivo: se words[idx+1] vuoto strcpy() copierebbe una stringa vuota e malloc() potrebbe restituire NULL


variable *variables_management:
aggiunta di:
if (current_type[0] == '\0') return variables;
motivo: se tipo vuoto salta (se non viene aggiunta può essere che ad es. a = 5 viene considerate valida, con questa aggiunta non viene considerata né valida né errata)


void print_processing_statics:
if (!current_var->used) {
            printf("%s", current_var->name);
            for (int i=0; i < 32 - strlen(current_var->name); i++) printf(" ");
            printf("dichiarata in riga %d\n", current_var->row);
        }
modificato in:
if (!current_var->used) {
            printf("%s", current_var->name);
            int padding = 32 - (int)strlen(current_var->name);
            if (padding > 0) {
                for (int i = 0; i < padding; i++) printf(" ")
            }
            printf("dichiarata in riga %d\n", current_var->row);
        }
motivo: se strlen(current_var->name) > 32, 32 + strlen() diventa negativa -> loop infinito


void count_used_variables:
aggiunta:
if (words[i+1][0] == '\0') continue;
motivo: se word[i+1] non esiste salta iterazione

while (!strcmp(words[end_brackets_array], "[")) {
modificato in:
while (words[end_brackets_array][0] != '\0' && !strcmp(words[end_brackets_array], "[")) {
motivo: se words[end_brackets_array] non esiste, non esegue loop
    
while (strcmp(words[end_brackets_array], "]")) end_brackets_array++;
                        end_brackets_array++;
modificato in:
while (words[end_brackets_array][0] != '\0' && strcmp(words[end_brackets_array], "]")) {
                            end_brackets_array++;
                            if (words[end_brackets_array][0] == '\0') break;
                        }
aggiunta di:
if (words[end_brackets_array][0] == '\0') break;
                        end_brackets_array++;

if (!strcmp(words[end_brackets_array], "=")) break;
modificato in:
if (words[end_brackets_array][0] != '\0' && !strcmp(words[end_brackets_array], "=")) break;

if (!strcmp(words[i+1], "=") && strcmp(words[i+2], "=")) break;
modificato in:
if (words[i+1][0] != '\0' && !strcmp(words[i+1], "=") && words[i+2][0] != '\0' && strcmp(words[i+2], "=")) {
                    break;
                }
motivo: se words[i+2] non esiste non si fa strcmp()

void get_processing_statistics:
aggiunta di:
statistics->var_count = 0;
statistics->err_count = 0;
statistics->var_unused_count = 0;
statistics->wrong_var_name_count = 0;
statistics->wrong_var_type_count = 0;
motivo: quando si usa malloc() la memoria contiene valori scritti in precedenza e quindi contiene valori casuali, quindi si inizializza la struttura a 0 prima di usare


void print_processing_statistics(processing_statistics *statistics, variable *variables, error *errors) {:

    printf("\n---------- STATISTICHE DI ELABORAZIONE -----------\n\n");

    printf("Numero totale di variabili valide:\t\t%d\n", statistics->var_count);
    printf("Numero totale di errori rilevati:\t\t%d\n", statistics->wrong_var_type_count + 
                                                        statistics->wrong_var_name_count +
                                                        statistics->var_unused_count);
    printf("Numero di variabili non utilizzate:\t\t%d\n", statistics->var_unused_count);
    printf("Numero di nomi di variabili non corretti:\t%d\n", statistics->wrong_var_name_count);
    printf("Numero di tipi di dato non corretti:\t\t%d\n", statistics->wrong_var_type_count);

    printf("\n--------------------------------------------------\n");
    
    printf("\n--- ERRORI RILEVATI ---\n\n");
modificato tutti i print con fprintf e di conseguenza modificato anche su header con aggiunta di FILE *out




Risultato del fileinputprova.c:
Numero totale di variabili valide:		42 (type + name validi)
Numero totale di errori rilevati:		46 (type + name non validi + var non usate)
Numero di variabili non utilizzate:		26 
Numero di nomi di variabili non corretti:	12
Numero di tipi di dato non corretti:    8

Codice per osservare come analyze_row() ha splittato il testo dopo rimozione commenti (da inserire in main dopo analyze_row):
printf("Testo %d: ", row);
    for (int i = 0; words[i][0] != '\0' && i < 10; i++) {
        printf("%s ", words[i]);
    }
    printf("\n\n");
