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
    int error;

    while(1) {
        a = 0;
        b = 0;
        error = 0;
        error = system("clear");
        if (error) {
            perror("System error");
            exit(1);
        }
        Menu();
        unsigned char key = getc(stdin);
        error = system("clear");
        if (error) {
            perror("System error");
            exit(1);
        }
        switch (key) {
        case '1':   
            puts("Addition\nEnter a and b (a + b)");
            error = scanf("%d%d", &a, &b);
            if (error == EOF) {
                perror("Error!");
                continue;
            }
            fprintf(stdout, "Result %d\n", Addition(a, b));
            getchar();
            getchar();
            break;
        case '2':
            puts("Substraction\nEnter a and b (a - b)");
            error = scanf("%d%d", &a, &b);
            if (error == EOF) {
                perror("Error!");
                continue;
            }
            fprintf(stdout, "Result %d\n", Substraction(a, b));
            getchar();
            getchar();
            break;
        case '3':
            puts("Multiplication\nEnter a and b (a * b)");
            error = scanf("%d%d", &a, &b);
            if (error == EOF) {
                perror("Error!");
                continue;
            }
            fprintf(stdout, "Result %d\n", Multiplication(a, b));
            getchar();
            getchar();
            break;
        case '4':
            puts("Division\nEnter a and b (a - b)");
            error = scanf("%d%d", &a, &b);
            if (error == EOF) {
                perror("Error!");
                continue;
            }
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