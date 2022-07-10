#include <stdio.h>
#include <stdlib.h>


int main() 
{
	char card_name[3];
	//int val = 0; 
	//puts("Please Input Card Name:");
	//scanf("%2s", card_name);

	/*if (card_name[0] == 'K') {
		val = 10;
	} else if (card_name[0] == 'Q') {
		val = 10;
	} else if (card_name[0] == 'J') {
		val = 10;
	} else if (card_name[0] == 'A') {
		val = 11;
	} else {
		val = atoi(card_name);
	}*/

	/*switch (card_name[0]) {
		case 'K':
		case 'Q':
		case 'J':
			val = 10;
			break;
		case 'A':
			val = 11;
			break;
		default:
			val = atoi(card_name);
			break;
	}

	if ((val>2) && (val<7)) {
		puts("Plus one.");
	} else if (val == 10) {
		puts("Plus -one.");
	} */

	int count = 0;
	do {
		puts("Please Input Card Name:");
		scanf("%2s", card_name);
		int val = 0;
		switch (card_name[0]) {
			case 'K':
			case 'Q':
			case 'J':
				val = 10;
				break;
			case 'A':
				val = 11;
				break;
			default:
				val = atoi(card_name);
				if (val<1 || val > 10) 
					printf("Wath ??\n");
					continue;
					
		}
		if ((val>2) && (val<7)) {
			count++;
			puts("Plus one.");
		} else if (val == 10) {
			count--;
			puts("Plus -one.");
		} 
		printf("Current count:%i\n", count);
	} while (card_name[0]!='X');
	
	return 0;
}
