#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

struct Matrix {
    double **matrix;
    int rows;
};

void clearIB()
{
    while (getchar() != '\n')
        ;

    return;
}

void getInput(void *input, char *format, const char *prompt)
{
    char temp = 0;

    do
    {
        printf("%s", prompt);
        int status = scanf(format, input);
        if (*((char *)input) == '\n')
        {
            continue;
        }
        if (status != 1)
        {
            printf("Invalid input, the format is %s, try again\n", format);
            clearIB();
            continue;
        }
        status = scanf("%c", &temp);
        if (temp != '\n')
        {
            printf("Invalid input, the format is %s, try again\n", format);
            clearIB();
        }
    } while (temp != '\n');

    return;
}

bool wasMemoryAllocated(void *pointer)
{
    if (pointer == NULL)
    {
        printf("Memory allocation error\n");
        exit(1);
    }
    return true;
}

// bool isOptionValid(char choice, const char *options)
// {
//     if (!strchr(options, choice))
//     {
//         printf("Invalid option, try again\n");
//         return false;
//     }
//     return true;
// }

// void getOption(char *choice, const char *options, const char *prompt)
// {
//     do
//     {
//         getInput(choice, "%c", prompt);
//     } while (!isOptionValid(*choice, options));

//     return;
// }

bool isConvergent(const struct Matrix *matrix_s)
{
    for (int i = 0; i < matrix_s->rows; i++)
    {
        double sum = 0;
        for (int j = 0; j < matrix_s->rows; j++)
        {
            if (j != i)
            {
                sum += matrix_s->matrix[i][j];
            }
        }
        if (fabs(matrix_s->matrix[i][i]) < fabs(sum))
        {
            printf("Row %d is not convergent\n", i);
            return false;
        }
    }
    return true;
}

void calculateStartX(double *xPrevious, const struct Matrix *matrix_s)
{
    for (int i = 0; i < matrix_s->rows; i++)
    {
        xPrevious[i] = matrix_s->matrix[i][matrix_s->rows] / matrix_s->matrix[i][i];
    }

    return;
}

void calculateNextX(double *xCurrent, double *xPrevious, const struct Matrix *matrix_s)
{
    for (int i = 0; i < matrix_s->rows; i++)
    {
        double sum = 0;
        for (int j = 0; j < matrix_s->rows; j++)
        {
            if (j != i)
            {
                sum += matrix_s->matrix[i][j] * xPrevious[j];
            }
        }
        xCurrent[i] = (matrix_s->matrix[i][matrix_s->rows] - sum) / matrix_s->matrix[i][i];
    }

    return;
}

double maxDelta(double *xCurrent, double *xPrevious, int rows)
{
    double max = 0;
    for (int i = 0; i < rows; i++)
    {
        if (fabs(xCurrent[i] - xPrevious[i]) > max)
        {
            max = fabs(xCurrent[i] - xPrevious[i]);
        }
    }

    if (max == fabs(INFINITY))
    {
        printf("infinity appeared in maxDelta()\n");
        return 0;
    }

    for (int i = 0; i < rows; i++)
    {
        xPrevious[i] = xCurrent[i];
    }
    return max;
}

double *solveLinearSystem(const struct Matrix *matrix_s, double precision)
{
    if (!isConvergent(matrix_s))
    {
        printf("The matrix is not convergent\n");
        return NULL;
    }

    double *xPrevious = malloc(sizeof(double) * matrix_s->rows);
    wasMemoryAllocated(xPrevious);
    calculateStartX(xPrevious, matrix_s);
    double *xCurrent = malloc(sizeof(double) * matrix_s->rows);
    wasMemoryAllocated(xCurrent);

    do
    {
        calculateNextX(xCurrent, xPrevious, matrix_s);
    } while (maxDelta(xCurrent, xPrevious, matrix_s->rows) > precision);

    free(xPrevious);
    return xCurrent;
}

void UI()
{
    return;
}

void endless(void (*function)())
{
    do
    {
        function();
        printf("Press ENTER to continue and type any key to exit\n");
    } while (getchar() == '\n');

    return;
}

void test()
{
    struct Matrix matrix_s = {
        .matrix = malloc(sizeof(double *) * 3),
        .rows = 3};

    double **matrix = matrix_s.matrix;
    wasMemoryAllocated(matrix);
    int rows = matrix_s.rows;

    for (int i = 0; i < 3; i++)
    {
        matrix[i] = malloc(sizeof(double) * 4);
        wasMemoryAllocated(matrix[i]);
    }

    matrix[0][0] = 1; matrix[0][1] = -1; matrix[0][2] = 0; matrix[0][3] = 4;
    matrix[1][0] = 1; matrix[1][1] = 6; matrix[1][2] = 0; matrix[1][3] = 8;
    matrix[2][0] = 0.2; matrix[2][1] = 0; matrix[2][2] = 11; matrix[2][3] = 12;
    double *solution = solveLinearSystem(&matrix_s, 1e-15);
    if (solution != NULL)
    {
        for (int i = 0; i < 3; i++)
        {
            printf("%lg ", solution[i]);
        }
        printf("\n");
    }

    for (int i = 0; i < 3; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    free(solution);
    return;
}

int main()
{
    // endless(UI);
    test();
    return 0;
}
