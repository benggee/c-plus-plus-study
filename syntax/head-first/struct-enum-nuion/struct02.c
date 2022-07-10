#include <stdio.h>

struct perference {
	const char *food;
	float exercise_hours;
};

struct fish {
	const char *name;
	const char *species;
	int teeth;
	int age;
	struct perference care;
};

int main()
{
	struct fish snappy = {"Snappy", "Piranha", 69, 4, {"Meat",7.5}};
	printf("Snappy like eat %s\n", snappy.care.food);
	printf("Snappy like do %f hours\n", snappy.care.exercise_hours);
	return 0;
}
