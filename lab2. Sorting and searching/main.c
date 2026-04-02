#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int number;
    char letter;
} Key;

typedef struct {
    char data[50];
} Data;

int key_comparison(Key key1, Key key2) {
    if (key1.number != key2.number)
        return key1.number - key2.number;

    return key1.letter - key2.letter;
}

void swap(Key *keys, Data *data_array, int i, int j) {
    Key temp_key = keys[i];
    keys[i] = keys[j];
    keys[j] = temp_key;

    Data temp_data = data_array[i];
    data_array[i] = data_array[j];
    data_array[j] = temp_data;
}

void selection_sort(Key *keys, Data *data_array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_ind = i;

        for (int j = i + 1; j < n; j++) {
            if (key_comparison(keys[min_ind], keys[j]) > 0) {
                min_ind = j;
            }
        }

        if (min_ind != i) {
            swap(keys, data_array, i, min_ind);
        }
    }
}

int binary_search(Key *keys, Key search, int n) {
    int left = 0, right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = key_comparison(search, keys[mid]);

        if (cmp == 0)
            return mid;
        else if (cmp > 0)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}

void print_table(Key *keys, Data *data_array, int n) {
    printf("Index Key(number, let)  Data\n");

    for (int i = 0; i < n; i++) {
        printf("%5d (%2d, %c)           %s\n",
               i,
               keys[i].number,
               keys[i].letter,
               data_array[i].data);
    }
    printf("\n");
}

void load_from_file(const char *filename, Key *keys, Data *data_array, int n) {
    FILE *f = fopen(filename, "r");

    if (!f) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(f, "%d %c %[^\n]",
                   &keys[i].number,
                   &keys[i].letter,
                   data_array[i].data) != 3) {
            printf("Error reading file %s at line %d\n", filename, i);
            exit(1);
        }
    }

    fclose(f);
}

void run_test(const char *filename, Key *keys, Data *data_array, int n) {
    printf("\nTEST: %s\n", filename);

    load_from_file(filename, keys, data_array, n);

    printf("\nBEFORE SORT\n");
    print_table(keys, data_array, n);

    selection_sort(keys, data_array, n);

    printf("\nAFTER SORT\n");
    print_table(keys, data_array, n);
}

int main() {
    int n = 20;

    Key keys[20];
    Data data_array[20];

    run_test("sorted.txt", keys, data_array, n);
    run_test("reverse.txt", keys, data_array, n);
    run_test("random.txt", keys, data_array, n);

    Key search;

    printf("\nEnter key to search:\n");
    printf("Number: ");
    scanf("%d", &search.number);

    printf("Letter: ");
    scanf(" %c", &search.letter);

    int index = binary_search(keys, search, n);

    if (index != -1) {
        printf("Found at index %d: %s\n", index, data_array[index].data);
    } else {
        printf("Not found\n");
    }

    return 0;
}