/*
Nash Granstrom
CS570 Carroll
Program Zero
Due 1/25/2018
Program Zero : Simple lexical analyzer
*/


#include "getword.h"
#include <stdbool.h>

int getword(char *w){
	int c;
	int count = 0;


	//eliminates leading spaces

	while((c = getchar()) == ' ') {
		;
	}
	
	//checks for a new line character

        if (c == '\n') {
                *w = '\0';
                return -10;
        }

	//checks for pound character, only when pound character starts a new word
	
	if (c == '#') {
		*w++ = (char) c;
		*w = '\0';
		return -1;
	}

	//checks for end of file

	if (c == EOF) {
		*w = '\0';
		return 0;
	}
	
	//if not leading space, new line, pound sign or end of file then add to array

	*w++ = (char) c;
	count++;

	//enter while loop for adding characters to array

	while (true) {
		c = getchar();
		switch (c) {
			case EOF:
				*w = '\0';
				return count;
			case '\n':
				*w = '\0';
				ungetc('\n', stdin);
				return count;
			case ' ':
				*w = '\0';
				ungetc(' ', stdin);
				return count;
			default:
				*w++ = (char) c;
				count++;
				}
			}
		}
	
