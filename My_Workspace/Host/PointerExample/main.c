#include <stdio.h>

int main(void)
{
	char data = 100;
	char* pAddress = &data;
	char data2 = *address;

	printf("The address of the variable is: %p\n", &data);
	printf("The value in data2 is: %d\n", data2);

	*address = 65;


	printf("The value of data is: %d\n", data);

	while(getchar() != '\n')
			{

			}
			getchar();
			return 0;

}
