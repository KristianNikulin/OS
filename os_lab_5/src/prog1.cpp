#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

extern "C" int GCF(int a, int b);
extern "C" int *Sort(int *array, int first, int last);

int main()
{
    int command;
    printf("1 - НОД, 2 - Сортировка, 3 - выход:\n");
    scanf("%d", &command);

    while (1)
    {
        if (command == 1)
        {
            int a, b;
            cin >> a >> b;

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
            int array[5];
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
    return 0;
}
