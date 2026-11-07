#include <stdio.h>

int main(void)
{
	int size = 0;
	int count = 0;
	int index = 0;
	printf("Enter the size of the pyramid");
	scanf("%d", &size);

	for(index=0; index<=size; index++)
	{
		for(count=index; count>0; count--)
		{
			printf(" %2d ", count);

		}
		printf("\n");
	}
	while(getchar() != '\n')
		    {

		    }
		    getchar();
	return 0;


}
