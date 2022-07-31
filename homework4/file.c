#include "abonent.h"

void SaveChanges(FILE* file, struct List* head) {
    if (!file) {
        puts("File dont open");
        return;
    }
    if (!head) {
        puts("Head is null");
        return;
    }
    struct List* temp = head;
    while(temp) {
        fwrite((struct Subscriber*)&temp->data, sizeof(struct Subscriber), 1, file);
        temp = temp->next;
    }
}

void LoadList(FILE** file, struct List** head) {
    getchar();
    if (*file) {
        SaveChanges(*file, *head);
        fclose(*file);
       if (*head) {
            DeleteList(*head);
        }
    }
    char file_name[32];
    sgets(file_name, 32);
    *file = fopen(file_name, "r+b");

    if (!*file) {
        puts("File not found");
        return;
    }

    *head = (struct List*)malloc(sizeof(struct List));
    struct List* temp = *head;

    while (!feof(*file)) {
        fread((struct Subscriber*)&temp->data, sizeof(struct Subscriber), 1, *file);
        temp->next = (struct List*)malloc(sizeof(struct List));
        temp->next->prev = temp;
        temp = temp->next;
    }
    temp->next = NULL;
}

void CreateFile(FILE** file, struct List** head) { 
    getchar();
    if (*file) {
        SaveChanges(*file, *head);
        fclose(*file);
        if (*head) {
            DeleteList(*head);
        }
    }

    char file_name[32];
    sgets(file_name, 32);
    *file = fopen(file_name, "w+b");
    *head = CreateList();

    char k = 0;

    do {
        AddNote(*head);
        puts("Netx? y/n");
        k = getchar();
    } while(k != 'n');
}