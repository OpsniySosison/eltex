#include "abonent.h"

void Menu(void) {
    puts("Main menu\nSelect an action");
    puts("1 - Open list from file");
    puts("2 - Create list (becomes open by default)");
    puts("3 - Add note");
    puts("4 - Delete note");
    puts("5 - Print list");
    puts("6 - Search by any field");
    puts("7 - Close the program");
}

int main(void) {
    struct List* head = NULL;
    FILE* file = NULL;
    
    while(1) {
        system("clear");
        Menu();
        unsigned char key = getchar();
        system("clear");
        switch (key) {
        case '1':
            LoadList(&file, &head);
            break;
        case '2':
            CreateFile(&file, &head);
            break;
        case '3':
            AddNote(head);
            break;
        case '4':
            SearchDeleteNote(head);
            break;
        case '5':
            PrintList(head);
            break;
        case '6':
            SearchInTheList(head);
            break;
        case '7':
            SaveChanges(file, head);
            return 0;
            break;
        }
    }
    
    return 0;
}