#include "abonent.h"

char* sgets(char* buff, unsigned int size) {
    fflush(stdin);

    int i, k = getchar();

    if (k == EOF)
        return NULL;

    for (i = 0; k != EOF && k != '\n' && i < size; i++) {
        buff[i] = k;
        k = getchar();

        if (k == EOF && !feof(stdin))
            return NULL;
    }

    buff[i] = '\0';

    return buff;
}

static void AddDataToNote(struct Subscriber* data) {
    sgets(data->name, 10);
    sgets(data->surname, 10);
    sgets(data->phone_number, 10);
}

struct List* CreateList(void) {
    struct List* temp;
    temp = (struct List*)malloc(sizeof(struct List));
    temp->next = NULL;
    temp->prev = NULL;
    AddDataToNote(&temp->data);
    return temp;
}

void DeleteList(struct List* head) {
    if (!head) {
        puts("Head is null");
        return;
    }
    struct List* temp = head;

    while(temp->next) {
        temp = temp->next;
        free(temp->prev);
    }
    free(temp);
}

struct List* AddNote(struct List* head) {
    static int call = 0;
    if (call) {
        getchar();
    }
    call++;
    if (!head) {
        puts("Head is null");
        return NULL;
    }
    struct List* note = head;
    while (note->next) {
        note = note->next;
    }

    note->next = (struct List*)malloc(sizeof(struct List));
    note->next->prev = note;
    note = note->next;
    note->next = NULL;
    AddDataToNote(&note->data);

    return note;
}

static void DeleteNote(struct List* note) {
    if (note->prev) { 
        note->prev->next = note->next;
    }
    if (note->next) {
        note->next->prev = note->prev;
    }
    free(note);
}

static void RedefineHead(struct List** head) {
    struct List* temp = *head;
    (*head)->next->prev = NULL;
    *head = (*head)->next;
    free(temp);
}

void SearchDeleteNote(struct List* head) {
    if (!head) {
        puts("Head is null");
        return;
    }
    struct List* temp = head;
    unsigned int number = 0;

    scanf("%u", &number);
    number--;

    if (!number) {
        RedefineHead(&head);
        return;
    }

    for (unsigned int i = 0; i < number; i++) {
        if (!temp->next) {
            puts("Note not found");
            getchar();
            return;
        }
        temp = temp->next;
    }
    DeleteNote(temp);
}

static void PrintNote(struct Subscriber data) {
    fprintf(stdout, "\nName: %s\nSurname: %s\nPhone number: %s\n", data.name, data.surname, data.phone_number);
}

void PrintList(struct List* head) {
    if (!head) {
        puts("Head is null");
        return;
    }
    struct List* temp = head;
    while(temp) {
        PrintNote(temp->data);
        temp = temp->next;
    }
    getchar();
    getchar();
}

static struct List* SearchName(struct List* head) {
    struct List* note = head;

    char name[10];
    sgets(name, 10);

    while(note) {
        if (!strcmp(note->data.name, name)) { 
            return note;
        }
        note = note->next;
    }

    return NULL;
}

static struct List* SearchSurname(struct List* head) {
    struct List* note = head;

    char surname[10];
    sgets(surname, 10);

    while(note) {
        if (!strcmp(note->data.surname, surname)) { 
            return note;
        }
        note = note->next;
    }

    return NULL;
}

static struct List* SearchPhoneNumber(struct List* head) {
    struct List* note = head;

    char phone_number[10];
    sgets(phone_number, 10);

    while(note) {
        if (!strcmp(note->data.phone_number, phone_number)) { 
            return note;
        }
        note = note->next;
    }

    return NULL;
}

void SearchInTheList(struct List* head) {\
    getchar();
    if (!head) {
        puts("Head is null");
        return;
    }
    puts("Select field\n1 - Name\n2 - Surname\n3 - Phone number\nOther key to cansel");
    unsigned char key = getchar();
    getchar();
    struct List* temp;
    switch (key) {
    case '1':
        temp = SearchName(head);
        break;
    case '2':
        temp = SearchSurname(head);
        break;
    case '3':
        temp = SearchPhoneNumber(head);
        break;
    default:
        return;
        break;
    }
    if (temp) {
        system("clear");
        PrintNote(temp->data);
        getchar();
    } 
    else {
        puts("Note not found");
        getchar();
    }
}