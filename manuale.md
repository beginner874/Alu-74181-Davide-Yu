# **Manuale Utente - Simulatore ALU 74181**

### Descrizione breve
Questo programma è un Simulatore di Unità Aritmetica e Logica (ALU) 74181. L'ALU è il "cervello" fondamentale di un computer, responsabile di tutte le operazioni matematiche (Aritmetica) e logiche (come AND, OR, NOT). Lo strumento è pensato principalmente per scopi didattici e per testare le funzioni logiche del chip 74181.

### Istruzioni di Avvio

1. Nella pagina del tale progetto premi il tasto "Code" per creare e attivare un nuovo ambiente di sviluppo virtuale (codespace).

2. Una volta che il terminale del codespace è pronto, scrivi il seguente comando per compilare il codice sorgente C e creare il programma eseguibile:
```
gcc main.c -o simulatore
```
3. Avvia l'eseguibile appena creato utilizzando il seguente comando:
```
./simulatore
```
4. All'avvio, ti verrà presentato il Menu Principale. Per selezionare un'opzione, digita il numero corrispondente e premi Invio.
```
============================
SIMULATORE ALU 74181
============================
[0] Esci
[1] ALU 74181 a 4 bit
[2] ALU 74181 a 4 bit (con clock) 
[3] Convertitore Binario → Decimale 
[4] Convertitore Decimale → Binario 
[5] ALU a 32 bit - 8x74181
[6] ALU a 32 bit - 8x74181 (con clock)
```

### Descrizione delle Funzioni del Simulatore

Il simulatore offre un'insieme di strumenti avanzati per l'analisi e calcoli logici. Le funzioni sono raggruppate in base alla loro utilità.

| Opzione | Nome della Funzione | Descrizione |
|---------|---------------------|--------------------------|
| 0 | Esci | Termina l'esecuzione del programma e chiude l'interfaccia a riga di comando |
| 1 | ALU 74181 a 4 bit | Esegue la simulazione di un singolo chip ALU 74181. L'utente deve fornire manualmente i due operandi A e B (a 4 bit) e tutti i segnali di controllo (Cn, M, S0-S3) per definire l'operazione logica o aritmetica da eseguire |
| 2 | ALU 74181 a 4 bit (con clock) | Esegue la stessa simulazione a 4 bit dell'opzione 1, ma introduce un ritardo fisso per simulare il tempo di elaborazione del circuito, utile per studi sul timing del sistema |
| 3 | Convertitore Binario → Decimale | Converte un numero scritto in codice binario (composto solo da 0 e 1, come 1101) nel suo equivalente nel sistema numerico standard |
| 4 | Convertitore Decimale → Binario | Converte un numero standard (decimale, es. 25) nel suo equivalente in codice binario (es. 11001) |
| 5 | ALU a 32 bit - 8x74181 | Simula una catena di 8 chip 74181 interconnessi per creare un'unità logica e aritmetica più potente, capace di eseguire operazioni su numeri più grandi (32 bit). Richiede input binari a 32 bit e segnali di controllo |
| 6 | ALU a 32 bit - 8x74181 (con clock) | Esegue la stessa simulazione a 32 bit dell'opzione 5, ma con un ritardo di tempo fisso |

### Istruzioni Dettagliate per Opzione

##### 1. Operazioni Logiche (ALU 74181 - Singolo)

**Modalità di Input**
Dopo aver selezionato l'opzione [1], il programma richiederà la modalità di inserimento dei dati:
```Inserire dati manualmente? (S/N):```
Rispondere con la lettera S (Sì) per l'inserimento diretto da tastiera, oppure con N (No) per la lettura dei dati da un file predefinito. La scelta non è sensibile alla distinzione tra maiuscole e minuscole.

**Sezione A: Inserimento Manuale (S)**

