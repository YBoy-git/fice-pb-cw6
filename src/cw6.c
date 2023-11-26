#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void clearIB()
{
    while (getchar() != '\n')
        ;
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

bool isOptionValid(char choice, const char *options)
{
    if (!strchr(options, choice))
    {
        printf("Invalid option, try again\n");
        return false;
    }
    return true;
}

void getOption(char *choice, const char *options, const char *prompt)
{
    do
    {
        getInput(choice, "%c", prompt);
    } while (!isOptionValid(*choice, options));
}

void UI()
{

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
    // endless(UI);

    return 0;
}
