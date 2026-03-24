#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char* digits;
    int sign;
} BigInteger;

void removeLeadingZeros(char* str) {
    int len = strlen(str);
    int i = 0;
    while (i < len - 1 && str[i] == '0') i++;
    if (i > 0) {
        for (int j = 0; j <= len - i; j++)
            str[j] = str[j + i];
    }
}

BigInteger createFromString(const char* str) {
    BigInteger num;
    int start = 0;
    if (str[0] == '-') {
        num.sign = -1;
        start = 1;
    } else if (str[0] == '+') {
        num.sign = 1;
        start = 1;
    } else {
        num.sign = 1;
    }
    int len = strlen(str) - start;
    num.digits = (char*)malloc(len + 1);
    strcpy(num.digits, str + start);
    removeLeadingZeros(num.digits);
    if (strcmp(num.digits, "0") == 0) num.sign = 1;
    return num;
}

BigInteger createFromInt(long long n) {
    char buffer[50];
    sprintf(buffer, "%lld", llabs(n));
    BigInteger num = createFromString(buffer);
    num.sign = (n >= 0) ? 1 : -1;
    return num;
}

void printBigInteger(const BigInteger* num) {
    if (num->sign == -1) printf("-");
    printf("%s", num->digits);
}

int compare(const BigInteger* a, const BigInteger* b) {
    int lenA = strlen(a->digits), lenB = strlen(b->digits);
    if (lenA > lenB) return 1;
    if (lenA < lenB) return -1;
    return strcmp(a->digits, b->digits);
}

char* addStrings(const char* a, const char* b) {
    int lenA = strlen(a), lenB = strlen(b), maxLen = lenA > lenB ? lenA : lenB;
    char* result = (char*)calloc(maxLen + 2, 1);
    int carry = 0, idxA = lenA - 1, idxB = lenB - 1, idxRes = maxLen;
    while (idxA >= 0 || idxB >= 0 || carry) {
        int sum = carry;
        if (idxA >= 0) sum += a[idxA--] - '0';
        if (idxB >= 0) sum += b[idxB--] - '0';
        result[idxRes--] = (sum % 10) + '0';
        carry = sum / 10;
    }
    if (idxRes >= 0) memmove(result, result + idxRes + 1, maxLen + 1 - idxRes);
    removeLeadingZeros(result);
    return result;
}

char* subtractStrings(const char* a, const char* b) {
    int lenA = strlen(a), lenB = strlen(b);
    char* result = (char*)calloc(lenA + 1, 1);
    int borrow = 0, idxA = lenA - 1, idxB = lenB - 1, idxRes = lenA - 1;
    while (idxA >= 0) {
        int diff = (a[idxA] - '0') - borrow;
        if (idxB >= 0) diff -= (b[idxB--] - '0');
        if (diff < 0) { diff += 10; borrow = 1; }
        else borrow = 0;
        result[idxRes--] = diff + '0';
        idxA--;
    }
    result[lenA] = '\0';
    removeLeadingZeros(result);
    return result;
}

BigInteger add(const BigInteger* a, const BigInteger* b) {
    BigInteger result;
    if (a->sign == b->sign) {
        result.digits = addStrings(a->digits, b->digits);
        result.sign = a->sign;
    } else {
        int cmp = compare(a, b);
        if (cmp >= 0) {
            result.digits = subtractStrings(a->digits, b->digits);
            result.sign = a->sign;
        } else {
            result.digits = subtractStrings(b->digits, a->digits);
            result.sign = b->sign;
        }
    }
    if (strcmp(result.digits, "0") == 0) result.sign = 1;
    return result;
}

BigInteger subtract(const BigInteger* a, const BigInteger* b) {
    BigInteger negB;
    negB.digits = strdup(b->digits);
    negB.sign = -b->sign;
    BigInteger result = add(a, &negB);
    free(negB.digits);
    return result;
}

BigInteger multiply(const BigInteger* a, const BigInteger* b) {
    if (strcmp(a->digits, "0") == 0 || strcmp(b->digits, "0") == 0)
        return createFromInt(0);
    int lenA = strlen(a->digits), lenB = strlen(b->digits), totalLen = lenA + lenB;
    int* res = (int*)calloc(totalLen, sizeof(int));
    for (int i = lenA - 1; i >= 0; i--) {
        for (int j = lenB - 1; j >= 0; j--) {
            int mul = (a->digits[i] - '0') * (b->digits[j] - '0');
            int sum = mul + res[i + j + 1];
            res[i + j + 1] = sum % 10;
            res[i + j] += sum / 10;
        }
    }
    BigInteger result;
    result.digits = (char*)malloc(totalLen + 1);
    int idx = 0;
    while (idx < totalLen && res[idx] == 0) idx++;
    if (idx == totalLen) {
        result.digits[0] = '0';
        result.digits[1] = '\0';
        result.sign = 1;
    } else {
        int j = 0;
        for (int i = idx; i < totalLen; i++)
            result.digits[j++] = res[i] + '0';
        result.digits[j] = '\0';
        result.sign = (a->sign == b->sign) ? 1 : -1;
    }
    free(res);
    return result;
}

