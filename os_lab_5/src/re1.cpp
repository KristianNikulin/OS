#include <cmath>
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

extern "C" int GCF(int a, int b);
extern "C" int *Sort(int *array, int first, int last);

int GCF(int a, int b)
{
    cout << "Алгоритм Евклида\n";
    int t;
    while (b != 0)
    {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int *Sort(int *array, int first, int last)
{
    cout << "Сортировка пузырьком\n";
    int n = 5;
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (array[j] > array[j + 1])
            {
                int t;
                t = array[j + 1];
                array[j + 1] = array[j];
                array[j] = t;
            }
        }
    }
    return array;
}
