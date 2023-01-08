#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <vector>
#include <iostream>

using namespace std;

int main()
{
    void *h = NULL;
    int (*GCF)(int a, int b);
    int *(*Sort)(int *array, int first, int last);
    int lib;
    printf("0 - изменить библиотеку, 1 - re1 , 2 - re2 , 3 - выход:\n");
    scanf("%d", &lib);

    while ((lib != 1) && (lib != 2))
    {
        printf("Input error, try again:\n");
        scanf("%d", &lib);
    }
    if (lib == 1)
    {
        h = dlopen("./libd1.so", RTLD_LAZY);
    }
    if (lib == 2)
    {
        h = dlopen("./libd2.so", RTLD_LAZY);
    }

    GCF = (int (*)(int, int))dlsym(h, "GCF");
    Sort = (int *(*)(int *, int, int))dlsym(h, "Sort");
    unsigned command;
    printf("1 - НОД, 2 - Сортировка:\n");
    scanf("%d", &command);
    while (1)
    {
        if (command == 0)
        {
            if (lib == 1)
            {
                dlclose(h);
                h = dlopen("./libd2.so", RTLD_LAZY);

                GCF = (int (*)(int, int))dlsym(h, "GCF");
                Sort = (int *(*)(int *, int, int))dlsym(h, "Sort");
                lib = 2;
                printf("re1 --> re2\n");
                scanf("%d", &command);
                continue;
            }
            else
            {
                dlclose(h);
                h = dlopen("./libd1.so", RTLD_LAZY);

                GCF = (int (*)(int, int))dlsym(h, "GCF");
                Sort = (int *(*)(int *, int, int))dlsym(h, "Sort");
                lib = 1;
                printf("re2 --> re1\n");
                scanf("%d", &command);
                continue;
            }
        }
        if (command == 1)
        {
            int a, b;
            scanf("%d%d", &a, &b);
            if ((a < 1) || (b < 1))
            {
                printf("Input error\n");
            }
            else
            {
                printf("%d\n", GCF(a, b));
            }
        }
        if (command == 2)
        {
            int n = 5;
            int array[n];
            for (int i = 0; i < n; ++i)
            {
                cin >> array[i];
            }
            int *arr = Sort(array, 0, n - 1);

            for (int i = 0; i < n; ++i)
            {
                printf("%d ", arr[i]);
            }
            printf("\n");
        }
        if (command == 3)
        {
            break;
        }
        scanf("%d", &command);
    }
    dlclose(h);
    return 0;
}
