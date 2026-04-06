#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <vector>

class BigInteger {
private:
    std::string digits;
    int sign;

    void removeLeadingZeros() {
        size_t i = 0;
        while (i < digits.size() - 1 && digits[i] == '0')
            ++i;
        if (i > 0)
            digits = digits.substr(i);
        if (digits == "0")
            sign = 1;
    }

    int absCompare(const BigInteger& other) const {
        if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size() ? -1 : 1;
        return digits.compare(other.digits);
    }

    static std::string addStrings(const std::string& a, const std::string& b) {
        int lenA = a.size(), lenB = b.size();
        int maxLen = std::max(lenA, lenB);
        std::string result(maxLen + 1, '0');
        int carry = 0;
        int i = lenA - 1, j = lenB - 1, k = maxLen;
        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) sum += a[i--] - '0';
            if (j >= 0) sum += b[j--] - '0';
            result[k--] = (sum % 10) + '0';
            carry = sum / 10;
        }
        if (result[0] == '0')
            result = result.substr(1);
        return result;
    }

    static std::string subtractStrings(const std::string& a, const std::string& b) {
        int lenA = a.size();
        std::string result(lenA, '0');
        int borrow = 0;
        int i = lenA - 1, j = b.size() - 1, k = lenA - 1;
        while (i >= 0) {
            int diff = (a[i] - '0') - borrow;
            if (j >= 0) diff -= (b[j--] - '0');
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result[k--] = diff + '0';
            --i;
        }
        size_t pos = result.find_first_not_of('0');
        if (pos == std::string::npos)
            return "0";
        return result.substr(pos);
    }

public:
    BigInteger() : digits("0"), sign(1) {}

    BigInteger(const std::string& str) {
        size_t start = 0;
        if (str[0] == '-') {
            sign = -1;
            start = 1;
        } else if (str[0] == '+') {
            sign = 1;
            start = 1;
        } else {
            sign = 1;
        }
        digits = str.substr(start);
        removeLeadingZeros();
    }

    BigInteger(long long n) {
        if (n < 0) {
            sign = -1;
            n = -n;
        } else {
            sign = 1;
        }
        digits = std::to_string(n);
        removeLeadingZeros();
    }

    BigInteger(const BigInteger& other) = default;
    BigInteger& operator=(const BigInteger& other) = default;

    BigInteger operator+(const BigInteger& other) const {
        BigInteger result;
        if (sign == other.sign) {
            result.digits = addStrings(digits, other.digits);
            result.sign = sign;
        } else {
            int cmp = absCompare(other);
            if (cmp >= 0) {
                result.digits = subtractStrings(digits, other.digits);
                result.sign = sign;
            } else {
                result.digits = subtractStrings(other.digits, digits);
                result.sign = other.sign;
            }
        }
        if (result.digits == "0") result.sign = 1;
        return result;
    }

    BigInteger operator-(const BigInteger& other) const {
        BigInteger negOther = other;
        negOther.sign = -negOther.sign;
        return *this + negOther;
    }

    BigInteger operator*(const BigInteger& other) const {
        if (digits == "0" || other.digits == "0")
            return BigInteger(0);

        int lenA = digits.size(), lenB = other.digits.size();
        int totalLen = lenA + lenB;
        std::vector<int> res(totalLen, 0);

        for (int i = lenA - 1; i >= 0; --i) {
            for (int j = lenB - 1; j >= 0; --j) {
                int mul = (digits[i] - '0') * (other.digits[j] - '0');
                int sum = mul + res[i + j + 1];
                res[i + j + 1] = sum % 10;
                res[i + j] += sum / 10;
            }
        }

        std::string resultStr;
        int idx = 0;
        while (idx < totalLen && res[idx] == 0) ++idx;
        if (idx == totalLen) {
            resultStr = "0";
        } else {
            for (int i = idx; i < totalLen; ++i)
                resultStr.push_back(res[i] + '0');
        }

        BigInteger result;
        result.digits = resultStr;
        result.sign = (sign == other.sign) ? 1 : -1;
        if (result.digits == "0") result.sign = 1;
        return result;
    }

    BigInteger operator/(const BigInteger& other) const {
        if (other.digits == "0") {
            std::cerr << "Ошибка: деление на ноль!\n";
            return BigInteger(0);
        }
        if (absCompare(other) < 0)
            return BigInteger(0);
        if (other.digits == "1")
            return BigInteger(*this);

        BigInteger quotient(0);
        BigInteger one(1);
        BigInteger remainder(digits);
        remainder.sign = 1;
        BigInteger absOther(other.digits);
        absOther.sign = 1;

        while (remainder.absCompare(absOther) >= 0) {
            remainder = remainder - absOther;
            quotient = quotient + one;
        }
        quotient.sign = (sign == other.sign) ? 1 : -1;
        if (quotient.digits == "0") quotient.sign = 1;
        return quotient;
    }

    void print() const {
        if (sign == -1) std::cout << '-';
        std::cout << digits;
    }

    void writeToFile(const std::string& filename) const {
        FILE* f = fopen(filename.c_str(), "w");
        if (!f) {
            std::cerr << "Ошибка: не удалось открыть файл " << filename << "\n";
            exit(1);
        }
        if (sign == -1) fprintf(f, "-");
        fprintf(f, "%s", digits.c_str());
        fclose(f);
    }

    static void readPairFromBinaryFile(const std::string& filename, BigInteger& a, BigInteger& b) {
        FILE* f = fopen(filename.c_str(), "rb");
        if (!f) {
            std::cerr << "Ошибка: не удалось открыть файл " << filename << "\n";
            exit(1);
        }
        char str1[1000], str2[1000];
        if (fscanf(f, "%999s %999s", str1, str2) != 2) {
            std::cerr << "Ошибка: в файле должно быть два числа\n";
            fclose(f);
            exit(1);
        }
        fclose(f);
        a = BigInteger(str1);
        b = BigInteger(str2);
    }
};

