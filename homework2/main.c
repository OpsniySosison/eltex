#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Subscriber { 
    char name[10];
    char surname[10];
    char phone_number[10];
};

void Menu(void) {
    printf("0 - Open list\n");
    printf("1 - Create list of 10 entries (becomes openn by default)\n");
    printf("2 - Add note to opened list\n");
    printf("3 - Delete note from opened list\n");
    printf("4 - Print opened list\n");
    printf("5 - Search by selected field\n");
    printf("6 - Exit\n");
}

void IncorrectKey(void) {
    printf("Unknow command\n");
    printf("Press any key to continue\n");
    pause();
}

void GetFileName(char* file_name) {
    printf("Enter file name\n");
    getchar();
    fgets(file_name, 32, stdin);
}

void SaveChanges(FILE* file, struct Subscriber* arr, unsigned char* fill_arr, unsigned int size) { 
    if (file) {
        for (unsigned int i = 0; i < size; i++) {
            fwrite(&arr[i], sizeof(struct Subscriber), 1, file);
        }
    } else {
        printf("Unknow error\n");
        printf("Press any key to continue\n");
        pause();
    }
}

void OpenList(FILE* file, char* name, struct Subscriber* arr, unsigned char* fill_arr, unsigned int size) {
    GetFileName(name);

    file = fopen(strcat(name, ".asd"), "r+b");

    if (!file) {
        printf("File not found\n");
        printf("Press any key to continue\n");
        pause();
        return;
    }

    for (unsigned int i = 0; i < size; i++) {
        fread(&arr[i], sizeof(struct Subscriber), 1, file);
        fill_arr[i] = 1;
    }
}

void CreateList(FILE* file, char* name, struct Subscriber* arr, unsigned char* fill_arr, unsigned int size) {
    GetFileName(name);

    file = fopen(strcat(name, ".asd"), "w+b");

    if (!file) {
        printf("Unknow error\n");
        printf("Press any key to continue\n");
        pause();
        return;
    }

    printf("Creating list, please dont close the program\n");
    for (unsigned int i = 0; i < size; i++) {
        printf("Enter data to %d note\n", i + 1);
        getchar();
        fgets(arr[i].name, 10, stdin);
        fgets(arr[i].surname, 10, stdin);
        fgets(arr[i].phone_number, 10, stdin);
        fill_arr[i] = 1;
    }
}

void AddNote(struct Subscriber* arr, unsigned char* fill_arr, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        if (!fill_arr[i]) {
            printf("Enter data of subscriber\n");
            getchar();
            fgets(arr[i].name, 10, stdin);
            fgets(arr[i].surname, 10, stdin);
            fgets(arr[i].phone_number, 10, stdin);
            fill_arr[i] = 1;
            return;
        }
    }

    printf("Your list is full\n");
    printf("Press any key to continue\n");
    pause();
}

void PrintList(struct Subscriber* arr, unsigned char* fill_arr, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        if (fill_arr[i]) {
            printf("%d note\n", i + 1);
            printf("%s\n", arr[i].name);
            printf("%s\n", arr[i].surname);
            printf("%s\n", arr[i].phone_number);
        }
    }
    printf("Press any key to continue\n");
    pause();
}

int main(void) {
    const unsigned int n = 5;
    struct Subscriber arr[n];
    unsigned char fill_arr[n];
    char file_name[32];
    FILE* file;

    while (1) {
        system("clear"); 
        Menu();
        unsigned char key = getchar();
        system("clear");
        switch (key) {
        case '0':
            OpenList(file, file_name, arr, fill_arr, n);
            break;
        case '1':
            CreateList(file, file_name, arr, fill_arr, n);
            break;
        case '2':
            AddNote(arr, fill_arr, n);
            break;
        case '3':
            break;
        case '4':
            PrintList(arr, fill_arr, n);
            break;
        case '5':
            break;
        case '6':
            SaveChanges(file, arr, fill_arr, n);
            fclose(file);
            return 0;
            break;
        default:
            IncorrectKey();
            break;
        }

    }

    return 0;
}