#include <stdio.h>
#define nX 50
#define nA 50
void sum_x(int nx, int na) {
    int X[nX];
    // Ввод элементов массива X
    printf("Введите %d элементов массива X:\n", nx);
    for (int i = 0; i < nx; i++) {
        printf("X[%d] = ", i);
        scanf_s("%d", &X[i]);
    }
    int A[nA];
    // Ввод элементов массива A
    printf("Введите %d элементов массива A:\n", na);
    for (int i = 0; i < na; i++) {
        printf("A[%d] = ", i);
        scanf_s("%d", &A[i]);
    }

    int S = 0;

    // Вычисление суммы элементов массива X по индексам из массива A
    for (int i = 0; i < na; i++) {
        int index = A[i]; // Получаем индекс из массива A

        // Проверяем, что индекс находится в допустимых границах массива X
        if (index >= 0 && index < nx) {
            S += X[index]; // Добавляем элемент к сумме
        }
    }
    printf("Сумма S = %d\n", S);
}
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int nx, na;
    // Ввод размера массива X
    printf("Введите размер массива X (от 1 до %d): ", nX);
    scanf_s("%d", &nx);

    // Ввод размера массива A
    printf("Введите размер массива A (от 1 до %d): ", nA);
    scanf_s("%d", &na);

    sum_x(nx, na);

    return 0;
}
