#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h>

#define N 10000

// функция для проверки: является ли символ русской или латинской буквой
bool is_letter(unsigned char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= 192 && c <= 255) ||
           (c == 168) || (c == 184);
}

int main() {
    setlocale(LC_ALL, "Russian");

    char string[N];
    printf("Введите строку длины не более %d, в которой все символы, кроме латинских и русских букв, будут разделителями:\n", N);
    fgets(string, N, stdin);

    char red_char;
    printf("Введите запрещённый символ: ");
    scanf("%c", &red_char);

    char max_word[N] = "";
    int max_len = 0;
    int best_start = -1, best_end = -1;

    int i = 0, len = strlen(string);

    while (i < len) {
        // Пропуск не-букв
        if (!is_letter((unsigned char)string[i])) {
            i++;
            continue;
        }

        // Начало слова
        int start = i;
        bool has_forbidden = false;

        // Считываем слово
        while (i < len && is_letter((unsigned char)string[i])) {
            if (string[i] == red_char) has_forbidden = true;
            i++;
        }

        int word_len = i - start;

        // Проверка и сохранение максимального слова
        if (!has_forbidden && word_len > max_len) {
            strncpy(max_word, &string[start], word_len);
            max_word[word_len] = '\0';
            max_len = word_len;
            best_start = start;
            best_end = i - 1;
        }
    }

    printf("\nИсходная строка с подчёркнутым словом:\n");

    for (int j = 0; j < len; j++) {
        if (j == best_start)
            printf("\x1B[4m"); // включаем подчёркивание
        if (j == best_end + 1)
            printf("\x1B[0m"); // выключаем подчёркивание
        printf("%c", string[j]);
    }
    return 0;
}