L'utente deve fornire il valore 0 o 1 per ciascuna delle seguenti variabili, nell'ordine in cui vengono richieste dal programma:
- Cn - **Segnale di Carry-in**: Il bit di riporto (o prestito) in ingresso al circuito. Utilizzato principalmente nelle operazioni aritmetiche
- M - **Selettore di Modalità**: Determina la natura dell'operazione. 0 per operazioni Aritmetiche (calcoli); 1 per operazioni Logiche (confronti tra bit)
- S0-S3 - **Selettore di Operazione**: I 4 bit che, in combinazione con M e Cn, definiscono in modo univoco l'operazione da eseguire. [(Tabella di Riferimento Funzioni ALU)](#riferimento-funzioni-alu-opzioni-1-2-5-6)
- A0-A3, B0-B3 - **Operandi di Ingresso A e B (4 bit)**: I due numeri binari a 4 cifre su cui verrà eseguita l'operazion

**Output e Risultati**

Una volta completato l'inserimento, il programma esegue il calcolo e presenta i risultati nel terminale in un formato strutturato.

- F0-F3: Le 4 uscite del chip, che rappresentano il risultato finale a 4 bit dell'operazione eseguita.
- Cn+4 (Carry-out): Il bit di Riporto in Uscita, generato dall'operazione.
- A=B: Un segnale che indica se gli operandi di ingresso A e B sono uguali (1) o diversi (0).
- P (Propagate), G (Generate): Indicatori interni utilizzati per la gestione del riporto veloce.

Archiviazione: I risultati stampati nel terminale vengono anche salvati in un file denominato `risultati_alu_74181.txt` per successiva consultazione.
```
╔═══════════════════════════════════════════════╗
║           RISULTATI ALU 74181                 ║
╠═══════════════════════════════════════════════╣
║                                               ║
║  - F0      = ...                              ║
║  - F1      = ...                              ║
║  - A = B   = ...                              ║
║  - F2      = ...                              ║
║  - F3      = ...                              ║
║  - P       = ...                              ║
║  - Cn + 4  = ...                              ║
║  - G       = ...                              ║
║                                               ║
╚═══════════════════════════════════════════════╝
```
dove 
- F0-F3: Le 4 uscite che forniscono il risultato a 4 bit dell'operazione aritmetica o logica eseguita.
- P: Indica che la colonna corrente propagherebbe un riporto se ricevesse un riporto in ingresso
- G: Indica che la colonna corrente genererà un riporto in uscita, indipendentemente dal riporto in ingresso
- A=B: Uscita che segnala se i due operandi di ingresso (A e B) sono uguali

Il programma anche generà un file `risultati_alu_74181.txt` con i risultati presentati nel terminale

**Sezione B: Inserimento tramite un file (N)**

Per utilizzare questa scelta serve il file `file input_alu.txt`. Se non c'è, lo generà il progranna.
Nel interno del file l'utente deve compilare i seguenti campi:
```
Cn: <0>
M: <0>
A0: <0>
B0: <0>
A1: <0>
B1: <0>
A2: <0>
B2: <0>
A3: <0>
B3: <0>
S0: <0>
S1: <0>
S2: <0>
S3: <0>
```
Dopo aver compilato il file, l'utente puo riavviare l'operazione sceglienedo Opzione 1 e inserimento tramite file (`N`), e i risultati saranno presentati sia nel menu, sia nel file `risultati_alu_74181.txt`

### 2. ALU 74181 a 4 bit (con clock)
Questa opzione è identica all'opzione `[1] ALU 74181 a 4 bit` per quanto riguarda l'input dei dati (manuale o da file) e l'output dei risultati.

Funzione Clock: L'unica differenza è l'introduzione di un ritardo fisso (delay) per simulare l'intervallo di tempo necessario al circuito logico per stabilizzare il risultato. Questo è utile per la verifica teorica del timing e della propagazione del segnale. Dopo il calcolo e prima di mostrare i risultati, verrà inserita una pausa (ritardo).

### 3. Convertitore Binario → Decimale
Questa funzione converte un numero binario in notazione decimale standard.

**Input Richiesto:**
1. Modalità di Input: Simile all'opzione [1], il programma chiederà se si desidera inserire i dati manualmente (S) 
```
>> Inserisci un numero binario: 
```
o leggerli da un file `input_bin.txt`(N)
```
Numero Binario: <0>
```
2. Valore Binario: L'utente deve inserire una sequenza di cifre composta solo da 0 e 1

