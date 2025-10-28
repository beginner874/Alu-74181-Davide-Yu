#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#if defined(_WIN32)
#define SISTEMA_WINDOWS 1
#include <windows.h>
#else
#define SISTEMA_WINDOWS 0
#endif
#ifdef __APPLE__
#define SISTEMA_MAC 1
#else
#define SISTEMA_MAC 0
#endif
#ifdef __linux__
#define SISTEMA_LINUX 1
#else
#define SISTEMA_LINUX 0
#endif

static int leggi_bit_input_74181(const char* nome, int* var);
static int leggi_bit_input_32(const char* nome, int* var);



void ritardo_ns(long nanosecondi) {
#if SISTEMA_WINDOWS
    long ms = nanosecondi / 1000000L;
    if (ms <= 0) {
        if (nanosecondi > 0) {
            ms = 1; 
        }
    }
    Sleep((DWORD)ms);
#else
    struct timespec inizio, attuale;
    clock_gettime(CLOCK_MONOTONIC, &inizio);
    do {
        clock_gettime(CLOCK_MONOTONIC, &attuale);
    } while ((attuale.tv_sec - inizio.tv_sec) * 1000000000L + (attuale.tv_nsec - inizio.tv_nsec) < nanosecondi);
#endif
}
void delay(int milliseconds) {
    long ns = (long)milliseconds * 1000000L;
    ritardo_ns(ns);
}
void attendi_secondi(double secondi) {
    if (secondi <= 0.0) return;
    long ns = (long)(secondi * 1e9);
    ritardo_ns(ns);
}
void clock_step(int *CLK, int *prev_CLK, int milliseconds) {
    delay(milliseconds);
    *prev_CLK = *CLK;
    *CLK = 1 - *CLK;
}

int NAND3(int A, int B, int C) { 
  return 1 - (A * B * C); 
} 
void n_SR_FLIP_FLOP(int D, int S_reg, int R_reg, int CLK, int *prev_CLK, int *Q, int *Q_bar) { 
  if ((CLK * (1 - *prev_CLK)) == 1) { 
    int J = D; 
    int K = 1 - D; 
    *Q     = NAND3(S_reg, *Q_bar, NAND3(J, CLK, *Q_bar)); 
    *Q_bar = NAND3(R_reg, *Q,     NAND3(K, CLK, *Q)); 
  } 
  *prev_CLK = CLK; 
}
void n_PIPO74198(int D[8], int S_reg[8], int R_reg[8], int CLK, int prev_CLK[8], int Q[8], int Q_bar[8]) { 
  for (int i = 0; i < 8; i++) { 
    n_SR_FLIP_FLOP(D[i], S_reg[i], R_reg[i], CLK, &prev_CLK[i], &Q[i], &Q_bar[i]); 
  } 
}
void reg_PIPO32(int D[32], int S_reg[32], int R_reg[32], int CLK, int prev_CLK[32], int Q[32], int Q_bar[32]) { 
  for (int i = 0; i < 4; i++) { 
    n_PIPO74198(&D[i * 8], &S_reg[i * 8], &R_reg[i * 8], CLK, &prev_CLK[i * 8], &Q[i * 8], &Q_bar[i * 8]);
  } 
}

int BIN_DEC_DECODER(const char *binario) { 
  if (binario == NULL) { 
    printf("ERRORE: input NULL non valido.\n"); 
    return -1; 
  } 
  int decimale = 0; 
  int lunghezza = strlen(binario); 
  for (int i = 0; i < lunghezza; i++) { 
    if (binario[i] == '1') { 
      decimale = decimale * 2 + 1; 
    } 
    else if (binario[i] == '0') { 
      decimale = decimale * 2; 
    } 
    else { 
      printf("Input non valido. Solo 0 e 1 sono accettati.\n"); 
      return -1; 
    } 
  } 
  return decimale; 
}
char* DEC_BIN_CODER(int numero) { 
  static char binario[33]; 
  int i = 0; 
  if (numero == 0) { 
    strcpy(binario, "0"); 
    return binario; 
  } 
  while (numero > 0) { 
    binario[i++] = '0' + (numero % 2); 
    numero /= 2; 
  } 
  binario[i] = '\0'; 
  for (int j = 0; j < i / 2; j++) { 
    char temp = binario[j]; 
    binario[j] = binario[i - 1 - j]; 
    binario[i - 1 - j] = temp; 
  } 
  return binario; 
}

