#include <stdio.h>

int main()
{
    char num1, num2, num3, num4;

    printf("Enter the numbers:" );

    scanf("%c %c %c %c", &num1, &num2, &num3, &num4);
    printf("\nThe ASCII character is: %u, %u, %u, %u\n", num1, num2, num3,num4);

    while(getchar() != '\n')
    {

    }
    getchar();
    return 0;
}
