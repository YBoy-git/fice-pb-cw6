#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

const double MAX_PRECISION = 1e-15;
const int MAX_ROWS = 100;
const int MAX_INPUT = 1 << 10;

struct Matrix
{
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
    double *xPrevious = malloc(sizeof(double) * matrix_s->rows);
    wasMemoryAllocated(xPrevious);
    calculateStartX(xPrevious, matrix_s);
    double *xCurrent = malloc(sizeof(double) * matrix_s->rows);
    wasMemoryAllocated(xCurrent);

    for (int i = 0; i < matrix_s->rows; i++)
    {
        printf("x%d = %lg\n", i + 1, xPrevious[i]);
    }
    do
    {
        calculateNextX(xCurrent, xPrevious, matrix_s);
    } while (maxDelta(xCurrent, xPrevious, matrix_s->rows) > precision);

    free(xPrevious);
    return xCurrent;
}

bool isRowConvergent(double *row, int rowNo, int rowLength)
{
        double sum = 0;
        for (int j = 0; j < rowLength - 1; j++)
        {
            if (j != rowNo)
            {
                sum += row[j];
            }
        }
        if (fabs(row[rowNo]) < fabs(sum))
        {
            printf("Row %d is not convergent\n", rowNo + 1);
            return false;
        }
    return true;
}

bool UI_scanInput(char *input, double *row, int maxSize, int rowNo)
{
    int count = 0;
    int n;
    char *ptr = input;

    while (sscanf(ptr, "%d", &n) == 1)
    {
        row[count] = n;
        count++;

        // Move the pointer to the next integer
        while (*ptr && (*ptr != ' ' && *ptr != '\n'))
        {
            ptr++;
        }

        // Skip any whitespace
        while (*ptr == ' ' || *ptr == '\n')
        {
            ptr++;
        }
    }

    if (count > maxSize)
    {
        printf("Too many elements in the row, try again\n");
        return false;
    }

    if (count < maxSize)
    {
        printf("Too few elements in the row, try again\n");
        return false;
    }

    if (row[rowNo] == 0) 
    {
        printf("Zeroes are not allowed on the main diagonal\n");
        return false;
    }

    if (!isRowConvergent(row, rowNo, maxSize))
    {
        return false;
    }

    return true;
}

struct Matrix UI_getMatrix()
{
    struct Matrix matrix_s;

    int rows = 0;
    char rowsPrompt[70];
    sprintf(rowsPrompt, "Enter the number of rows (max: %d): ", MAX_ROWS);
    getInput(&rows, "%d", rowsPrompt);
    double **matrix = malloc(sizeof(double *) * rows);
    wasMemoryAllocated(matrix);

    matrix_s.matrix = matrix;
    matrix_s.rows = rows;

    for (int i = 0; i < rows; i++)
    {
        matrix[i] = calloc(rows + 1, sizeof(double));
        wasMemoryAllocated(matrix[i]);
    }

    char input[MAX_INPUT];
    int count = 0;
    printf("Enter the matrix row by row, each element separated by a space, exactly %d elements in each row\n", rows + 1);
    do
    {
        printf("Row %d: ", count + 1);
        do
        {
            fgets(input, MAX_INPUT, stdin);
        } while (!UI_scanInput(input, matrix[count], rows + 1, count));

        count++;
    } while (input != "\n" && count < rows);

    return matrix_s;
}

double validatePrecision(double precision)
{
    if (precision >= 1.0)
    {
        int count = (int)precision;
        precision = 1.0;
        while (count > 0)
        {
            precision /= 10.0;
            count--;
        }
    }
    return precision;
}

bool isPrecisionValid(double precision)
{
    if (precision <= 0)
    {
        printf("Precision must be positive, try again\n");
        return false;
    }

    if (precision < MAX_PRECISION)
    {
        printf("Max precision is %e, try again\n", MAX_PRECISION);
        return false;
    }

    return true;
}

void getPrecision(double *precision)
{
    do
    {
        char message[70];
        sprintf(message, "Enter the precision (max: %lg): ", MAX_PRECISION);
        getInput(precision, "%lf", message);
        *precision = validatePrecision(*precision);
    } while (!isPrecisionValid(*precision));
}

void UI()
{
    struct Matrix matrix_s = UI_getMatrix();
    double precision = 0.0;
    getPrecision(&precision);
    double *solution = solveLinearSystem(&matrix_s, MAX_PRECISION);
    if (solution != NULL)
    {
        char solutionFormat[25];
        sprintf(solutionFormat, "%%d: %%.%dlg\n", (int)(-log10(precision)));
        for (int i = 0; i < matrix_s.rows; i++)
        {
            printf(solutionFormat, i + 1, solution[i]);
        }
        printf("\n");
    }

    free(solution);
    for (int i = 0; i < matrix_s.rows; i++)
    {
        free(matrix_s.matrix[i]);
    }
    free(matrix_s.matrix);

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

int main()
{
    endless(UI);
    return 0;
}
