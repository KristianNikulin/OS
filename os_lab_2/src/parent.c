#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>

/*
gcc parent.c -o lab2
*/

#define BUF_SIZE 128

char *read_string()
{

    char *x = calloc(sizeof(char), BUF_SIZE);

    if (read(0, x, sizeof(char) * BUF_SIZE) == -1)
    {
        perror("Error during reading string\n");
        exit(1);
    }

    char *y = malloc(sizeof(char) * strlen(x));
    strncpy(y, x, strlen(x));
    free(x);

    return y;
}

int main()
{
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Error during creating pipe\n");
        exit(3);
    }

    // FILE NAME INPUT

    char *print_file = "Print output file name:\n";
    write(1, print_file, sizeof(char) * strlen(print_file));

    char *file_name = read_string();

    // INT NUMBERS INPUT

    char *print_numbers = "Enter the numbers:\n";
    write(1, print_numbers, sizeof(char) * strlen(print_numbers));

    char *a = read_string();

    int a_len = strlen(a), a_len1;

    // WRITE ARRAY TO PIPE

    if ((a_len1 = write(fd[1], a, a_len)) != a_len)
    {
        perror("write arr error");
        exit(2);
    }

    sleep(1);

    // CHILD

    int child = fork();

    if (child == -1)
    {
        perror("Error during creating fork\n");
        exit(4);
    }
    else if (child == 0)
    {
        // char * child_pr = "child process\n";
        // write(1, child_pr, sizeof(char) * strlen(child_pr));

        char *args[] = {file_name, NULL};

        if (close(fd[1]) == -1)
        {
            perror("close fd error");
            exit(5);
        }

        if (dup2(fd[0], STDIN_FILENO) == -1)
        {
            perror("Descriptor fd error output\n");
            exit(7);
        }
        fflush(stdout);

        // EXEC

        execv("./child", args);
        perror("Exec error\n");
    }
    return 0;
}
