#include <stdio.h>

typedef union {
	short count;
	float weight;
	float volume;
} quantity;

typedef struct {
	const char *name;
	const char *country;
	quantity amount;
} fruit_order;

int main()
{
	quantity q = {4};

	printf("The weight is:%2f\n", q.weight);
	printf("The count is:%i\n", q.count);
	
	q.weight = 5.6;
	printf("The weight02 is:%2f\n", q.weight); // is 5.6
	printf("The count02 is:%i\n", q.count); // is ?????

	quantity q2 = {.volume=40.5};
	printf("The q2.volume is:%2f\n", q2.volume);

	// fruite order
	fruit_order apples = {"apples", "England", .amount.weight=4.2};
	printf("This order contains %2.2fbs of %s\n", apples.amount.weight, apples.name);

	return 0;
}
