/*Semafory - synchronizacja.
Rozwiązać zadanie synchronizacji trzech procesów przy pomocy semaforów:
P1: t11, t12
P2: t21, t22, t23
P3: t31, t32, t33
Kolejność czasowa zdefiniowana jest zbiorem par G={(t21,t31),(t32,t22),(t22,t11),(t12,t23),(t23,t33)}.
Jeżeli para (a,b) należy do zbioru, to akcja b musi być poprzedzona akcją a.
Każdy z tasków tij(dla i,j=1,2,3) wyprowadza na ekran ciąg znaków:
Sekcja tij procesu o PID=tu podaje PID procesu, oraz powyższy ciąg znaków zapisuje w pliku o nazwie
spr1.txt. Każdy z tasków kończy się funkcją: sleep(1);
Napisz program do generowania procesów P1, P2, P3 w oparciu o funkcje fork(), exec(). W programie tym
należy utworzyć oraz usunąć (po zakończeniu procesów P1, P2, P3) zbiór semaforów uŜyty do synchronizacji. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/errno.h>
key_t key;
int semaphore;
int x;
pid_t w;

void end(char text[]) {
  perror(text);
  exit(EXIT_FAILURE);
}

static void createNewSem(key_t key) {
  if((semaphore = semget(key, 5, 0666 | IPC_CREAT)) == -1) end("[MAIN] Błąd przy tworzeniu nowego semafora!\n");
  printf("[MAIN] Semafor został utworzony: %d\n", semaphore);
}

static void setSem() {
  int arguments[5] = {0,0,0,0,0};
  if((semctl(semaphore, 0, SETALL, arguments)) == -1) end("[MAIN] Nie można ustawić semafora!\n");
  printf("[MAIN] Semafor został ustawiony.\n");
}

static void createChilds() {
  char path[20], program[20];
  int i, id;
  for(i = 0; i < 3; i++) {
    sprintf(path, "./program1_%d", i + 1);
    sprintf(program, "program1_%d", i + 1);
    id = fork();
    switch(id) {
        case -1:
            end("[MAIN] Błąd przy tworzeniu potomka");
            break;
        case 0: //Proces potomny
            if((execl(path, program, NULL)) == -1) end("[MAIN] Błąd przy execl!\n");
            break;
        default: //Proces macierzysty
            sleep(2);
            break;
    }
  }
}

static void waitSem() {
  int i;
  for (i = 0; i < 3; i++) {
      w = wait(&x);
      if (w == -1) {
        end("[MAIN] Wait error");
      } else
        printf("PID potomka: %d, Kod powrotu = %d\n", w, WIFEXITED(x));
  }
}

static void deleteSem() {
  if((semctl(semaphore, 5, IPC_RMID)) == -1) end("[MAIN] Nie można usunąć semafora!\n");
  printf("[MAIN] Semafor został usunięty.\n");
}

int main() {
  FILE* file = fopen("spr1.txt", "w");
  fclose(file);
  if((key = ftok(".", 'T')) == -1) end("[MAIN] Błąd tworzenia klucza!\n");
  createNewSem(key);
  setSem();
  createChilds();
  waitSem();
  deleteSem();
  exit(0);
}