**Output e Risultati:**
Il programma eseguirà la conversione tramite la funzione BIN_DEC_DECODER  e visualizzerà il risultato decimale. Il risultato viene anche salvato nel file `risultati_dec.txt`
```
╔═════════════════════════════════════════════╗
║          RISULTATI CONVERTITORE             ║
╚═════════════════════════════════════════════╝
Risultato      = <...>
```

### 4. Convertitore Decimale → Binario
Questa funzione converte un numero decimale in notazione binaria.

**Input Richiesto:**
1. Modalità di Input: Simile all'opzione [1], il programma chiederà se si desidera inserire i dati manualmente (S) 
```
>> Inserisci un numero decimale:  
```
o leggerli da un file `input_dec.txt` (N).
```
Numero Decimale: <0>
```
2. Valore Decimale: L'utente deve inserire un numero intero positivo.

**Output e Risultati:**
Il programma eseguirà la conversione tramite la funzione DEC_BIN_CODER  e visualizzerà il risultato binario. Il risultato viene anche salvato nel file `risultati_bin.txt`
```
╔═════════════════════════════════════════════╗
║          RISULTATI CONVERTITORE             ║
╚═════════════════════════════════════════════╝
Risultato      = <...>
```

### 5. e 6. ALU a 32 bit (8x74181)
Queste opzioni simulano l'interconnessione di otto chip 74181 per creare un'unità logica e aritmetica a 32 bit, capace di gestire operandi più grandi. L'opzione [6] include un ritardo (con clock) come l'opzione [2].