int porta_not(int a) { 
  return 1 - a; 
} 
int porta_and(int a, int b) { 
  return a * b; 
} 
int porta_or(int a, int b) { 
  return (a + b) - (a * b); 
} 
int porta_exor(int a, int b) { 
  return (a + b) - 2 * (a * b); 
} 
int porta_or_3(int a, int b, int c) { 
  int tmp = porta_or(a, b); 
  return porta_or(tmp, c); 
} 
int porta_or_4(int a, int b, int c, int d) { 
  int tmp1 = porta_or(a, b); 
  int tmp2 = porta_or(c, d); 
  return porta_or(tmp1, tmp2); 
} 
int porta_or_5(int a, int b, int c, int d, int e) { 
  int tmp = porta_or_4(a, b, c, d); 
  return porta_or(tmp, e); 
} 
int porta_and_3(int a, int b, int c) { 
  int tmp = porta_and(a, b); 
  return porta_and(tmp, c); 
} 
int porta_and_4(int a, int b, int c, int d) { 
  int tmp = porta_and(a, b); 
  int tmp2 = porta_and(c, d); 
  return porta_and(tmp, tmp2); 
} 
int porta_and_5(int a, int b, int c, int d, int e) { 
  int tmp = porta_and_3(a, b, c); 
  tmp = porta_and(tmp, d); 
  return porta_and(tmp, e); 
} 
int porta_exor_3(int a, int b, int c) { 
  int tmp = porta_exor(a, b); 
  return porta_exor(tmp, c); 
} 
int porta_exor_4(int a, int b, int c, int d) { 
  int tmp = porta_exor(a, b); 
  int tmp2 = porta_exor(c, d); 
  return porta_exor(tmp, tmp2); 
} 
int porta_exor_5(int a, int b, int c, int d, int e) { 
  int tmp = porta_exor_4(a, b, c, d); 
  return porta_exor(tmp, e); 
}

void pulire_buffer() {
    int c;
    while (porta_and(porta_not((c = getchar()) == '\n'),porta_not(c == EOF)));
}

