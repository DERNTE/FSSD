#include <iostream>
#include <fstream>
#include <string>
#include <clocale>

// Функция проверки буквы
bool is_letter(unsigned char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
           (c >= 192 && c <= 255) || (c == 168) || (c == 184);
}

// Шифр Цезаря (сдвиг на 3)
char caesar_encrypt(unsigned char c, int shift = 3) {
    if (c >= 'a' && c <= 'z') return (c - 'a' + shift) % 26 + 'a';
    if (c >= 'A' && c <= 'Z') return (c - 'A' + shift) % 26 + 'A';

    if (c >= 192 && c <= 255) return (c - 192 + shift) % 64 + 192;
    return c;
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::string text;
    std::cout << "Введите строку: ";
    std::getline(std::cin, text);

    char red_char;
    std::cout << "Введите запрещённый символ: ";
    std::cin >> red_char;

    std::string max_word = "";
    int best_start = -1, best_end = -1;

    for (size_t i = 0; i < text.length(); ) {
        if (!is_letter((unsigned char)text[i])) {
            i++;
            continue;
        }

        size_t start = i;
        bool has_forbidden = false;

        while (i < text.length() && is_letter((unsigned char)text[i])) {
            if (text[i] == red_char) has_forbidden = true;
            i++;
        }

        size_t word_len = i - start;
        if (!has_forbidden && word_len > max_word.length()) {
            max_word = text.substr(start, word_len);
            best_start = start;
            best_end = i - 1;
        }
    }

    // Вывод с подчеркиванием в консоль
    std::cout << "\nРезультат в консоли:\n";
    for (size_t j = 0; j < text.length(); j++) {
        if ((int)j == best_start) std::cout << "\x1B[4m";
        std::cout << text[j];
        if ((int)j == best_end) std::cout << "\x1B[0m";
    }

    // шифрование + запись
    std::ofstream outFile("encrypted.txt");
    if (outFile.is_open()) {
        std::cout << "\n\nЗашифрованный текст:\n";
        for (char c : text) {
            char encrypted = caesar_encrypt(c);
            std::cout << encrypted;
            outFile << encrypted;
        }
        outFile.close();
        std::cout << "\n\nДанные успешно сохранены в encrypted.txt";
    }

    return 0;
}
