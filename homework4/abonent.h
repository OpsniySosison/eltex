#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef SUBSCRIBER
#define SUBSCRIBER
    struct Subscriber {
        char name[10];
        char surname[10];
        char phone_number[10];
    };
#endif

#ifndef LIST
#define LIST
    struct List {
        struct List* next;
        struct List* prev;
        struct Subscriber data;
    };

    void SearchInTheList(struct List* head);
    struct List* CreateList(void);
    void DeleteList(struct List* head);
    struct List* AddNote(struct List* head);
    void SearchDeleteNote(struct List* head);
    void PrintList(struct List* head);
    char* sgets(char* buff, unsigned int size);
#endif

#ifndef FILES
#define FILES
    void SaveChanges(FILE* file, struct List* head);
    void LoadList(FILE** file, struct List** head);
    void CreateFile(FILE** file, struct List** head);
#endif

#define DEBUG 1

#define dprint(fmt, ...) do { if (DEBUG) fprintf(stderr, "\ndebbug info in func %s(): "fmt, __func__, __VA_ARGS__);} while(0)

    