void n_ALU74181(int Cn, int M, int A[4], int B[4], int S[4], int F[4], int *A_uguale_B, int *P, int *Cn_piu_4, int *G) { 
  F[0] = porta_exor(porta_not(porta_and(Cn, porta_not(M))), porta_and(porta_not(porta_not(porta_or_3(A[0], porta_and(B[0], S[0]), porta_and(S[1], porta_not(B[0]))))), porta_not(porta_or(porta_and_3(porta_not(B[0]), S[2], A[0]), porta_and_3(A[0], B[0], S[3]))))); 
  F[1] = porta_exor(porta_not(porta_or(porta_and(porta_not(M), porta_not(porta_or_3(A[0], porta_and(B[0], S[0]), porta_and(S[1], porta_not(B[0]))))), porta_and_3(porta_not(M), porta_not(porta_or(porta_and_3(porta_not(B[0]), S[2], A[0]), porta_and_3(A[0], B[0], S[3]))), Cn))), porta_and(porta_not(porta_not(porta_or_3(A[1], porta_and(B[1], S[0]), porta_and(S[1], porta_not(B[1]))))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1]))))); 
  F[2] = porta_exor(porta_not(porta_or_3(porta_and(porta_not(M), porta_not(porta_or_3(A[1], porta_and(B[1], S[0]), porta_and(S[1], porta_not(B[1]))))), porta_and_3(porta_not(M), porta_not(porta_or_3(A[0], porta_and(B[0], S[0]), porta_and(S[1], porta_not(B[0])))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1])))), porta_and_4(porta_not(M), Cn, porta_not(porta_or(porta_and_3(porta_not(B[0]), S[2], A[0]), porta_and_3(A[0], B[0], S[3]))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1])))))), porta_and(porta_not(porta_not(porta_or_3(A[2], porta_and(B[2], S[0]), porta_and(S[1], porta_not(B[2]))))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2]))))); 
  F[3] = porta_exor(porta_not(porta_or_4(porta_and(porta_not(M), porta_not(porta_or_3(A[2], porta_and(B[2], S[0]), porta_and(S[1], porta_not(B[2]))))), porta_and_3(porta_not(M), porta_not(porta_or_3(A[1], porta_and(B[1], S[0]), porta_and(S[1], porta_not(B[1])))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2])))), porta_and_4(porta_not(M), porta_not(porta_or_3(A[0], porta_and(B[0], S[0]), porta_and(S[1], porta_not(B[0])))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1]))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2])))), porta_and_5(porta_not(M), Cn, porta_not(porta_or(porta_and_3(porta_not(B[0]), S[2], A[0]), porta_and_3(A[0], B[0], S[3]))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1]))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2])))))), porta_and(porta_not(porta_not(porta_or_3(A[3], porta_and(B[3], S[0]), porta_and(S[1], porta_not(B[3]))))), porta_not(porta_or(porta_and_3(porta_not(B[3]), S[2], A[3]), porta_and_3(A[3], S[3], B[3]))))); 
  *A_uguale_B = porta_and_4(porta_exor(porta_not(porta_and(Cn, porta_not(M))), porta_and(porta_not(porta_not(porta_or_3(A[0], porta_and(B[0], S[0]), porta_and(S[1], porta_not(B[0]))))), porta_not(porta_or(porta_and_3(porta_not(B[0]), S[2], A[0]), porta_and_3(A[0], B[0], S[3]))))), porta_exor(porta_not(porta_or(porta_and(porta_not(M), porta_not(porta_or_3(A[0], porta_and(B[0], S[0]), porta_and(S[1], porta_not(B[0]))))), porta_and_3(porta_not(M), porta_not(porta_or(porta_and_3(porta_not(B[0]), S[2], A[0]), porta_and_3(A[0], B[0], S[3]))), Cn))), porta_and(porta_not(porta_not(porta_or_3(A[1], porta_and(B[1], S[0]), porta_and(S[1], porta_not(B[1]))))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1]))))), porta_exor(porta_not(porta_or_3(porta_and(porta_not(M), porta_not(porta_or_3(A[1], porta_and(B[1], S[0]), porta_and(S[1], porta_not(B[1]))))), porta_and_3(porta_not(M), porta_not(porta_or_3(A[0], porta_and(B[0], S[0]), porta_and(S[1], porta_not(B[0])))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1])))), porta_and_4(porta_not(M), Cn, porta_not(porta_or(porta_and_3(porta_not(B[0]), S[2], A[0]), porta_and_3(A[0], B[0], S[3]))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1])))))), porta_and(porta_not(porta_not(porta_or_3(A[2], porta_and(B[2], S[0]), porta_and(S[1], porta_not(B[2]))))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2]))))), porta_exor(porta_not(porta_or_4(porta_and(porta_not(M), porta_not(porta_or_3(A[2], porta_and(B[2], S[0]), porta_and(S[1], porta_not(B[2]))))), porta_and_3(porta_not(M), porta_not(porta_or_3(A[1], porta_and(B[1], S[0]), porta_and(S[1], porta_not(B[1])))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2])))), porta_and_4(porta_not(M), porta_not(porta_or_3(A[0], porta_and(B[0], S[0]), porta_and(S[1], porta_not(B[0])))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1]))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2])))), porta_and_5(porta_not(M), Cn, porta_not(porta_or(porta_and_3(porta_not(B[0]), S[2], A[0]), porta_and_3(A[0], B[0], S[3]))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1]))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2])))))), porta_and(porta_not(porta_not(porta_or_3(A[3], porta_and(B[3], S[0]), porta_and(S[1], porta_not(B[3]))))), porta_not(porta_or(porta_and_3(porta_not(B[3]), S[2], A[3]), porta_and_3(A[3], S[3], B[3])))))); 
  *P = porta_not(porta_and_4(porta_not(porta_or(porta_and_3(porta_not(B[0]), S[2], A[0]), porta_and_3(A[0], B[0], S[3]))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1]))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2]))), porta_not(porta_or(porta_and_3(porta_not(B[3]), S[2], A[3]), porta_and_3(A[3], S[3], B[3]))))); 
  *Cn_piu_4 = porta_or(porta_not(porta_not(porta_and_5(Cn, porta_not(porta_or(porta_and_3(porta_not(B[0]), S[2], A[0]), porta_and_3(A[0], B[0], S[3]))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1]))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2]))), porta_not(porta_or(porta_and_3(porta_not(B[3]), S[2], A[3]), porta_and_3(A[3], S[3], B[3])))))), porta_not(porta_not(porta_or_4(porta_and_4(porta_not(porta_or_3(A[0], porta_and(B[0], S[0]), porta_and(S[1], porta_not(B[0])))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1]))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2]))), porta_not(porta_or(porta_and_3(porta_not(B[3]), S[2], A[3]), porta_and_3(A[3], S[3], B[3])))), porta_and_3(porta_not(porta_or_3(A[1], porta_and(B[1], S[0]), porta_and(S[1], porta_not(B[1])))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2]))), porta_not(porta_or(porta_and_3(porta_not(B[3]), S[2], A[3]), porta_and_3(A[3], S[3], B[3])))), porta_and(porta_not(porta_or_3(A[2], porta_and(B[2], S[0]), porta_and(S[1], porta_not(B[2])))), porta_not(porta_or(porta_and_3(porta_not(B[3]), S[2], A[3]), porta_and_3(A[3], S[3], B[3])))), porta_not(porta_or_3(A[3], porta_and(B[3], S[0]), porta_and(S[1], porta_not(B[3])))))))); 
  *G = porta_not(porta_or_4(porta_and_4(porta_not(porta_or_3(A[0], porta_and(B[0], S[0]), porta_and(S[1], porta_not(B[0])))), porta_not(porta_or(porta_and_3(porta_not(B[1]), S[2], A[1]), porta_and_3(A[1], S[3], B[1]))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2]))), porta_not(porta_or(porta_and_3(porta_not(B[3]), S[2], A[3]), porta_and_3(A[3], S[3], B[3])))), porta_and_3(porta_not(porta_or_3(A[1], porta_and(B[1], S[0]), porta_and(S[1], porta_not(B[1])))), porta_not(porta_or(porta_and_3(porta_not(B[2]), S[2], A[2]), porta_and_3(A[2], S[3], B[2]))), porta_not(porta_or(porta_and_3(porta_not(B[3]), S[2], A[3]), porta_and_3(A[3], S[3], B[3])))), porta_and(porta_not(porta_or_3(A[2], porta_and(B[2], S[0]), porta_and(S[1], porta_not(B[2])))), porta_not(porta_or(porta_and_3(porta_not(B[3]), S[2], A[3]), porta_and_3(A[3], S[3], B[3])))), porta_not(porta_or_3(A[3], porta_and(B[3], S[0]), porta_and(S[1], porta_not(B[3])))))); 
}
static int leggi_bit_input_74181(const char* nome, int* var) {
    int input;
    int lettura_riuscita;
    int input_valido = 1;
    printf(">> %s: ", nome);
    lettura_riuscita = scanf("%d", &input);
    if (lettura_riuscita != 1) {
        input_valido = 0;
    }
    if (input_valido == 1) { 
        if (input != 0) {
            if (input != 1) {
                input_valido = 0;
            }
        }
    }
    if (porta_not(input_valido)) {
        printf("╔════════════════════════════════╗\n");
        printf("║            ERRORE              ║\n");
        printf("╠════════════════════════════════╣\n");
        printf("║                                ║\n");
        printf("║   %s deve essere 0 o 1      ║\n", nome);
        printf("║                                ║\n");
        printf("╚════════════════════════════════╝\n");
        pulire_buffer();
        return 0;
    }
    *var = input;
    return 1;
}
static int leggi_bit_input_32(const char* nome, int* var) {
    return leggi_bit_input_74181(nome, var);
}

