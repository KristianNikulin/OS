#include <cmath>
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

extern "C" int GCF(int a, int b);
extern "C" int *Sort(int *array, int first, int last);

int GCF(int a, int b)
{
    cout << "Обычный алгоритм\n";
    int t = min(a, b);
    while (t > 1)
    {
        if (a % t == 0 && b % t == 0)
        {
            return t;
        }
        t--;
    }
    return t;
}

// Сортировка Хоара
int *Sort(int *array, int first, int last)
{
    int i = first, j = last;
    double tmp, x = array[(first + last) / 2];

    do
    {
        while (array[i] < x)
        {
            i++;
        }
        while (array[j] > x)
        {
            j--;
        }
        if (i <= j)
        {
            if (i < j)
            {
                tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
            i++;
            j--;
        }
    } while (i <= j);

    if (i < last)
    {
        Sort(array, i, last);
    }
    if (first < j)
    {
        Sort(array, first, j);
    }
    return array;
}
