#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Структура для хранения длинного числа
typedef struct {
    char* digits;  // строка с цифрами
    int sign;      // знак: 1 - положительное, -1 - отрицательное
} BigInteger;

// Удаление ведущих нулей
void removeLeadingZeros(char* str) {
    int len = strlen(str);
    int i = 0;
    
    // Пропускаем ведущие нули, но оставляем хотя бы одну цифру
    while (i < len - 1 && str[i] == '0') {
        i++;
    }
    
    // Сдвигаем оставшиеся цифры в начало
    if (i > 0) {
        for (int j = 0; j <= len - i; j++) {
            str[j] = str[j + i];
        }
    }
}

// Создание числа из строки
BigInteger createFromString(const char* str) {
    BigInteger num;
    int start = 0;
    
    // Определяем знак
    if (str[0] == '-') {
        num.sign = -1;
        start = 1;
    } else if (str[0] == '+') {
        num.sign = 1;
        start = 1;
    } else {
        num.sign = 1;
    }
    
    // Выделяем память под цифры
    int len = strlen(str) - start;
    num.digits = (char*)malloc(len + 1);
    
    // Копируем цифры
    strcpy(num.digits, str + start);
    removeLeadingZeros(num.digits);
    
    // Если результат - ноль
    if (strcmp(num.digits, "0") == 0) {
        num.sign = 1;  // ноль всегда положительный
    }
    
    return num;
}

// Создание числа из целого
BigInteger createFromInt(long long n) {
    char buffer[50];
    sprintf(buffer, "%lld", llabs(n));
    BigInteger num = createFromString(buffer);
    num.sign = (n >= 0) ? 1 : -1;
    return num;
}

// Печать числа
void printBigInteger(const BigInteger* num) {
    if (num->sign == -1) {
        printf("-");
    }
    printf("%s", num->digits);
}

// Сравнение двух чисел (по модулю)
int compare(const BigInteger* a, const BigInteger* b) {
    int lenA = strlen(a->digits);
    int lenB = strlen(b->digits);
    
    if (lenA > lenB) return 1;
    if (lenA < lenB) return -1;
    
    return strcmp(a->digits, b->digits);
}

// Сложение положительных чисел
char* addStrings(const char* a, const char* b) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    int maxLen = (lenA > lenB) ? lenA : lenB;
    
    char* result = (char*)calloc(maxLen + 2, 1); // +1 для переноса, +1 для '\0'
    
    int carry = 0;
    int idxA = lenA - 1;
    int idxB = lenB - 1;
    int idxRes = maxLen;
    
    while (idxA >= 0 || idxB >= 0 || carry) {
        int sum = carry;
        if (idxA >= 0) sum += a[idxA--] - '0';
        if (idxB >= 0) sum += b[idxB--] - '0';
        
        result[idxRes--] = (sum % 10) + '0';
        carry = sum / 10;
    }
    
    // Сдвигаем результат в начало
    if (idxRes >= 0) {
        memmove(result, result + idxRes + 1, maxLen + 1 - idxRes);
    }
    
    removeLeadingZeros(result);
    return result;
}

// Вычитание положительных чисел (a >= b)
char* subtractStrings(const char* a, const char* b) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    char* result = (char*)calloc(lenA + 1, 1);
    
    int borrow = 0;
    int idxA = lenA - 1;
    int idxB = lenB - 1;
    int idxRes = lenA - 1;
    
    while (idxA >= 0) {
        int diff = (a[idxA] - '0') - borrow;
        if (idxB >= 0) diff -= (b[idxB--] - '0');
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result[idxRes--] = diff + '0';
        idxA--;
    }
    
    result[lenA] = '\0';
    removeLeadingZeros(result);
    return result;
}

// Сложение
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
    
    if (strcmp(result.digits, "0") == 0) {
        result.sign = 1;
    }
    
    return result;
}

// Вычитание
BigInteger subtract(const BigInteger* a, const BigInteger* b) {
    BigInteger negB;
    negB.digits = strdup(b->digits);
    negB.sign = -b->sign;
    
    BigInteger result = add(a, &negB);
    free(negB.digits);
    return result;
}

// Умножение
BigInteger multiply(const BigInteger* a, const BigInteger* b) {
    // Если одно из чисел ноль
    if (strcmp(a->digits, "0") == 0 || strcmp(b->digits, "0") == 0) {
        return createFromInt(0);
    }
    
    int lenA = strlen(a->digits);
    int lenB = strlen(b->digits);
    int totalLen = lenA + lenB;
    
    // Создаем массив для результата
    int* res = (int*)calloc(totalLen, sizeof(int));
    
    // Умножаем в столбик
    for (int i = lenA - 1; i >= 0; i--) {
        for (int j = lenB - 1; j >= 0; j--) {
            int mul = (a->digits[i] - '0') * (b->digits[j] - '0');
            int sum = mul + res[i + j + 1];
            
            res[i + j + 1] = sum % 10;
            res[i + j] += sum / 10;
        }
    }
    
    // Преобразуем массив в строку
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
        for (int i = idx; i < totalLen; i++) {
            result.digits[j++] = res[i] + '0';
        }
        result.digits[j] = '\0';
        result.sign = (a->sign == b->sign) ? 1 : -1;
    }
    
    free(res);
    return result;
}

// Деление (целочисленное)
BigInteger divide(const BigInteger* a, const BigInteger* b) {
    // Проверка деления на ноль
    if (strcmp(b->digits, "0") == 0) {
        printf("Ошибка: деление на ноль!\n");
        return createFromInt(0);
    }
    
    // Сравнение чисел
    int cmp = compare(a, b);
    
    // Если a < b, результат 0
    if (cmp < 0) {
        return createFromInt(0);
    }
    
    // Если b = 1
    if (strcmp(b->digits, "1") == 0) {
        BigInteger result;
        result.digits = strdup(a->digits);
        result.sign = (a->sign == b->sign) ? 1 : -1;
        return result;
    }
    
    // Простой алгоритм деления вычитанием
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
    
    // Устанавливаем знак
    quotient.sign = (a->sign == b->sign) ? 1 : -1;
    
    // Освобождаем память
    free(one.digits);
    free(remainder.digits);
    free(absB.digits);
    
    return quotient;
}

// Освобождение памяти
void freeBigInteger(BigInteger* num) {
    free(num->digits);
}

// Основная функция
int main() {
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