void simula_alu_74181(int scelta_clock) {
    int Cn, M, A0, B0, A1, B1, A2, B2, A3, B3, S0, S1, S2, S3;
    char scelta[3];
    int input_valido = 1;

    printf("Inserire dati manualmente? (S/N): ");
    scanf("%2s", scelta);
    scelta[0] = toupper(scelta[0]);

    if (scelta[0] == 'S') {
        if (porta_not(leggi_bit_input_74181("Cn", &Cn))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("M", &M)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("A0", &A0)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("B0", &B0)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("A1", &A1)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("B1", &B1)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("A2", &A2)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("B2", &B2)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("A3", &A3)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("B3", &B3)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("S0", &S0)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("S1", &S1)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("S2", &S2)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_74181("S3", &S3)))) input_valido = 0;
        if (scelta_clock - 1) {attendi_secondi(0.5);}
    } else {
        FILE *file = fopen("input_alu.txt", "r");
        if (file == NULL) {
            file = fopen("input_alu.txt", "w");
            if (file == NULL) {
                printf("ERRORE: Impossibile creare il file input_alu.txt\n");
                input_valido = 0;
            } else {
                fprintf(file, "Cn: <0>\n");
                fprintf(file, "M: <0>\n");
                fprintf(file, "A0: <0>\n");
                fprintf(file, "B0: <0>\n");
                fprintf(file, "A1: <0>\n");
                fprintf(file, "B1: <0>\n");
                fprintf(file, "A2: <0>\n");
                fprintf(file, "B2: <0>\n");
                fprintf(file, "A3: <0>\n");
                fprintf(file, "B3: <0>\n");
                fprintf(file, "S0: <0>\n");
                fprintf(file, "S1: <0>\n");
                fprintf(file, "S2: <0>\n");
                fprintf(file, "S3: <0>\n");
                fclose(file);
                if (scelta_clock - 1) {attendi_secondi(0.5);}
                printf("Creato file input_alu.txt. Compilarlo e riavviare.\n");
                input_valido = 0;
            }
        } else {
            char line[100];
            #define LEGGI_BIT_FILE(var, nome) do { \
                char *result_fgets_macro; \
                result_fgets_macro = fgets(line, sizeof(line), file); \
                if (result_fgets_macro == NULL) { \
                    printf("ERRORE: Formato file incompleto (%s)\n", nome); \
                    input_valido = 0; \
                } \
                if (input_valido == 1) { \
                    if (sscanf(line, "%*[^<]<%d>", &(var)) != 1) { \
                        printf("ERRORE: Valore non valido in %s\n", nome); \
                        input_valido = 0; \
                    } \
                } \
                if (input_valido == 1) { \
                    if ((var) != 0) { \
                        if ((var) != 1) { \
                            printf("╔════════════════════════════════╗\n"); \
                            printf("║            ERRORE              ║\n"); \
                            printf("╠════════════════════════════════╣\n"); \
                            printf("║                                ║\n"); \
                            printf("║   %s deve essere 0 o 1      ║\n", nome); \
                            printf("║                                ║\n"); \
                            printf("╚════════════════════════════════╝\n"); \
                            input_valido = 0; \
                        } \
                    } \
                } \
            } while(0)

            LEGGI_BIT_FILE(Cn, "Cn");
            if (input_valido) LEGGI_BIT_FILE(M, "M");
            if (input_valido) LEGGI_BIT_FILE(A0, "A0");
            if (input_valido) LEGGI_BIT_FILE(B0, "B0");
            if (input_valido) LEGGI_BIT_FILE(A1, "A1");
            if (input_valido) LEGGI_BIT_FILE(B1, "B1");
            if (input_valido) LEGGI_BIT_FILE(A2, "A2");
            if (input_valido) LEGGI_BIT_FILE(B2, "B2");
            if (input_valido) LEGGI_BIT_FILE(A3, "A3");
            if (input_valido) LEGGI_BIT_FILE(B3, "B3");
            if (input_valido) LEGGI_BIT_FILE(S0, "S0");
            if (input_valido) LEGGI_BIT_FILE(S1, "S1");
            if (input_valido) LEGGI_BIT_FILE(S2, "S2");
            if (input_valido) LEGGI_BIT_FILE(S3, "S3");

            #undef LEGGI_BIT_FILE
            fclose(file);
        }
    }

    if (porta_not(input_valido)) {
        return;
    }
    if (scelta_clock - 1) {printf("Elaborazione in corso...\n");};
    int A[4] = {A0, A1, A2, A3};
    int B[4] = {B0, B1, B2, B3};
    int S_arr[4] = {S0, S1, S2, S3};
    int F[4], A_uguale_B, P, Cn_piu_4, G;
    n_ALU74181(Cn, M, A, B, S_arr, F, &A_uguale_B, &P, &Cn_piu_4, &G);
    if (scelta_clock - 1) {attendi_secondi(0.5);}

    printf("\n");
    printf("╔═════════════════════════════════════════════╗\n");
    printf("║           RISULTATI ALU 74181               ║\n");
    printf("╠═════════════════════════════════════════════╣\n");
    printf("║                                             ║\n");
    printf("║  - F0      = %-3d                            ║\n", F[0]);
    printf("║  - F1      = %-3d                            ║\n", F[1]);
    printf("║  - A = B   = %-3d                            ║\n", A_uguale_B);
    printf("║  - F2      = %-3d                            ║\n", F[2]);
    printf("║  - F3      = %-3d                            ║\n", F[3]);
    printf("║  - P       = %-3d                            ║\n", P);
    printf("║  - Cn + 4  = %-3d                            ║\n", Cn_piu_4);
    printf("║  - G       = %-3d                            ║\n", G);
    printf("║                                             ║\n");
    printf("╚═════════════════════════════════════════════╝\n");

    FILE *file_out = fopen("risultati_alu_74181.txt", "w");
    if (file_out) {
        fprintf(file_out, "╔═════════════════════════════════════════════╗\n");
        fprintf(file_out, "║           RISULTATI ALU 74181               ║\n");
        fprintf(file_out, "╠═════════════════════════════════════════════╣\n");
        fprintf(file_out, "║                                             ║\n");
        fprintf(file_out, "║  - F0      = %-3d                            ║\n", F[0]);
        fprintf(file_out, "║  - F1      = %-3d                            ║\n", F[1]);
        fprintf(file_out, "║  - A = B   = %-3d                            ║\n", A_uguale_B);
        fprintf(file_out, "║  - F2      = %-3d                            ║\n", F[2]);
        fprintf(file_out, "║  - F3      = %-3d                            ║\n", F[3]);
        fprintf(file_out, "║  - P       = %-3d                            ║\n", P);
        fprintf(file_out, "║  - Cn + 4  = %-3d                            ║\n", Cn_piu_4);
        fprintf(file_out, "║  - G       = %-3d                            ║\n", G);
        fprintf(file_out, "║                                             ║\n");
        fprintf(file_out, "╚═════════════════════════════════════════════╝\n");
        fclose(file_out);
    } else {
        printf("╔════════════════════════════════╗\n");
        printf("║            ERRORE              ║\n");
        printf("╠════════════════════════════════╣\n");
        printf("║                                ║\n");
        printf("║    Impossibile aprire file     ║\n");
        printf("║         di scrittura           ║\n");
        printf("║                                ║\n");
        printf("╚════════════════════════════════╝\n");
    }

    attendi_secondi(2.0);
}

