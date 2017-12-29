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
  if ((semop(semaphore, &bufor_sem, 1)) == -1) end("[P3] Błąd przy zamykaniu semafora!\n");
  printf("[P3] Semafor %d zostal zamkniety.\n", x);
}

static void semaphore_v(int x) {
  struct sembuf bufor_sem;
  bufor_sem.sem_num=x;
  bufor_sem.sem_op=1;
  bufor_sem.sem_flg=SEM_UNDO;
  if ((semop(semaphore, &bufor_sem, 1)) == -1) end("[P3] Błąd przy zamykaniu semafora!\n");
  printf("[P3] Semafor %d zostal zamkniety.\n", x);
}

static void createNewSem(key_t key) {
  if((semaphore = semget(key, 5, 0666 | IPC_CREAT)) == -1) end("[P3] Błąd przy tworzeniu nowego semafora!\n");
  printf("[P3] Semafor został utworzony: %d\n", semaphore);
}

int main() {
  if((key = ftok(".", 'T')) == -1) end("[P3] Błąd tworzenia klucza!\n");
  createNewSem(key);
  FILE* file = fopen("spr1.txt", "a");
  //p(s0)
  //t31
  //t32
  //v(s1)
  //p(s4)
  //t33
  semaphore_p(0);
  printf("Sekcja t31 o PID = %d\n", getpid());
	fprintf(file, "Sekcja t31 o PID = %d\n", getpid());
  sleep(1);
  printf("Sekcja t32 o PID = %d\n", getpid());
	fprintf(file, "Sekcja t32 o PID = %d\n", getpid());
  sleep(1);
  semaphore_v(1);
  semaphore_p(4);
  printf("Sekcja t33 o PID = %d\n", getpid());
	fprintf(file, "Sekcja t33 o PID = %d\n", getpid());
  sleep(1);

  fclose(file);
  exit(0);
}
