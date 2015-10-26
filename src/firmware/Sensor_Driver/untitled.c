
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
int main ()
{

long christian = 0;
long david = 0;
long sargis = 0;
long ryan = 0;
int rando = 0;
long poop = (long)pow(2,322)-1;
long i = 0;
 srand (time(NULL));

 // initialize milli loop
for(i =1; i<=(poop);i++)
{
  /* initialize random seed: */
  
	rando = (int)((rand() % 4) + 1);
	printf("\ni = %ld",i);
	switch (rando)
	{
		case 1:
			christian++;
			break;
		case 2:
			david++;
			break;
		case 3:
			ryan++;
			break;
		case 4:
			sargis++;
			break;
		default:
			break;
	}

}
	printf("Christian: %ld\nDavid: %ld\nRyan: %ld\nSargis: %ld\n",christian,david,ryan,sargis);
	if((christian>david) && (christian>ryan) && (christian>sargis))
	{ puts("Christian Loser");
	}
	if((david>christian) && (david>ryan) && (david>sargis))
	{ puts("David is Loser");
	}
	if((sargis>christian) && (sargis>ryan) && (sargis>david))
	{ puts("Sargis is Loser");
	}
	if((ryan>christian) && (ryan>sargis) && (ryan>david))
	{puts("Ryan is Loser");
	}
	
  return 0;
}
