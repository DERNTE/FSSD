#include <stdio.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "RU");
    int day, month;
    int wd1; // день недели 1 января (0 - понедельник, 1 - вторник, ..., 6 - воскресенье)

    printf("Введите день недели 1 января (0-ПН, 1-ВТ, 2-СР, 3-ЧТ, 4-ПТ, 5-СБ, 6-ВС): ");
    scanf_s("%d", &wd1);
    printf("Введите дату (день месяц): ");
    scanf_s("%d %d", &day, &month);

    // Проверка корректности ввода
    if (month < 1 || month > 12 || day < 1 || day > 31 || wd1 < 0 || wd1 > 6 || (day == 31 && (month == 4 || month == 6 || month == 9 || month == 11)) ||
        (day > 28 && month == 2)) {
        printf("Ошибка ввода данных!\n");
        return 1;
    }

    // Подсчет номера дня в году (невисокосный)
    int dayN = 0;

    // Суммируем дни в месяцах до заданного месяца
    //case соответствуют "сколько полных месяцев уже прошло"
    switch (month - 1) {
    case 11: dayN += 30;
    case 10: dayN += 31;
    case 9: dayN += 30;
    case 8:  dayN += 31;
    case 7:  dayN += 31;
    case 6:  dayN += 30;
    case 5:  dayN += 31;
    case 4:  dayN += 30;
    case 3:  dayN += 31;
    case 2:  dayN += 28;
    case 1:  dayN += 31;
    }

    // Добавляем день текущего месяца
    dayN += day;

    // Сколько дней прошло с начала года до нашей даты (не включая 1 января)
    int days_passed = dayN - 1;

    // День недели нашей даты
    int wd = (wd1 + days_passed) % 7;

    // Вывод результата
    printf("Дата %d.%d - это ", day, month);
    switch (wd) {
    case 0: printf("понедельник\n"); break;
    case 1: printf("вторник\n"); break;
    case 2: printf("среда\n"); break;
    case 3: printf("четверг\n"); break;
    case 4: printf("пятница\n"); break;
    case 5: printf("суббота\n"); break;
    case 6: printf("воскресенье\n"); break;
    }

    return 0;
}