void ALU32(int scelta_clock) {
    unsigned int operandoA = 0, operandoB = 0;
    int Cn = 0, M = 0, S[4] = {0};
    char scelta[3];
    int input_valido = 1;

    printf("Inserire dati manualmente? (S/N): ");
    scanf("%2s", scelta);
    scelta[0] = toupper(scelta[0]);

    if (scelta[0] == 'S') {
        printf(">> Inserisci il primo operando (numero decimale a 32 bit): ");
        if (scanf("%u", &operandoA) != 1) {
            printf("ERRORE: Input operando A non valido.\n");
            input_valido = 0;
        }
        if (porta_not(input_valido)) { return; }
        if (scelta_clock - 5) {attendi_secondi(0.5);}

        printf(">> Inserisci il secondo operando (numero decimale a 32 bit): ");
        if (scanf("%u", &operandoB) != 1) {
            printf("ERRORE: Input operando B non valido.\n");
            input_valido = 0;
        }
        if (porta_not(input_valido)) { return; }
        if (scelta_clock - 5) {attendi_secondi(0.5);}

        if (porta_not(leggi_bit_input_32("Cn", &Cn))) input_valido = 0;
        if (scelta_clock - 5) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_32("M", &M)))) input_valido = 0;
        if (scelta_clock - 5) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_32("S0", &S[0])))) input_valido = 0;
        if (scelta_clock - 5) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_32("S1", &S[1])))) input_valido = 0;
        if (scelta_clock - 5) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_32("S2", &S[2])))) input_valido = 0;
        if (scelta_clock - 5) {attendi_secondi(0.5);}
        if (porta_and(input_valido, porta_not(leggi_bit_input_32("S3", &S[3])))) input_valido = 0;
        if (scelta_clock - 5) {attendi_secondi(0.5);}
    } else {
        FILE *file = fopen("input_alu32.txt", "r");
        if (file == NULL) {
            file = fopen("input_alu32.txt", "w");
            if (file == NULL) {
                printf("ERRORE: Impossibile creare il file input_alu32.txt\n");
                input_valido = 0;
            } else {
                fprintf(file, "Operando A: <0>\n");
                fprintf(file, "Operando B: <0>\n");
                fprintf(file, "Cn: <0>\n");
                fprintf(file, "M: <0>\n");
                fprintf(file, "S0: <0>\n");
                fprintf(file, "S1: <0>\n");
                fprintf(file, "S2: <0>\n");
                fprintf(file, "S3: <0>\n");
                fclose(file);
                if (scelta_clock - 5) {attendi_secondi(0.5);}
                printf("Creato file input_alu32.txt. Compilarlo e riavviare.\n");
                input_valido = 0;
            }
        } else {
            char line[100];

            #define LEGGI_DA_FILE(var, nome, is_unsigned) do { \
                if (fgets(line, sizeof(line), file) == NULL) { \
                     printf("ERRORE: Formato file incompleto (%s)\n", nome); \
                     input_valido = 0; \
                 } else { \
                     if (is_unsigned) { \
                         unsigned int tmp_u; \
                         if (sscanf(line, "%*[^<]<%u>", &tmp_u) != 1) { \
                             printf("ERRORE: Valore non valido in %s\n", nome); \
                             input_valido = 0; \
                         } else { \
                             *(var) = tmp_u; \
                         } \
                     } else { \
                         int tmp_i; \
                         if (sscanf(line, "%*[^<]<%d>", &tmp_i) != 1) { \
                             printf("ERRORE: Valore non valido in %s\n", nome); \
                             input_valido = 0; \
                         } \
                         if (input_valido == 1) { \
                             if (tmp_i != 0) { \
                                 if (tmp_i != 1) { \
                                     printf("╔════════════════════════════════╗\n"); \
                                     printf("║            ERRORE              ║\n"); \
                                     printf("╠════════════════════════════════╣\n"); \
                                     printf("║                                ║\n"); \
                                     printf("║   %s deve essere 0 o 1      ║\n", nome); \
                                     printf("║                                ║\n"); \
                                     printf("╚════════════════════════════════╝\n"); \
                                     input_valido = 0; \
                                 } \
                             } \
                             if (input_valido == 1) { \
                                 *(var) = tmp_i; \
                             } \
                         } \
                     } \
                 } \
             } while(0)

            LEGGI_DA_FILE(&operandoA, "Operando A", 1);
            if (input_valido) LEGGI_DA_FILE(&operandoB, "Operando B", 1);
            if (input_valido) LEGGI_DA_FILE(&Cn, "Cn", 0);
            if (input_valido) LEGGI_DA_FILE(&M, "M", 0);
            if (input_valido) LEGGI_DA_FILE(&S[0], "S0", 0);
            if (input_valido) LEGGI_DA_FILE(&S[1], "S1", 0);
            if (input_valido) LEGGI_DA_FILE(&S[2], "S2", 0);
            if (input_valido) LEGGI_DA_FILE(&S[3], "S3", 0);

            #undef LEGGI_DA_FILE
            fclose(file);
        }
    }

    if (porta_not(input_valido)) { return; }
    if (scelta_clock - 5) {printf("Elaborazione in corso...\n");};
    int D_A[32], D_B[32], D_F[32];
    for (int i = 0; i < 32; i++) {
        D_A[i] = (operandoA >> i) & 1;
        D_B[i] = (operandoB >> i) & 1;
        D_F[i] = 0;
    }

    int Q_A[32] = {0}, Q_bar_A[32] = {0}, prev_CLK_A[32] = {0};
    int Q_B[32] = {0}, Q_bar_B[32] = {0}, prev_CLK_B[32] = {0};
    int Q_F[32] = {0}, Q_bar_F[32] = {0}, prev_CLK_F[32] = {0};
    int S_reg[32], R_reg[32];
    for (int i = 0; i < 32; i++) {
        S_reg[i] = 1;
        R_reg[i] = 1;
    }

    int CLK = 0, prev_CLK = 0;
    for (int i = 0; i < 4; i++) {
        clock_step(&CLK, &prev_CLK, 100);
        reg_PIPO32(D_A, S_reg, R_reg, CLK, prev_CLK_A, Q_A, Q_bar_A);
        reg_PIPO32(D_B, S_reg, R_reg, CLK, prev_CLK_B, Q_B, Q_bar_B);
    }
    if (scelta_clock - 5) {attendi_secondi(0.5);}

    int currentCn = Cn;
    int F[4], A_uguale_B, P, Cn_piu_4, G;
    for (int nibble = 0; nibble < 8; nibble++) {
        int Abits[4], Bbits[4];
        for (int bit = 0; bit < 4; bit++) {
            Abits[bit] = Q_A[nibble * 4 + bit];
            Bbits[bit] = Q_B[nibble * 4 + bit];
        }
        n_ALU74181(currentCn, M, Abits, Bbits, S, F, &A_uguale_B, &P, &Cn_piu_4, &G);
        for (int bit = 0; bit < 4; bit++) {
            D_F[nibble * 4 + bit] = F[bit];
        }
        currentCn = Cn_piu_4;
    }
    if (scelta_clock - 5) {attendi_secondi(0.5);}

    for (int i = 0; i < 4; i++) {
        clock_step(&CLK, &prev_CLK, 100);
        reg_PIPO32(D_F, S_reg, R_reg, CLK, prev_CLK_F, Q_F, Q_bar_F);
    }
    if (scelta_clock - 1) {attendi_secondi(0.5);}

    unsigned int result = 0; 
    for (int i = 0; i < 32; i++) {
        if (Q_F[i] == 1) { 
            result |= (1u << i);
        }
    }
    if (scelta_clock - 5) {attendi_secondi(0.5);}

    printf("\n");
    printf("╔═════════════════════════════════════════════╗\n");
    printf("║           RISULTATI ALU 32bit               ║\n");
    printf("╚═════════════════════════════════════════════╝\n");
    printf("- Risultato      = %u\n", result);

    FILE *file_out = fopen("risultati_alu32.txt", "w");
    if (file_out) {
        fprintf(file_out, "╔═════════════════════════════════════════════╗\n");
        fprintf(file_out, "║           RISULTATI ALU 32bit               ║\n");
        fprintf(file_out, "╚═════════════════════════════════════════════╝\n");
        fprintf(file_out, "- Risultato      = %u\n", result);
        fclose(file_out);
    } else {
        printf("╔════════════════════════════════╗\n");
        printf("║            ERRORE              ║\n");
        printf("╠════════════════════════════════╣\n");
        printf("║                                ║\n");
        printf("║    Impossibile aprire file     ║\n");
        printf("║         di scrittura           ║\n");
        printf("║                                ║\n");
        printf("╚════════════════════════════════╝\n");
    }
}


