#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define READEND 0
#define WRITEEND 1

int main(int argc,char* argv[]) {  
   if (argc < 3) {
      printf("Usage: pipes a b, where a and b are integers. \n");
      exit(1);
   }

   /* TODO: Implement the command: "ls /dev | xargs | cut -d ' ' -f<a>-<b>" 
      	    where <a> and <b> are integers that specify the range and are taken as input from the command line. */


   int a = atoi(argv[1]);
   int b = atoi(argv[2]);
   int pipe1[2], pipe2[2];
   if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
      perror("pipe");
      exit(1);
   }
   pid_t pid1, pid2, pid3;
   // first child: ls /dev
   pid1 = fork();
   if (pid1 == 0) {
      dup2(pipe1[WRITEEND], STDOUT_FILENO);
      close(pipe1[READEND]);
      close(pipe1[WRITEEND]);
      close(pipe2[READEND]);
      close(pipe2[WRITEEND]);
      execlp("ls", "ls", "/dev", NULL);
      perror("execlp ls");
      exit(1);
   }
   // second child: xargs
   pid2 = fork();
   if (pid2 == 0) {
      dup2(pipe1[READEND], STDIN_FILENO);
      dup2(pipe2[WRITEEND], STDOUT_FILENO);
      close(pipe1[READEND]);
      close(pipe1[WRITEEND]);
      close(pipe2[READEND]);
      close(pipe2[WRITEEND]);
      execlp("xargs", "xargs", NULL);
      perror("execlp xargs");
      exit(1);
   }
   // third child: cut
   pid3 = fork();
   if (pid3 == 0) {
      char range[20];
      snprintf(range, sizeof(range), "%d-%d", a, b);
      dup2(pipe2[READEND], STDIN_FILENO);
      close(pipe1[READEND]);
      close(pipe1[WRITEEND]);
      close(pipe2[WRITEEND]);
      close(pipe2[READEND]);
      execlp("cut", "cut", "-d", " ", "-f", range, NULL);
      perror("execlp cut");
      exit(1);
   }
   // close pipes in parent
   close(pipe1[READEND]);
   close(pipe1[WRITEEND]);
   close(pipe2[READEND]);
   close(pipe2[WRITEEND]);
   // wait for children
   waitpid(pid1, NULL, 0);
   waitpid(pid2, NULL, 0);
   waitpid(pid3, NULL, 0);


   return 0;
}


