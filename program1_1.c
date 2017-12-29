#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
key_t key;
int semaphore;

void end(char text[]) {
  perror(text);
  exit(EXIT_FAILURE);
}

static void semaphore_p(int x) {
  struct sembuf bufor_sem;
  bufor_sem.sem_num=x;
  bufor_sem.sem_op=-1;
  bufor_sem.sem_flg=SEM_UNDO;
  if ((semop(semaphore, &bufor_sem, 1)) == -1) end("[P1] Błąd przy zamykaniu semafora!\n");
  printf("[P1] Semafor %d zostal zamkniety.\n", x);
}

static void semaphore_v(int x) {
  struct sembuf bufor_sem;
  bufor_sem.sem_num=x;
  bufor_sem.sem_op=1;
  bufor_sem.sem_flg=SEM_UNDO;
  if ((semop(semaphore, &bufor_sem, 1)) == -1) end("[P1] Błąd przy otwieraniu semafora!\n");
  printf("[P1] Semafor %d zostal otwarty.\n", x);
}

static void createNewSem(key_t key) {
  if((semaphore = semget(key, 5, 0666 | IPC_CREAT)) == -1) end("[P1] Błąd przy tworzeniu nowego semafora!\n");
  printf("[P1] Semafor został utworzony: %d\n", semaphore);
}

int main() {
  if((key = ftok(".", 'T')) == -1) end("[P1] Błąd tworzenia klucza!\n");
  createNewSem(key);
  FILE* file = fopen("spr1.txt", "a");
  //p(s2)
  //t11
  //t12
  //v(s3)
  semaphore_p(2);
  printf("Sekcja t11 o PID = %d\n", getpid());
	fprintf(file, "Sekcja t11 o PID = %d\n", getpid());
  sleep(1);
  printf("Sekcja t12 o PID = %d\n", getpid());
	fprintf(file, "Sekcja t12 o PID = %d\n", getpid());
  sleep(1);
  semaphore_v(3);

  fclose(file);
  exit(0);
}
