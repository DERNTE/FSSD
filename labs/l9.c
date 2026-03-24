#include <stdio.h>
#include <locale.h>
#define N 100

// Функция для получения бита на определенной позиции
int ml_bit(int n, int pos) {
    return (n >> pos) & 1;
}

// Функция для нахождения позиции самого старшего значащего бита в числе
int st_bit(int n) {
    if (n == 0) return 0;
    int pos = 0;
    while (n > 0) {
        n >>= 1;
        pos++;
    }
    return pos - 1;
}
// Рекурсивная функция для разделения массива по биту на позиции pos
void partition(int* mas, int size, int pos, int i, int j) {
    if (i > j) return;

    // Ищем слева элемент с битом 1
    if (i < size && ml_bit(mas[i], pos) == 0) {
        partition(mas, size, pos, i + 1, j);
        return;
    }

    // Ищем справа элемент с битом 0
    if (j >= 0 && ml_bit(mas[j], pos) == 1) {
        partition(mas, size, pos, i, j - 1);
        return;
    }

    // Меняем местами найденные элементы
    if (i <= j) {
        int temp = mas[i];
        mas[i] = mas[j];
        mas[j] = temp;
        partition(mas, size, pos, i + 1, j - 1);
    }
}

// Основная рекурсивная функция поразрядной сортировки
void sort(int* mas, int size, int pos) {
    if (size <= 1 || pos < 0) return;

    // Разделяем массив по текущему биту
    partition(mas, size, pos, 0, size - 1);

    // Находим границу разделения
    int graniza = 0;
    for (int i = 0; i < size; i++) {
        if (ml_bit(mas[i], pos) == 1) {
            graniza = i;
            break;
        }
    }

    // Рекурсивно сортируем левую часть (бит = 0)
    sort(mas, graniza, pos - 1);

    // Рекурсивно сортируем правую часть (бит = 1)
    sort(mas + graniza, size - graniza, pos - 1);
}

int main() {
    setlocale(LC_ALL, "RU");

    int mas[N];
    int size;

    // Ввод размера массива
    printf("Введите количество элементов в массиве (от 1 до %d): ", N);
    scanf_s("%d", &size);
    if (size <= 0 || size > N) {
        printf("Размер массива должен находиться в диапазоне от 1 до %d!\n", N);
        return 1;
    }

    // Ввод элементов массива
    printf("Введите %d неотрицательных целых чисел:\n", size);
    for (int i = 0; i < size; i++) {
        printf("Элемент %d: ", i + 1);
        scanf_s("%d", &mas[i]);
        if (mas[i] < 0) {
            printf("Числа должны быть неотрицательными!");
            return 1;
        }
    }

    printf("\nИсходный массив:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", mas[i]);
    }

    // Находим максимальное число в массиве
    int max = mas[0];
    for (int i = 1; i < size; i++) {
        if (mas[i] > max) {
            max = mas[i];
        }
    }

    // Определяем позицию старшего значащего бита максимального числа
    int stbit = st_bit(max);

    // Вызываем рекурсивную сортировку, начиная со старшего бита
    sort(mas, size, stbit);

    printf("\n\nОтсортированный массив: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", mas[i]);
    }

    return 0;
}
