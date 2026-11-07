
#include<stdio.h>
#include <math.h>

int main (void)
{
	double charge, chargeE;
	double electrons;

	printf("Enter the charge:\n");
	scanf("%lf", &charge);

	printf("Enter the charge of an electron:\n");
	scanf("%le", &chargeE);

	electrons = (charge/chargeE) *-1;
	printf(" The number of electrons are : %le", electrons);

	while(getchar() != '\n')
	    {

	    }
	    getchar();
	    return 0;
}
