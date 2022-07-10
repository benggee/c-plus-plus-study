#include <stdio.h>

struct exercise{
	const char *description;
	float duration;
};

struct meal {
	const char *ingredients;
	float weight;
};

struct perferences {
	struct meal food;
	struct exercise exercise;
};

struct fish {
	const char *name;
	const char *species;
	int teeth;
	int age;
	struct perferences care;
};

void  label(struct fish a)
{
	printf("Feed with %2.2f lbs of %s and allow to %s for %2.2f hours\n",
			a.care.food.weight, a.care.food.ingredients,
			a.care.exercise.description, a.care.exercise.duration);
}


int main() 
{	
	struct fish snappy = {"Snappy", "Piranha", 69, 4, {{"meat", 0.2},{"swim in the jacuxxi", 7.5}}};
	label(snappy);
	return 0;
}