int main() {
    int scelta;
    char input[10];
    while (1) {   
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║                                                            ║\n");
        printf("║                    SIMULATORE ALU 74181                    ║\n");
        printf("║                                                            ║\n");
        printf("╠════════════════════════════════════════════════════════════╣\n");
        printf("║    0. Esci                                                 ║\n");
        printf("║    1. ALU 74181 a 4 bit                                    ║\n");
        printf("║    2. ALU 74181 a 4 bit (con clock)                        ║\n");
        printf("║    3. Convertitore Binario → Decimale                      ║\n");
        printf("║    4. Convertitore Decimale → Binario                      ║\n");
        printf("║    5. ALU a 32 bit - 8x74181                               ║\n");
        printf("║    6. ALU a 32 bit - 8x74181 (con clock)                   ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n");
        printf(">> Inserisci la tua scelta: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Errore di input.\n");
            continue;
        }
        if (input[0] == '\n') {
            continue;
        }
        if (input[0] == '\0') {
            continue;
        }
        int isValid = 1;
        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] == '\n') {
                break; 
            }
            if (isdigit((unsigned char)input[i]) == 0) {
                isValid = 0;
                break;
            }
        }
        if (porta_not(isValid)) {
            printf("\n╔════════════════════════════════╗\n");
            printf("║             ERRORE             ║\n");
            printf("╠════════════════════════════════╣\n");
            printf("║                                ║\n");
            printf("║   Inserisci un numero valido   ║\n");
            printf("║                                ║\n");
            printf("╚════════════════════════════════╝\n");
            attendi_secondi(3.0);
            continue;
        }
        scelta = atoi(input);
        if (scelta == 0) {
            printf("Uscita dal programma...\n");
            break;
        }
        else if (scelta == 1) {
            simula_alu_74181(scelta);
            pulire_buffer();
            attendi_secondi(3.0);
            continue;
        }
        else if (scelta == 2) {
            simula_alu_74181(scelta);
            pulire_buffer();
            attendi_secondi(3.0);
            continue;
        }
        else if (scelta == 3) {
            char bin[33];
            char risposta[3];
            printf("Inserire dati manualmente? (S/N): ");
            scanf("%2s", risposta);
            risposta[0] = toupper(risposta[0]);
            if (risposta[0] == 'S') {
                printf(">> Inserisci un numero binario: ");
                scanf("%32s", bin);
                int risultato = BIN_DEC_DECODER(bin);
                if (risultato != -1) {
                    printf("Risultato (decimale): %d\n", risultato);
                    pulire_buffer();
                    continue;
                }
            } else {
                FILE *file = fopen("input_bin.txt", "r");
                if (file == NULL) {
                    file = fopen("input_bin.txt", "w");
                    if (file == NULL) {
                        printf("ERRORE: Impossibile creare il file\n");
                        continue;
                    } else {
                        fprintf(file, "Numero Binario: <0>\n");
                        fclose(file);
                        printf("Creato file input_bin.txt. Compilarlo e riavviare.\n");
                        pulire_buffer();
                        continue;
                    }
                } else {
                    char line[100];
                    if (fgets(line, sizeof(line), file)) {
                        fclose(file);
                        if (sscanf(line, "%*[^<]<%32[^>]>", bin) == 1) {
                            int risultato = BIN_DEC_DECODER(bin);
                            FILE *file_out = fopen("risultati_dec.txt", "w");
                            if (file_out) {
                                fprintf(file_out, "╔═════════════════════════════════════════════╗\n");
                                fprintf(file_out, "║          RISULTATI CONVERTITORE             ║\n");
                                fprintf(file_out, "╚═════════════════════════════════════════════╝\n");
                                fprintf(file_out, "Risultato      = %-3d\n", risultato);
                                fclose(file_out);
                                pulire_buffer();
                                continue;
                            }
                        }
                    } else {
                        fclose(file);
                        printf("ERRORE: Formato file incompleto\n");
                        pulire_buffer();
                        continue;
                    }
                }
            }
            pulire_buffer();
            attendi_secondi(3.0);
        }
        else if (scelta == 4) {
            char risposta[3];
            int dec;
            printf("Inserire dati manualmente? (S/N): ");
            scanf("%2s", risposta);
            risposta[0] = toupper(risposta[0]);
            if (risposta[0] == 'S') {
                printf(">> Inserisci un numero decimale: ");
                scanf("%d", &dec);
                printf("Risultato (binario): %s\n", DEC_BIN_CODER(dec));
                pulire_buffer();
                continue;
            } else {
                FILE *file = fopen("input_dec.txt", "r");
                if (file == NULL) {
                    file = fopen("input_dec.txt", "w");
                    if (file == NULL) {
                        printf("ERRORE: Impossibile creare il file\n");
                        pulire_buffer();
                        continue;
                    } else {
                        fprintf(file, "Numero Decimale: <0>\n");
                        fclose(file);
                        printf("Creato file input_dec.txt. Compilarlo e riavviare.\n");
                        pulire_buffer();
                        continue;
                    }
                } else {
                    char line[100];
                    if (fgets(line, sizeof(line), file)) {
                        fclose(file);
                        char buffer[33];
                        if (sscanf(line, "%*[^<]<%32[^>]>", buffer) == 1) {
                            dec = atoi(buffer);
                            FILE *file_out = fopen("risultati_bin.txt", "w");
                            if (file_out) {
                                fprintf(file_out, "╔═════════════════════════════════════════════╗\n");
                                fprintf(file_out, "║          RISULTATI CONVERTITORE             ║\n");
                                fprintf(file_out, "╚═════════════════════════════════════════════╝\n");
                                fprintf(file_out, "Risultato      = %-16s\n", DEC_BIN_CODER(dec));
                                fclose(file_out);
                                pulire_buffer();
                                continue;
                            }
                        }
                    } else {
                        fclose(file);
                        printf("ERRORE: Formato file incompleto\n");
                        pulire_buffer();
                        continue;
                    }
                }
            }
            pulire_buffer();
            attendi_secondi(3.0);
        }
        else if (scelta == 5) {
            ALU32(scelta);
            pulire_buffer();
            attendi_secondi(3.0);
            continue;
        }
        else if (scelta == 6) {
            ALU32(scelta);
            pulire_buffer();
            attendi_secondi(3.0);
            continue;
        }
        else {
            printf("Scelta non valida!\n");
            attendi_secondi(3.0);
            continue;
        }
    }
    return 0;
}