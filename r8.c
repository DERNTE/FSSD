#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define N 100

void reverse_str(char *str, char *rev_str) {

int len = strlen(str);

if (len % 2 != 0) {

for (int i = strlen(str) - 1, j = 0; i >= 0; i--, j++)
rev_str[i] = str[j];
}
}

int main() {

FILE *f1, *f2;
char str[N], rev_str[N];

if ((f1 = fopen("text.txt", "r")) == NULL) {

printf("Ошибка при открытии исходного файла!");
return 1;
}

if ((f2 = fopen("changed_text.txt", "w")) == NULL) {

printf("Ошибка при создании файла-резульата!");
return 1;
}

while (fgets(str, sizeof(str), f1) != NULL) {

reverse_str(str, rev_str);
fputs(rev_str, f2);
}
fclose(f1), fclose(f2);

printf("Строки нечетной длины из первого файла, записанные в перевернутом виде:\n\n");
if ((f2 = fopen("changed_text.txt", "r")) == NULL) {

printf("Ошибка чтения файла!");
return 1;
}

while (fgets(str, sizeof(str), f2) != NULL) {

printf("%s", str);
}
fclose(f2);
return 0;
}
