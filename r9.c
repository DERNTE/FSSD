#include <stdio.h>
#include <stdlib.h>

// Функция для создания массива
int* create_array(int n) {
    int* arr = (int*)malloc(n * sizeof(int));  // *
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }
    return arr;
}

// Функция для ввода массива
void input_array(int* arr, int n) {
    printf("Введите %d чисел:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
}

// Функция для вывода массива
void print_array(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Рекурсивная функция для сортировки пар (шаг 1)
void sort_pairs(int* arr, int n) {
    // Проходим по всем парам
    for (int i = 0; i < n; i += 2) {
        // Если есть два элемента в паре
        if (i + 1 < n) {
            // Меняем местами, если не в порядке
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
            }
        }
    }
}

// Рекурсивная функция для слияния двух отсортированных массивов
int* merge_two_arrays(int* arr1, int n1, int* arr2, int n2) {
    // Создаем новый массив для результата
    int* result = create_array(n1 + n2);
    
    int i = 0; // индекс для arr1
    int j = 0; // индекс для arr2
    int k = 0; // индекс для result
    
    // Пока есть элементы в обоих массивах
    while (i < n1 && j < n2) {
        if (arr1[i] <= arr2[j]) {
            result[k] = arr1[i];
            i++;
        } else {
            result[k] = arr2[j];
            j++;
        }
        k++;
    }
    
    // Добавляем оставшиеся элементы из arr1
    while (i < n1) {
        result[k] = arr1[i];
        i++;
        k++;
    }
    
    // Добавляем оставшиеся элементы из arr2
    while (j < n2) {
        result[k] = arr2[j];
        j++;
        k++;
    }
    
    return result;
}

// Рекурсивная функция для сортировки методом фон Неймана
int* von_neumann_sort_recursive(int* arr, int n) {
    // Массив из 0 или 1 элемента уже отсортирован
    if (n <= 1) {
        int* result = create_array(n);
        if (n == 1) {
            result[0] = arr[0];
        }
        return result;
    }
    
    // Сначала сортируем все пары в исходном массиве
    sort_pairs(arr, n);
    
    // Если массив маленький, просто возвращаем копию
    if (n <= 2) {
        int* result = create_array(n);
        for (int i = 0; i < n; i++) {
            result[i] = arr[i];
        }
        return result;
    }
    
    // Делим массив на две части (каждая часть - это уже отсортированные пары)
    int mid = (n + 1) / 2; // чтобы учесть нечетное количество
    
    // Создаем левую и правую части
    int* left_part = create_array(mid);
    int* right_part = create_array(n - mid);
    
    // Копируем данные в левую часть
    for (int i = 0; i < mid; i++) {
        left_part[i] = arr[i];
    }
    
    // Копируем данные в правую часть
    for (int i = 0; i < n - mid; i++) {
        right_part[i] = arr[mid + i];
    }
    
    // Рекурсивно сортируем левую часть
    int* sorted_left = von_neumann_sort_recursive(left_part, mid);
    
    // Рекурсивно сортируем правую часть
    int* sorted_right = von_neumann_sort_recursive(right_part, n - mid);
    
    // Сливаем две отсортированные части
    int* result = merge_two_arrays(sorted_left, mid, sorted_right, n - mid);
    
    // Освобождаем память
    free(left_part);
    free(right_part);
    free(sorted_left);
    free(sorted_right);
    
    return result;
}


int main() {
    
    int n;
    printf("Введите количество чисел: ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("Количество чисел должно быть положительным!\n");
        return 1;
    }
    
    // Создаем и заполняем массив
    int* arr = create_array(n);
    input_array(arr, n);
    
    printf("\nИсходный массив:\n");
    print_array(arr, n);
    
    // Сортируем массив методом фон Неймана
    int* sorted_arr = von_neumann_sort_recursive(arr, n);
    
    printf("\nОтсортированный массив:\n");
    print_array(sorted_arr, n);
    
    // Освобождаем память
    free(arr);
    free(sorted_arr);
    
    return 0;
}
