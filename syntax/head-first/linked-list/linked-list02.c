#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct island {
	char *name;
	char *open_time;
	char *close_time;
	struct island *next;
}island;

void display(island *i);
void release(island *i);
island* create(char *name);

// ./tour < trip1.txt
int main()
{
	island *land = NULL;
	island *next = NULL;
	island *flag = NULL;
	char land_name[80];
	for (;fgets(land_name, 80, stdin)!=NULL;land = next) {
		next = create(land_name);	
		if (flag == NULL) {
			flag = next;
		}
		if (land != NULL) {
			land->next = next;
		}
	}
	display(flag);
	release(flag);
	return 0;
}

island* create(char *name)
{
	island *i = malloc(sizeof(island));
	i->name = strdup(name);
	i->open_time = "09:30";
	i->close_time = "22:00";
	i->next = NULL;
	return i;
}

void display(island *start)
{
	island *i = start;	
	for (;i != NULL;i = i->next) {
		printf("Name: %sOpen Time:%s-%s\n", i->name, i->open_time, i->close_time);
	}
}

void release(island *start)
{
	island *i = start;
	island *next = NULL;
	for (;i!=NULL;i=next) {
		next = i->next;
		free(i->name);
		free(i);
	}
}
