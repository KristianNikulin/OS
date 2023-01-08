#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/mman.h>

/*
gcc main.c -o lab4
*/

#define BUF_SIZE 128

int main()
{

    printf("Print output file name:\n");

    // WRITE FILE NAME TO MAP

    char *file = mmap(NULL, sizeof(char) * BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    fgets(file, BUF_SIZE, stdin);

    if (file == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }

    printf("Enter the numbers:\n");

    // WRITE NUMBERS TO MAP

    char *arr = mmap(NULL, sizeof(char) * BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    fgets(arr, BUF_SIZE, stdin);

    if (arr == MAP_FAILED)
    {
        perror("mmap error");
        exit(2);
    }

    // CHILD

    int child = fork();

    if (child == -1)
    {
        perror("Error during creating fork\n");
        exit(3);
    }
    else if (child == 0)
    {

        int f = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);

        if (f == -1)
        {
            perror("Error during creating file\n");
            exit(4);
        }

        int neg = 0;

        // FIRST NUMBER

        char *result = malloc(sizeof(char) * BUF_SIZE);
        int len = 0;
        char c = arr[len];

        if (c == '-')
        {
            neg = 1;
        }

        while (c != ' ')
        {
            result[len] = c;
            len++;
            c = arr[len];
        }
        len++;
        result[len] = '\0';
        c = arr[len];

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
                c = arr[len];
            }
            len++;
            c = arr[len];
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
                printf("Zero found\n");
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

        if (munmap(arr, BUF_SIZE) == -1)
        {
            perror("UNmap error");
            exit(5);
        }
        if (munmap(file, BUF_SIZE) == -1)
        {
            perror("UNmap error");
            exit(6);
        }
        return 0;
    }
    return 0;
}
