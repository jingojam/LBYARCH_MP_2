/*
    Arano, Christian LBYARCH - S14
    Manaois, Raidon LBYARCH - S14
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//external assembly function call
extern void imgCvtGrayFloatToInt(float** grayScale, int** converted, int rows, int cols);

int main() {
    int rows, cols;
    srand(time(NULL));

    //user prompt for rows and column sizes
    printf("Enter the number of rows: ");
    scanf("%d", &rows);
    printf("Enter the number of columns: ");
    scanf("%d", &cols);

    //dynamically allocate 2D arrays for both the inputs and outputs
    float** grayScale = (float**)malloc(rows * sizeof(float*)); //allocate an array of pointers (of type float) for the input
    int** converted = (int**)malloc(rows * sizeof(int*)); //allocate an array of pointers (of type int) for the output

    for (int i = 0; i < rows; i++) { //allocate memory for each row
        grayScale[i] = (float*)malloc(cols * sizeof(float));
        converted[i] = (int*)malloc(cols * sizeof(int));
    }

    //user prompt for float value inputs (grayscale floats)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter value at [%d,%d]: ", i, j);
            scanf("%f", &grayScale[i][j]);
        }
    }

    printf("\n");

    printf("ORIGINAL IMAGE:\n"); // print out input values in a matrix format
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", grayScale[i][j]);
        }
        printf("\n");
    }

    clock_t start_time = clock(); //starting time

    //call external asmfunc (refer to the accompanying assembly file)
    imgCvtGrayFloatToInt(grayScale, converted, rows, cols);

    clock_t end_time = clock(); //ending time

    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nAssembly function execution time: %.6f seconds\n", time_taken);

    printf("\n");

    //display the new values after the call
    printf("OUTPUT IMAGE:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", converted[i][j]);
        }
        printf("\n");
    }

    //free the allocated arrays by row
    for (int i = 0; i < rows; i++) {
        free(grayScale[i]);
        free(converted[i]);
    }

    free(grayScale);
    free(converted);

    return 0;
}
