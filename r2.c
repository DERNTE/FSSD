#include <stdio.h>
#include <math.h>

int Count_Pos(int number)
{
 int p = 1;
 while ((number / (int)pow(10, p)) > 0) p++; // цикл с предусловием
 return p;
}

int posl() {
    int k, n;

    // Ввод k с проверкой
    do {
        printf("Введите количество цифр последовательности: ");
        scanf_s("%d", &k);
        if (k <= 1) {
            printf("Количество цифр последовательности должно быть больше 1.\n\n");
        }
    } while (k <= 1);

    // Ввод n с проверкой
    do {
        printf("\nВведите основание: ");
        scanf_s("%d", &n);
        if (n <= 1) {
            printf("Основание должно быть больше 1.\n\n");
        }
    } while (n <= 1);

    int s = 1;
    printf("%d", s);
    k = k - 1;

    // Основной цикл генерации последовательности
    while (k > 0) {
        s = s * n;
        int l = Count_Pos(s);

        if (k - l > 0) {
            printf("%d", s);
            k = k - l;
        }
        else {
            // Вывод части числа
            for (int i = k; i < l; i++) {
                s = s / 10;
            }
            printf("%d", s);
            break;
        }
    }

    return 1;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    posl();
    return 0;
}
