#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <clocale>

using namespace std;

// Функция для сложения двух восьмеричных чисел с выводом в столбик
void addition(ofstream& out, int num1, int num2) {
    out << "\n  " << oct << num1 << "\n";
    out << "+ " << oct << num2 << "\n";
    out << "----\n";
    int sum = num1 + num2;
    out << "  " << oct << sum << "\n";
}

// Функция для умножения двух восьмеричных чисел с выводом в столбик
void multiplication(ofstream& out, int num1, int num2) {
    out << "\n  " << oct << num1 << "\n";
    out << "* " << oct << num2 << "\n";
    out << "----\n";

    int temp = num2; 
    int product = num1 * num2;
    int pos = 0; 
    int count_parts = 0; 

    if (temp == 0) out << "  " << oct << product << "\n";

    while (temp > 0) {
        int digit = temp % 8; 
        int part = num1 * digit;

        out << "  " << oct << part;

        // Добавляем нули в зависимости от позиции цифры
        if (pos > 0) {
            for (int i = 0; i < pos; i++) {
                out << "0";
            }
        }
        out << "\n";

        temp /= 8;
        pos++;
        count_parts++;
    }

    if (count_parts > 1) {
        out << "----\n";
        out << "  " << oct << product << "\n";
    }
}

// Функция для чтения и проверки одного восьмеричного числа из файла
int input_octal_number(ifstream& in, int* result, string &invalid_num) {
    string input_str;
    
    if (!(in >> input_str)) {
        invalid_num = "";
        return 0; 
    }

    invalid_num = input_str;

    if (input_str.length() >= 11) { 
        return 0;
    }

    if (input_str.length() == 0) {
        return 0;
    }

    for (size_t i = 0; i < input_str.length(); i++) {
        if (input_str[i] < '0' || input_str[i] > '7') {
            return 0;
        }
    }

    sscanf(input_str.c_str(), "%o", result);

    return 1;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    ifstream input_file("input.txt");
    ofstream output_file("output.txt");
    
    if (!input_file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл input.txt" << endl;
        return 1;
    }
    
    if (!output_file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл output.txt" << endl;
        return 1;
    }
    
    int num1 = 0, num2 = 0;
    string invalid_num;

    output_file << "Сложение и умножение \"столбиком\" двух восьмеричных чисел\n\n";
    
    // Чтение первого числа из файла
    if (input_octal_number(input_file, &num1, invalid_num) == 0) {
        if (invalid_num.empty()){
            output_file << "Ошибка: не удалось прочитать первое число из файла" << endl;
        }else{
            output_file << "Ошибка: первое число " << invalid_num << " не является корректным восьмеричным числом (0-7, макс. 10 цифр)" << endl;
        }

        cout << "Результат записан в файл output.txt" << endl;
        input_file.close();
        output_file.close();
        return 1;
    }

    // Чтение второго числа из файла
    if (input_octal_number(input_file, &num2, invalid_num) == 0) {
        if (invalid_num.empty()){
            output_file << "Ошибка: не удалось прочитать второе число из файла" << endl;
        }else{
            output_file << "Ошибка: второе число " << invalid_num << " не является корректным восьмеричным числом (0-7, макс. 10 цифр)" << endl;
        }
        cout << "Результат записан в файл output.txt" << endl;
        input_file.close();
        output_file.close();
        return 1;
    }

    // Выполнение сложения
    output_file << "\nСложение:\n";
    addition(output_file, num1, num2);

    // Выполнение умножения
    output_file << "\nУмножение:\n";
    multiplication(output_file, num1, num2);
    
    input_file.close();
    output_file.close();
    
    cout << "Результат записан в файл output.txt" << endl;

    return 0;
}