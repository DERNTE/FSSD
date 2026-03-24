#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define N 1000

// функция дублирования строки, длина которой меньше заданной
void dubl_str(char* line, int min_len, char* f2)
{
    int len = strlen(line); // получаем длину строки
    if (len > 0 && line[len - 1] == '\n') // убираем символ новой строки из подсчета длины, если он есть
        len--;

    if (len < min_len) { // если длина строки (уже без \n) меньше заданной, то дублируем строку в файл
        fputs(line, f2);
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    FILE* f1, * f2; // указатели на файлы
    char filename1[100], filename2[100]; // буферы для имен файлов
    char line[N]; // буфер для чтения строк из файла
    int min_len; // минимальная длина строки

    // ввод имен файлов и минимальной длины

    puts("Введите имя исходного файла: ");
    fgets(filename1, sizeof(filename1), stdin);

    filename1[strlen(filename1) - 1] = '\0'; // убираем символ новой строки

    puts("Введите имя выходного файла: ");
    fgets(filename2, sizeof(filename2), stdin);

    filename2[strlen(filename2) - 1] = '\0'; // убираем символ новой строки

    puts("Введите минимальную длину строки: ");
    scanf("%d", &min_len);

    // открытие файлов
    f1 = fopen(filename1, "r"); // открываем исходный файл для чтения
    if (f1 == NULL) {
        puts("Ошибка: не удалось открыть исходный файл");
        return 1; // завершение с ошибкой
    }

    f2 = fopen(filename2, "w"); // открываем(если нету,создаем) выходной файл для записи
    if (f2 == NULL) {
        puts("Ошибка: не удалось создать выходной файл");
        fclose(f1); // закрываем уже открытый файл
        return 1; // завершение с ошибкой
    }



    // Обработка строк
    while (fgets(line, sizeof(line), f1) != NULL) {
        fputs(line, f2); // записываем строку в выходной файл
        dubl_str(line, min_len, f2); // проверяем и дублируем строку, если ее длина меньше минимальной
    }

    // Закрытие файлов
    fclose(f1);
    fclose(f2);

    puts("Обработка завершена. Результат записан в выходной файл");

    return 0;
}
