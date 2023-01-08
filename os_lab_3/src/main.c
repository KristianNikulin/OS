#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <pthread.h>
#include <errno.h>
#include <ctype.h>

// gcc main.c -pthread -o lab3
// ./lab3 - -

int *N;

typedef struct arguments
{
    int points;
    int ii;
} Arg;

void *thread_function(void *args)
{

    Arg *arg = (Arg *)args;
    int points = arg->points;
    int ii = arg->ii;

    int deck0[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                   26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

    int deck1[52], deck3[51];

    int ik = 51, ij, ijk;

    for (int j = 0; j < points; j++)
    {

        for (int i = 0; i <= 51; i++)
        {
            deck1[i] = deck0[i];
        }

        ijk = 51;

        for (int i = 0; i <= ik; i++)
        {

            if (i != ik)
            {
                ij = rand() % ijk;
            }
            else
            {
                ij = 0;
            }

            deck3[i] = deck1[ij] % 13;
            deck1[ij] = deck1[ijk];
            ijk = ijk - 1;
        }
        if ((deck3[ik]) == (deck3[ik - 1]))
        {
            N[ii]++;
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("syntax error ./lab3 count_of_rounds number_of_threads\n");
        exit(1);
    }

    int rounds = atoi(argv[1]), threads_num = atoi(argv[2]);

    N = (int *)calloc(threads_num, sizeof(int));

    srand(time(NULL));

    pthread_t *threads = (pthread_t *)calloc(threads_num, sizeof(pthread_t));
    if (threads == NULL)
    {
        printf("threads memory error\n");
        exit(2);
    }

    int points_for_thread = rounds / threads_num;

    printf("%d rounds for each thread\n\n", points_for_thread);

    int ost;
    if (rounds % threads_num != 0)
    {
        ost = rounds % threads_num;
    }

    Arg a;

    for (int i = 0; i < threads_num; i++)
    {
        a.points = points_for_thread;
        a.ii = i;

        if (i + 1 == threads_num)
        {
            a.points += ost;
        }

        // указатель на поток, атрибуты потока (NULL), функция выполняющаяся в новом потоке, аргументы функции (struct)
        if (pthread_create(&threads[i], NULL, thread_function, &a) != 0)
        {
            printf("create thread error\n");
            exit(3);
        }
    }

    for (int i = 0; i < threads_num; i++)
    {
        // ожидание выхода из указанного потока
        if (pthread_join(threads[i], NULL) != 0)
        {
            printf("join error\n");
            exit(4);
        }
    }

    double ans = 0;
    for (int i = 0; i < threads_num; i++)
    {
        ans += (double)N[i] / rounds;
    }

    printf("\n");
    printf("Monte-Carlo chance %.5f\n", (double)ans);
    printf("Real chance        %.5f\n", (double)3 / 51);
    free(threads);
    return 0;
}