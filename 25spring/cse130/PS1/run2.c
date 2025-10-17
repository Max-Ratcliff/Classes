#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    /* There should at least be 3 arguments in addition to run2: 2 for the first command, and the rest for the second command */
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }

    /* TODO: Create 2 child processes. Each child process must execute one of the two commands. Use execlp and execvp to do this.
	     The parent must reap the dead child processes before exiting. 
	     Print the exit status of the children using the following printf statement.
	     printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status)); */

    pid_t pid1, pid2;
    int status;

    // first child for cmd1
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(1);
    }
    if (pid1 == 0) {
        execlp(argv[1], argv[1], argv[2], NULL);
        perror("execlp");
        exit(1);
    }
    // second child for cmd2
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(1);
    }
    if (pid2 == 0) {
        execvp(argv[3], &argv[3]);
        perror("execvp");
        exit(1);
    }
    // parent reaps children
    if (waitpid(pid1, &status, 0) > 0) {
        printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status));
    }
    if (waitpid(pid2, &status, 0) > 0) {
        printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status));
    }

    return 0;
}