**Input Richiesto:**
L'interfaccia di input è logicamente la stessa delle opzioni a 4 bit, ma gli operandi A e B sono di 32 bit ciascuno.
1. Segnali di Controllo (1-bit):
    - Cn (Carry-in), M (Modalità), S0-S3 (Selettore di Operazione) [(Tabella di Riferimento Funzioni ALU)](#riferimento-funzioni-alu-opzioni-1-2-5-6)

2. Operandi di Ingresso (32-bit):
    - A0-A31 (Operando A)
    - B0-B31 (Operando B)

**Modalità di Input:**

- Manuale (S): 
    1. L'utente inserisce gli operandi A e B in decimale.
    2. Successivamente, l'utente inserisce i 6 bit di controllo (Cn, M, S0-S3) come valori 0 o 1.
    ```
    >> Inserisci il primo operando (numero decimale a 32 bit): 
    >> Inserisci il secondo operando (numero decimale a 32 bit): 
    >> Cn:
    >> M:
    >> S0:
    >> S1:
    >> S2:
    >> S3:
    ```
- Da File (N): 
    1. Il programma cercherà/genererà un file denominato `input_alu32.txt`
    ```
    Operando A: <0>
    Operando B: <0>
    Cn: <0>
    M: <0>
    S0: <0>
    S1: <0>
    S2: <0>
    S3: <0>
    ```
    2. Questo file dovrà contenere i 6 segnali di controllo e i due operandi A e B. I segnali di controllo devono essere specificati come 0 o 1, mentre gli operandi A e B devono essere specificati in formato decimale.

**Output e Risultati:**

Il risultato in uscita F (l'uscita a 32 bit) sarà presentato in formato decimale. Inoltre verra salvato nel file `risultati_alu32.txt`

```
╔═════════════════════════════════════════════╗
║           RISULTATI ALU 32bit               ║
╚═════════════════════════════════════════════╝
- Risultato      = <...>
```


### Riferimento Funzioni ALU (Opzioni 1, 2, 5, 6)
La seguente tabella mostra come i segnali di controllo M, Cn e S0-S3 selezionano l'operazione eseguita.

| M | S3 | S2 | S1 | S0 | Cn | Operazione | 
|---|----|----|----|----|----|------------|
| 1 | 0  | 0  | 0  | 0  | X  | F = NOT(A)|
| 1 | 0  | 0  | 0  | 1  | X  | F = NOT(A + B) |
| 1 | 0  | 0  | 1  | 0  | X  | F = NOT(A)*B |
| 1 | 0  | 0  | 1  | 1  | X  | F = 0 |
| 1 | 0  | 1  | 0  | 0  | X  | F = NOT(AB) |
| 1 | 0  | 1  | 0  | 1  | X  | F = NOT(B) |
| 1 | 0  | 1  | 1  | 0  | X  | F = A ⊕ B |
| 1 | 0  | 1  | 1  | 1  | X  | F = A*NOT(B) |
| 1 | 1  | 0  | 0  | 0  | X  | F = NOT(A)+B |
| 1 | 1  | 0  | 0  | 1  | X  | F = NOT(A ⊕ B) |
| 1 | 1  | 0  | 1  | 0  | X  | F = B |
| 1 | 1  | 0  | 1  | 1  | X  | F = AB |
| 1 | 1  | 1  | 0  | 0  | X  | F = 1 |
| 1 | 1  | 1  | 0  | 1  | X  | F = A + NOT(B) |
| 1 | 1  | 1  | 1  | 0  | X  | F = A + B |
| 1 | 1  | 1  | 1  | 1  | X  | F = A |
| 0 | 0  | 0  | 0  | 0  | 0  | F = A PLUS 1 |
| 0 | 0  | 0  | 0  | 1  | 0  | F = (A + B) PLUS 1 |
| 0 | 0  | 0  | 1  | 0  | 0  | F = (A + NOT(B)) PLUS 1 |
| 0 | 0  | 0  | 1  | 1  | 0  | F = 0 |
| 0 | 0  | 1  | 0  | 0  | 0  | F = A PLUS (A*NOT(B)) PLUS 1 |
| 0 | 0  | 1  | 0  | 1  | 0  | F = (A+B) PLUS A*NOT(B) PLUS 1 |
| 0 | 0  | 1  | 1  | 0  | 0  | F = A MINUS B |
| 0 | 0  | 1  | 1  | 1  | 0  | F = A*NOT(B) |
| 0 | 1  | 0  | 0  | 0  | 0  | F = A PLUS AB PLUS 1 |
| 0 | 1  | 0  | 0  | 1  | 0  | F = A PLUS B PLUS 1 |
| 0 | 1  | 0  | 1  | 0  | 0  | F = (A+NOT(B)) PLUS AB PLUS 1 |
| 0 | 1  | 0  | 1  | 1  | 0  | F = AB |
| 0 | 1  | 1  | 0  | 0  | 0  | F = A PLUS A PLUS 1 |
| 0 | 1  | 1  | 0  | 1  | 0  | F = (A+B) PLUS A PLUS 1 |
| 0 | 1  | 1  | 1  | 0  | 0  | F = (A+NOT(B)) PLUS A PLUS 1 |
| 0 | 1  | 1  | 1  | 1  | 0  | F = A |
| 0 | 0  | 0  | 0  | 0  | 1  | F = A |
| 0 | 0  | 0  | 0  | 1  | 1  | F = A + B |
| 0 | 0  | 0  | 1  | 0  | 1  | F = A + NOT(B) |
| 0 | 0  | 0  | 1  | 1  | 1  | F = -1 |
| 0 | 0  | 1  | 0  | 0  | 1  | F = A PLUS A*NOT(B) |
| 0 | 0  | 1  | 0  | 1  | 1  | F = (A+B) PLUS A*NOT(B) |
| 0 | 0  | 1  | 1  | 0  | 1  | F = A MINUS B MINUS 1 |
| 0 | 0  | 1  | 1  | 1  | 1  | F = A*NOT(B) MINUS 1 |
| 0 | 1  | 0  | 0  | 0  | 1  | F = A PLUS AB |
| 0 | 1  | 0  | 0  | 1  | 1  | F = A PLUS B |
| 0 | 1  | 0  | 1  | 0  | 1  | F = (A+NOT(B)) PLUS AB|
| 0 | 1  | 0  | 1  | 1  | 1  | F = AB MINUS 1 |
| 0 | 1  | 1  | 0  | 0  | 1  | F = A PLUS A |
| 0 | 1  | 1  | 0  | 1  | 1  | F = (A+B) PLUS A |
| 0 | 1  | 1  | 1  | 0  | 1  | F = (A+NOT(B) PLUS A |
| 0 | 1  | 1  | 1  | 1  | 1  | F = A MINUS 1 |


> Nota: “X” indica valore indifferente 
> Nella tabella seguente, AND è indicato come prodotto, OR con il segno +, XOR con ⊕, e più e meno aritmetici utilizzando le parole PLUS e MINUS.
