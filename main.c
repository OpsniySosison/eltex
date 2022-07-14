#include <stdio.h>

void FillMatrix(int* arr, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            arr[i * n + j] = i * n + j; 
        }
    }
}
 
void PrintMatrix(int* arr, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            printf("%d ", arr[i * n + j]);
        }
        printf("\n");
    } 
}

void PrintArray(int* arr, unsigned int n) {
      for (unsigned int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
}

void FillTreangleMatrix(int* arr, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) { 
            arr[i * n + j] = j < n - i ? 1 : 0;
        }
    }
}

void InputArray(int* arr, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void InvertArray(int* arr, unsigned int n) {
    for (unsigned int i = 0; i < n / 2; i++) {
        swap(&arr[i], &arr[n - i - 1]);
    }
}

void FillMatrixSpiral(int* arr, unsigned int n) {
    int count = 1;
    int k = 0;

    while (count <= n * n) {
        for (unsigned int i = k; i < n - k; i++) {
            arr[k * n + i] = count;
            count++;
        }

        if (count > n * n) break;

        for (unsigned int i = 1 + k; i < n - k; i++) {
            arr[(i + 1) * n - k - 1] = count;
            count++;
        }

        for (unsigned int i = 1 + k; i < n - k; i++) {
            arr[(n - k) * n - i - 1] = count;
            count++;
        }

        for (unsigned int i = n - k - 2; i > k; i--) {
            arr[i * n + k] = count;
            count++;
        }

        k++;
    }

}

int main() {
    const unsigned int n = 5;

    int arr[n][n];

    printf("first task\n\n");

    FillMatrix(arr, n);
    PrintMatrix(arr, n);

    printf("\nsecond task\n\n");

    int inv_arr[n];
    InputArray(inv_arr, n);
    InvertArray(inv_arr, n);
    printf("\n");
    PrintArray(inv_arr, n);

    printf("\n\nthird task\n\n");

    FillTreangleMatrix(arr, n);
    PrintMatrix(arr, n);

    printf("\nfourth task\n\n");

    FillMatrixSpiral(arr, n);
    PrintMatrix(arr, n);

    return 0;
} 