int main(int argc, char* argv[]) {
    if (argc == 4) {
        std::string binFile = argv[1];
        std::string txtFile = argv[2];
        char op = argv[3][0];

        BigInteger num1, num2;
        BigInteger::readPairFromBinaryFile(binFile, num1, num2);

        BigInteger result;
        switch (op) {
            case '+': result = num1 + num2; break;
            case '-': result = num1 - num2; break;
            case '*': result = num1 * num2; break;
            case '/': result = num1 / num2; break;
            default:
                std::cerr << "Неизвестная операция. Используйте +, -, *, /\n";
                return 1;
        }

        result.writeToFile(txtFile);
        std::cout << "Результат сохранён в " << txtFile << "\n";
        return 0;
    }

    std::string s1, s2;
    std::cout << "=== Калькулятор больших чисел ===\n";
    std::cout << "Введите первое число: ";
    std::cin >> s1;
    std::cout << "Введите второе число: ";
    std::cin >> s2;

    BigInteger num1(s1), num2(s2);

    int choice;
    do {
        std::cout << "\nВыберите операцию:\n";
        std::cout << "1. Сложение\n";
        std::cout << "2. Вычитание\n";
        std::cout << "3. Умножение\n";
        std::cout << "4. Деление\n";
        std::cout << "5. Выход\n";
        std::cout << "Ваш выбор: ";
        std::cin >> choice;

        if (choice >= 1 && choice <= 4)
            std::cout << "\nРезультат: ";

        switch (choice) {
            case 1: (num1 + num2).print(); break;
            case 2: (num1 - num2).print(); break;
            case 3: (num1 * num2).print(); break;
            case 4: (num1 / num2).print(); break;
            case 5: std::cout << "Завершение программы.\n"; break;
            default: std::cout << "Неверный выбор!\n";
        }
        std::cout << std::endl;
    } while (choice != 5);

    return 0;
}