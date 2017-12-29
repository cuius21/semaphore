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
  if ((semop(semaphore, &bufor_sem, 1)) == -1) end("[P2] Błąd przy zamykaniu semafora!\n");
  printf("[P2] Semafor %d zostal zamkniety.\n", x);
}

static void semaphore_v(int x) {
  struct sembuf bufor_sem;
  bufor_sem.sem_num=x;
  bufor_sem.sem_op=1;
  bufor_sem.sem_flg=SEM_UNDO;
  if ((semop(semaphore, &bufor_sem, 1)) == -1) end("[P2] Błąd przy otwieraniu semafora!\n");
  printf("[P2] Semafor %d zostal otwarty.\n", x);
}

static void createNewSem(key_t key) {
  if((semaphore = semget(key, 5, 0666 | IPC_CREAT)) == -1) end("[P2] Błąd przy tworzeniu nowego semafora!\n");
  printf("[P2] Semafor został utworzony: %d\n", semaphore);
}

int main() {
  if((key = ftok(".", 'T')) == -1) end("[P2] Błąd tworzenia klucza!\n");
  createNewSem(key);
  FILE* file = fopen("spr1.txt", "a");
  //t21
  //v(s0)
  //p(s1)
  //t22
  //v(s2)
  //p(s3)
  //t23
  //v(s4)
  printf("Sekcja t21 o PID = %d\n", getpid());
	fprintf(file, "Sekcja t21 o PID = %d\n", getpid());
  sleep(1);
  semaphore_v(0);
  semaphore_p(1);
  printf("Sekcja t22 o PID = %d\n", getpid());
	fprintf(file, "Sekcja t22 o PID = %d\n", getpid());
  sleep(1);
  semaphore_v(2);
  semaphore_p(3);
  printf("Sekcja t23 o PID = %d\n", getpid());
	fprintf(file, "Sekcja t23 o PID = %d\n", getpid());
  sleep(1);
  semaphore_v(4);

  fclose(file);
  exit(0);
}
