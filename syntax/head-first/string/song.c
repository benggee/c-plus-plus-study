#include <stdio.h>
#include <string.h>

char tracks[][80] = {
	"I left my heart in Harvard Med School",
	"Newark, Newark - a wonderful town",
	"Dancing with a Dork",
	"From here to maternity",
	"The girl from Iwo Jima",
};

void find_track(char search_for[])
{
	int i;
	for (i = 0; i < 5; i++) {
		
		if (strstr(tracks[i], search_for) != 0) {
			printf("Find The Song:", tracks[i]);
		}
	}
	printf("The end.\n");
}


int main()
{
	char search_key[80];
	
	printf("Please Type Song Name:\n");
	fgets(search_key, 80, stdin);

	search_key[strlen(search_key)-1] = '\0';

	find_track(search_key);

	return 0;
}