BigInteger divide(const BigInteger* a, const BigInteger* b) {
    if (strcmp(b->digits, "0") == 0) {
        printf("Ошибка: деление на ноль!\n");
        return createFromInt(0);
    }
    if (compare(a, b) < 0) return createFromInt(0);
    if (strcmp(b->digits, "1") == 0) {
        BigInteger result;
        result.digits = strdup(a->digits);
        result.sign = (a->sign == b->sign) ? 1 : -1;
        return result;
    }
    BigInteger quotient = createFromInt(0);
    BigInteger one = createFromInt(1);
    BigInteger remainder = createFromString(a->digits);
    remainder.sign = 1;
    BigInteger absB = createFromString(b->digits);
    absB.sign = 1;
    while (compare(&remainder, &absB) >= 0) {
        BigInteger temp = subtract(&remainder, &absB);
        free(remainder.digits);
        remainder = temp;
        BigInteger newQuotient = add(&quotient, &one);
        free(quotient.digits);
        quotient = newQuotient;
    }
    quotient.sign = (a->sign == b->sign) ? 1 : -1;
    free(one.digits);
    free(remainder.digits);
    free(absB.digits);
    return quotient;
}

void freeBigInteger(BigInteger* num) {
    free(num->digits);
}

void readFromBinaryFile(const char* filename, BigInteger* num1, BigInteger* num2) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", filename);
        exit(1);
    }
    char str1[1000], str2[1000];
    if (fscanf(f, "%999s %999s", str1, str2) != 2) {
        fprintf(stderr, "Ошибка: в файле должно быть два числа\n");
        fclose(f);
        exit(1);
    }
    fclose(f);
    *num1 = createFromString(str1);
    *num2 = createFromString(str2);
}

void writeToTextFile(const char* filename, const BigInteger* result) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", filename);
        exit(1);
    }
    if (result->sign == -1) fprintf(f, "-");
    fprintf(f, "%s", result->digits);
    fclose(f);
}

int main(int argc, char* argv[]) {
    if (argc == 4) {
        const char* binFile = argv[1];
        const char* txtFile = argv[2];
        char op = argv[3][0];

        BigInteger num1, num2;
        readFromBinaryFile(binFile, &num1, &num2);

        BigInteger result;
        switch (op) {
            case '+': result = add(&num1, &num2); break;
            case '-': result = subtract(&num1, &num2); break;
            case '*': result = multiply(&num1, &num2); break;
            case '/': result = divide(&num1, &num2); break;
            default:
                fprintf(stderr, "Неизвестная операция. Используйте +, -, *, /\n");
                freeBigInteger(&num1);
                freeBigInteger(&num2);
                return 1;
        }

        writeToTextFile(txtFile, &result);
        freeBigInteger(&num1);
        freeBigInteger(&num2);
        freeBigInteger(&result);

        printf("Результат сохранён в %s\n", txtFile);
        return 0;
    }

    char str1[1000], str2[1000];

    printf("=== Калькулятор больших чисел ===\n");
    printf("Введите первое число: ");
    scanf("%s", str1);
    printf("Введите второе число: ");
    scanf("%s", str2);

    BigInteger num1 = createFromString(str1);
    BigInteger num2 = createFromString(str2);

    int choice;
    do {
        printf("\nВыберите операцию:\n");
        printf("1. Сложение\n");
        printf("2. Вычитание\n");
        printf("3. Умножение\n");
        printf("4. Деление\n");
        printf("5. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= 4) {
            printf("\nРезультат: ");
        }

        BigInteger result;

        switch(choice) {
            case 1:
                result = add(&num1, &num2);
                printBigInteger(&result);
                freeBigInteger(&result);
                break;
            case 2:
                result = subtract(&num1, &num2);
                printBigInteger(&result);
                freeBigInteger(&result);
                break;
            case 3:
                result = multiply(&num1, &num2);
                printBigInteger(&result);
                freeBigInteger(&result);
                break;
            case 4:
                result = divide(&num1, &num2);
                printBigInteger(&result);
                freeBigInteger(&result);
                break;
            case 5:
                printf("Завершение программы.\n");
                break;
            default:
                printf("Неверный выбор!\n");
        }
    } while (choice != 5);

    freeBigInteger(&num1);
    freeBigInteger(&num2);

    return 0;
}