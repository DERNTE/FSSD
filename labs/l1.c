#include <stdio.h>
#include <Windows.h>

// Функция для сортировки и вывода трех чисел в порядке возрастания
void Por_Vozr(int a, int b, int c) {
	if (a > b) {
		if (a > c) {
			// a - наибольшее число
			if (b > c)
				printf("%d %d %d", c, b, a);
			else
				printf("%d %d %d", b, c, a);
		}
		else {
			// c - наибольшее число, a - среднее
			if (a > b)
				printf("%d %d %d", b, a, c);
			else
				printf("%d %d %d", a, b, c);
		}

	}
	else {
		if (b > c) {
			// b - наибольшее число
			if (a > c)
				printf("%d %d %d", c, a, b);
			else
				printf("%d %d %d", a, c, b);
		}
		else {
			// c - наибольшее число
			if (a > b)
				printf("%d %d %d", b, a, c);
			else
				printf("%d %d %d", a, b, c);
		}
	}
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int a, b, c;
	printf("Вывод трёх различных чисел в порядке возрастания.\nВведите первое число: ");
	scanf_s("%d", &a);
	printf("Введите второе число: ");
	scanf_s("%d", &b);
	printf("Введите третье число: ");
	scanf_s("%d", &c);
	printf("Расположение данных чисел в порядке возрастания: ");
	Por_Vozr(a, b, c);

	return 0;
}
