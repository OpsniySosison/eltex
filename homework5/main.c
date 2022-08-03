#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"

void Menu(void) {
    puts("Calculator");
    puts("Select an action");
    puts("1 - Addition");
    puts("2 - Substraction");
    puts("3 - Multiplication");
    puts("4 - Division");
    puts("5 - Exit");
}

int main(void) {
    int a;
    int b;

    while(1) {
        a = 0;
        b = 0;
        system("clear");
        Menu();
        unsigned char key = getc(stdin);
        system("clear");
        switch (key) {
        case '1':   
            puts("Addition\nEnter a and b (a + b)");
            scanf("%d%d", &a, &b);
            fprintf(stdout, "Result %d\n", Addition(a, b));
            getchar();
            getchar();
            break;
        case '2':
            puts("Substraction\nEnter a and b (a - b)");
            scanf("%d%d", &a, &b);
            fprintf(stdout, "Result %d\n", Substraction(a, b));
            getchar();
            getchar();
            break;
        case '3':
            puts("Multiplication\nEnter a and b (a * b)");
            scanf("%d%d", &a, &b);
            fprintf(stdout, "Result %d\n", Multiplication(a, b));
            getchar();
            getchar();
            break;
        case '4':
            puts("Division\nEnter a and b (a + b)");
            scanf("%d%d", &a, &b);
            fprintf(stdout, "Result %d\n", Division(a, b));
            getchar();
            getchar();
            break;
        case '5':
            puts("The program was closed");
            return 0;
        }
    }

    return 0;
}