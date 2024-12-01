#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern void imgCvtGrayFloatToInt(float** grayScale, int** converted, int rows, int cols);

/*
    these functions creates .txt files for input/output testing
*/
void generateInputFile(int rows, int cols) { //create a sample input matrix of size rows*cols
    FILE* file = fopen("INPUT.txt", "w");

    fprintf(file, "%d\n", rows);
    fprintf(file, "%d\n", cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float randomVal = (float)rand() / (float)RAND_MAX;
            fprintf(file, "%f\n", randomVal);
        }
    }
    fclose(file);
}

void loadInputFile(float** grayScale) { //load inputs from a file into the driver program
    FILE* file = fopen("INPUT.txt", "r");
    int rows, cols;

    fscanf(file, "%d\n", &rows);
    fscanf(file, "%d\n", &cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%f\n", &grayScale[i][j]);
        }
    }

    fclose(file);
}

int main() {
    int rows, cols;
    srand(time(NULL));

    //user prompt for rows and column sizes
    printf("Enter the number of rows: ");
    scanf("%d", &rows);
    printf("Enter the number of columns: ");
    scanf("%d", &cols);

    //since the specs doesn't specify maximum array sizes, we assume that arrays are dynamically allocated
    //here, we malloc() 2D arrays for both inputs and outputs
    float** grayScale = (float**)malloc(rows * sizeof(float*)); //malloc an array of pointers (of type float) for the input
    int** converted = (int**)malloc(rows * sizeof(int*)); //malloc an array of pointers (of type int) for the output

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

    printf("ORIGINAL:\n"); // print out input values in a matrix format
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", grayScale[i][j]);
        }
        printf("\n");
    }

    //call external asmfunc (refer to the accompanying assembly file)
    imgCvtGrayFloatToInt(grayScale, converted, rows, cols);

    printf("\n");

    //display the new values after the call
    printf("OUTPUT:\n");
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
