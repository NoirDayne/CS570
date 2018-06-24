/*
Nash Granstrom
CS570 Carroll
Assignment #1
Due 2/9/2018

Description:
Program 1 is a further expansion of Program 0, which is a simple lexical analyzer.
In Program 1 we aim to add metacharacters in the getword() function. It returns 0
when there is no word and only end-of-file is encountered. It returns -10 when there
is no word and only newline is encountered, otherwise it will return the amount
of numbers in the word. It will return the negative value for metacharacters entered.
The metacharacters are "<",">","|","&","#" and the combination "|&". getword() skips
leading spaces by use of the rmleadspace() function. The backslash character "\"
is a special character, it changes the behaviour of what directly follows it 
because if it is a metacharacter, the metacharacter gets treated like a normal
character. If the word scanned is longer than STORAGE-1 (STORAGE is defined in getword.h)
then getword() constructs the string consisting of the first bytes only.


*/


#include "getword.h"
#include <stdbool.h>

//function written at the bottom
static int rmleadspace(void);


//begin getword
int getword(char *w) {

	int c, count = 0;
	char *n = w;

top: 
	//function that removes all leading white space
	c = rmleadspace();
	//begin switch cases for beginning of words
	switch(c){
		case ';':
		case '\n':
			*n = '\0';
			return -10;
		case EOF:
			*n = '\0';
			return 0;
		case '<':
		case '>':
		case '#':
		case '&':
			*n++ = (char) c;
			*n = '\0';
			return -1;
		case '|':

			*n++ = '|';
			c = getchar();
			if (c == '&'){
				// "|&"
				*n++ = '&';
				*n = '\0';
				return -2;
			}

			// here we use the function ungetc()
			// to place the next char onto stdin
			ungetc(c, stdin);
			*n = '\0';
			return -1;


		}
		
		do {
			//begin switch cases for all other scenarios
			switch(c) {
				case '\t':
				case ' ':
				case EOF:
					//end of word
					*n = '\0';
					goto end;
				case '>':
				case '<':
				case '|':
				case '&':
				case ';':

				//it is important to use ungetc() here
				//in order to make a new line a delimiter
				case '\n':
					*n = '\0';
					ungetc(c, stdin);
					goto end;
				case '\\':
					c = getchar();

					//special case for when it is a new line
					if (c == '\n') {
						*n = '\0';
						ungetc(c, stdin);
						goto end;
					}
					//special case when it is end of file
					if (c == EOF) {
						*n = '\0';
						ungetc(c, stdin);
						goto end;
					}
					if (c == '\\') {
						*n++ = '\\';
						count++;
						continue;
					}
				default:
					*n++ = (char) c;
					count++;
					//here we use __builtin_expect to check to see
					//if the buffer has gone over the storage limit
					//if so it returns the current word and count
					if (__builtin_expect ((count == STORAGE - 1), 0)){
						*n = '\0';
						return count;
					}
				}
			}
			while ((c = getchar()), true);
		end:

			return count;
		}
		
		//function to remove all leading spaces before entering switch cases
		static int rmleadspace(void){
			int c;
			while (true) {
			c = getchar();
			switch(c){
				case '\t':
				case ' ':
					continue;
				default:
					return c;
			}
		}
	}

					
