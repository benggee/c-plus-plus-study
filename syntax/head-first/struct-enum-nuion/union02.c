#include <stdio.h>

typedef union{
	float lemon;
	int lime_pieces;
} lemon_lime;

typedef struct {
	float tequila;
	float cointreau;
	lemon_lime citrus;
}margarita;

int main()
{
	margarita m1 = {2.0, 1.0, .citrus.lemon=2};
	printf("%2.1f measurs of tequila\n%2.1f measures of cointreau\n%2.1f measures of juice\n", 
			m1.tequila, m1.cointreau, m1.citrus.lemon);


	margarita m2 = {2.0, 1.0, 0.5};

	margarita m3 = {2.0, 1.0, {2}};

	return 0;
	
}


