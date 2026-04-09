#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int number;
    char letter;
} Key;

typedef struct {
    char data[50];
} Data;

int is_greater(Key k1, Key k2) {
    if (k1.number > k2.number) return 1;
    if (k1.number < k2.number) return 0;
    return k1.letter > k2.letter;
}

void swap_elements(Key *keys, Data *data_array, int i, int j) {
    Key tempKey = keys[i];
    keys[i] = keys[j];
    keys[j] = tempKey;

    Data tempData = data_array[i];
    data_array[i] = data_array[j];
    data_array[j] = tempData;
}

void selection_sort(Key *keys, Data *data_array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_ind = i;
        for (int j = i + 1; j < n; j++) {
            if (is_greater(keys[min_ind], keys[j])) {
                min_ind = j;
            }
        }
        if (min_ind != i) {
            swap_elements(keys, data_array, min_ind, i);
        }
    }
}

int binary_search(Key *keys, Key search, int n) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (keys[mid].number == search.number && keys[mid].letter == search.letter) {
            return mid;
        }
        if (is_greater(search, keys[mid])) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

void print_table(Key *keys, Data *data_array, int n) {
    printf("\n%-14s  %-12s  %-s\n", "Индекс", "Ключ", "Данные");
    for (int i = 0; i < n; i++) {
        printf("%-8d  %-2d %-5c  %s\n", i, keys[i].number, keys[i].letter, data_array[i].data);
    }
}

int main() {
    int choice;
    char filename[20];

    printf("Выберите файл для загрузки:\n");
    printf("1 - Отсортированная таблица\n");
    printf("2 - Обратная таблица\n");
    printf("3 - Рандомная таблица\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1: strcpy(filename, "sorted.txt"); break;
        case 2: strcpy(filename, "reverse.txt"); break;
        case 3: strcpy(filename, "random.txt"); break;
        default: printf("Неверный выбор\n"); return 1;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return 1;
    }

    int capacity = 10;
    int count = 0;
    Key *keys = malloc(capacity * sizeof(Key));
    Data *data_array = malloc(capacity * sizeof(Data));

    int temp_num;
    char temp_let;
    char temp_text[100];

    while (fscanf(file, "%d %c %[^\n]", &temp_num, &temp_let, temp_text) == 3) {
        if (count >= capacity) {
            capacity *= 2;
            keys = realloc(keys, capacity * sizeof(Key));
            data_array = realloc(data_array, capacity * sizeof(Data));
        }
        keys[count].number = temp_num;
        keys[count].letter = temp_let;
        strncpy(data_array[count].data, temp_text, 49);
        data_array[count].data[49] = '\0';
        count++;
    }
    fclose(file);

    if (count < 17) {
        printf("Ошибка: считано только %d элементов (нужно 17+)\n", count);
        free(keys); free(data_array);
        return 1;
    }

    printf("\nИСХОДНЫЕ ДАННЫЕ");
    print_table(keys, data_array, count);

    selection_sort(keys, data_array, count);

    printf("\nРЕЗУЛЬТАТ СОРТИРОВКИ");
    print_table(keys, data_array, count);

    printf("\nБИНАРНЫЙ ПОИСК");
    printf("\nВведите ключ (сначала ЧИСЛО, затем БУКВУ через пробел): ");
    while (1) {
        int res_scanf = scanf("%d %c", &temp_num, &temp_let);

        if (res_scanf == 2) {
            Key search_key = {temp_num, temp_let};
            int result = binary_search(keys, search_key, count);
            
            if (result != -1) {
                printf("Найдено в индексе [%d]. Данные: %s\n", result, data_array[result].data);
            } else {
                printf("Элемент c ключом {%d, %c} в таблице не найден.\n", temp_num, temp_let);
            }
            break;
        } else {
            printf("Ошибка! Нужно ввести СНАЧАЛА число, потом букву. Попробуйте еще раз: ");
            
            while (getchar() != '\n'); 
        }
    }

    free(keys);
    free(data_array);
    return 0;
}