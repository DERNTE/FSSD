#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <string.h>

// Функция для сложения двух восьмеричных чисел с выводом в столбик
void addition(int num1, int num2) {
    printf("\n  %o\n", num1);
    printf("+ %o\n", num2);
    printf("----\n");
    int sum = num1 + num2;
    printf("  %o\n", sum);
}

// Функция для умножения двух восьмеричных чисел с выводом в столбик
void multiplication(int num1, int num2) {
    printf("\n  %o\n", num1);
    printf("* %o\n", num2);
    printf("----\n");

    int temp = num2; // Временная переменная для обработки цифр второго числа
    int product = num1 * num2;
    int pos = 0; // Позиция текущей цифры (для сдвига при умножении)
    int count_parts = 0; // Счетчик промежуточных произведений

    // Если второе число равно нулю, сразу выводим результат (ноль)
    if (temp == 0) printf("  %o\n", product);

    while (temp > 0) {
        int digit = temp % 8; // Получаем младшую восьмеричную цифру
        int part = num1 * digit;

        // Выводим промежуточное произведение
        printf("  %o", part);

        // Добавляем нули в зависимости от позиции цифры
        if (pos > 0) {
            for (int i = 0; i < pos; i++) {
                printf("0");
            }
        }
        printf("\n");

        temp /= 8;
        pos++;
        count_parts++;
    }

    // Если было более одного промежуточного произведения, выводим разделитель и результат
    if (count_parts > 1) {
        printf("----\n");
        printf("  %o\n", product);
    }

}

// Функция для ввода и проверки одного восьмеричного числа
int input_octal_number(int* result) {
    char input[12]; // Буфер для ввода (10 цифр + '\n' + '\0')

    fgets(input, sizeof(input), stdin);

    // Удаляем символ новой строки
    int len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    // Проверка длины
    if (len >= sizeof(input) - 1) {
        printf("Слишком длинное число! Максимум %d цифр\n", sizeof(input) - 2);
        return 0;
    }

    // Проверка на пустой ввод
    if (len == 0) {
        printf("Ошибка: пустой ввод\n");
        return 0;
    }

    // Проверка восьмеричных цифр
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '7') {
            printf("Ошибка: '%c' - не восьмеричная цифра (0-7)\n", input[i]);
            return 0;
        }
    }

    // Преобразование строки в восьмеричное число
    sscanf(input, "%o", result);

    return 1; // Успех
}

int main() {
    setlocale(LC_ALL, "ru");
    int num1, num2;

    printf("Сложение и умножение «столбиком» двух восьмеричных чисел\n\n");
    // Ввод первого числа
    printf("Введите первое неотрицательное восьмеричное число (максимум 10 цифр): ");
    if (input_octal_number(&num1) == 0) {
        return 1;
    }

    // Ввод второго числа
    printf("Введите второе неотрицательное восьмеричное число (максимум 10 цифр): ");
    if (input_octal_number(&num2) == 0) {
        return 1;
    }

    // Выполнение сложения
    printf("\nСложение:\n");
    addition(num1, num2);

    // Выполнение умножения
    printf("\nУмножение:\n");
    multiplication(num1, num2);

    return 0;
}
