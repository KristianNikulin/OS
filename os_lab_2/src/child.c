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
gcc child.c -o child
*/

#define BUF_SIZE 128

int main(int argv, char *args[])
{
    char buf[BUF_SIZE];

    char *file_name = args[0];

    int f = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0777);

    if (f == -1)
    {
        perror("Error during creating file\n");
        exit(1);
    }

    if (read(STDIN_FILENO, buf, BUF_SIZE) < 0)
    {
        perror("read error");
        exit(2);
    }

    int neg = 0;

    // FIRST NUMBER

    char *result = malloc(sizeof(char) * BUF_SIZE);
    int len = 0;
    char c = buf[len];

    if (c == '-')
    {
        neg = 1;
    }

    while (c != ' ')
    {
        result[len] = c;
        len++;
        c = buf[len];
    }
    len++;
    result[len] = '\0';
    c = buf[len];

    int res = atoi(result);

    if (neg)
    {
        res *= -1;
    }

    // NUMBERS

    int index = 0;

    while ((c >= 48 && c <= 57) || c == ' ' || c == '-')
    {

        char *number = malloc(sizeof(char) * BUF_SIZE);
        index = 0;

        while (c != ' ')
        {
            number[index] = c;
            index++;
            len++;
            c = buf[len];
        }
        len++;
        c = buf[len];
        index++;
        number[index] = '\0';

        int num = atoi(number);

        if (num < 0)
        {
            num *= -1;
            if (neg == 1)
            {
                neg = 0;
            }
            else
            {
                neg = 1;
            }
        }

        if (num == 0)
        {
            char *zero = "Zero found\n";
            write(1, zero, sizeof(char) * strlen(zero));

            break;
        }

        res /= num;

        char buffer[30];
        int len = sprintf(buffer, "%d", res);
        if (neg)
        {
            write(f, "-", 1);
        }
        write(f, buffer, len);
        write(f, "\n", 1);

        free(number);
    }
    return 0;
}
