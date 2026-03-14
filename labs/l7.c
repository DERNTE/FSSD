#include <stdio.h>
#define nX 50
#define nA 50
void sum_x(int nx, int na) {
    int X[nX];

    // Ввод элементов массива X
    printf("Введите %d элементов массива X:\n", nx);
    int* xPtr;
    xPtr = X;
    for (int i = 0; i < nx; i++) {
        printf("X[%d] = ", i);
        scanf_s("%d", xPtr + i);
    }

    int A[nA];
    // Ввод элементов массива A
    printf("Введите %d элементов массива A:\n", na);
    int* aPtr;
    aPtr = A;
    for (int i = 0; i < na; i++) {
        printf("A[%d] = ", i);
        scanf_s("%d", aPtr + i);
    }

    // Вычисление суммы элементов массива X по индексам из массива A
    int S = 0;


    for (int i = 0; i < na; i++) {
        int index = *(aPtr + i); // Получаем значение, на которое указывает aPtr

        // Проверяем, что индекс находится в допустимых границах массива X
        if (index >= 0 && index < nx) {
            S += *(xPtr + index); // // Получаем элемент массива X через указатель и добавляем его к сумме
        }
    }
    printf("Сумма S = %d\n", S);
}
int main() {
    int nx, na;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Ввод размера массива X
    printf("Введите размер массива X (от 1 до %d): ", nX);
    scanf_s("%d", &nx);

    // Ввод размера массива A
    printf("Введите размер массива A (от 1 до %d): ", nA);
    scanf_s("%d", &na);

    sum_x(nx, na);

    return 0;
}
