#include <stdio.h>

typedef struct island {
	char *name;
	char *open_time;
	char *close_time;
	struct island *next;
} island;

 void display(island *i);

int main()
{
	island amity = {"Amity", "09:00", "17:00", NULL};
	island craggy = {"Craggy", "09:00", "17:00", NULL};
	island isla_nublar = {"Isla Nublar", "09:00", "17:00", NULL};
	island shutter = {"Shutter", "09:00", "17:00", NULL};

	amity.next = &craggy;
	craggy.next = &isla_nublar;
	shutter.next = &shutter;
	display(&amity);

	return 0;
}

void display(island *start)
{
	island *i = start;
	for (; i != NULL; i = i->next) {
		printf("Name: %s\n open: %s-%s\n", i->name, i->open_time, i->close_time);
	}
}
