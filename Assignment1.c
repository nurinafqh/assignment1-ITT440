
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SUM_LENGTH 4
#define SUM_WAIT_INTERVAL 2

void getSUM(char sum[SUM_LENGTH + 1]) {
srand(getpid() + getppid());

sum[0] = 49 + rand() % 7;

for(int i = 1; i < SUM_LENGTH; i++) {
sum[i] = 48 + rand() % 7;
}

sum[SUM_LENGTH] = '\0';
}

void sigint_handler(int sig);
int main() {
signal(SIGINT, sigint_handler);

while(1) {
int pipefds[2];
char sum[SUM_LENGTH + 1];
char buffer[SUM_LENGTH + 1];
char str[50];

pipe(pipefds);

pid_t pid = fork();

if(signal(SIGINT, sigint_handler) == SIG_ERR) {
perror("signal");
exit(1);
}

if (pid == 0) {
getSUM(sum); 
close(pipefds[0]); //close read fd
write(pipefds[1], sum, SUM_LENGTH + 1); //write PIN to pipe

printf("Enter your Name:\n");
fgets(str, 50, stdin);
printf("\nYou entered: %s\n\n", str);
  
printf("Generating output from parent and sending to child...\n");
  
sleep(SUM_WAIT_INTERVAL); //delaying PIN generation intentionally

exit(EXIT_SUCCESS);
}

if(pid > 0) {
wait(NULL); //waiting for child to finish

close(pipefds[1]); //close write fd
read(pipefds[0], buffer, SUM_LENGTH + 1); //read PIN from pipe
close(pipefds[0]); //close read fd
printf("Child received input '%s' from Parent. \n\n", buffer);
}
}

return EXIT_SUCCESS;
}

void sigint_handler(int sig) {
printf("This is an interrupt message!\n");
exit(1);
}

