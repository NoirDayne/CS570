/*
Nash Granstrom
John Carroll
CS570
Program 4
Due 4/25/2018


Description:
Program 2 (p2) is a further expansion of our Program 1 (getword.c) which uses 
getword.c to determine what "words" are considered which was decided by our
program guidelines.
p2 is a shell that interprets these words that we get an uses the parse function
to seperate the words and decide what commands to execute after they are 
determined. There are special cases for p2, most importantly:

p2 skips input lines that contain 0 words, and simply reissues the prompt.
p2 terminates when it sees an input line whose first word is the end of file
marker [EOF].  Some of the metacharacters ("<", ">", "|", "&", "#", and "|&")
have special significance to p2; see below.

The three line terminators are "&", EOF, and newline.  If the response from
getword() indicates you have one of these three situations ["&" in the storage
array with a return value of -1, return value of 0 or -10, respectively], then
it is time to leave parse() and fork() a child who will execute the command.

p2 understands two commands that were instructed to be implemented, cd and MV.

*/




#include "p2.h"

#include "getword.h"


int stdin_result; 	//saves result of stdin dup
int stdout_result; 	//saves result of stdout dup
char *infile; 		//points to infile in buffer
char *outfile; 		//points to outfile in buffer
int size;		//word size retrieved from getword()
int move = 0;		//how much to move through our buffer
char *newargv[MAXITEM];
char w[STORAGE*MAXITEM];

/*
Flag Declarations
*/

int inFlag = 0; 	//<
int outFlag = 0; 	//>
int hashtagFlag = 0;    //#
int ampersandFlag = 0;  //&
int pipeFlag = 0;	//|

pid_t dead_child; 	//'dead' child




	/* 
	main() function
	*/	

int main(){
	
		
	(void) setpgid(0, 0);			//this is an outset for killpg command
	(void) signal(SIGTERM, sigHandler);	//sigHandler()

	pid_t kidpid; //child

	char *home;
	home = getenv("HOME"); 			//change directory to $HOME when 'cd' is called with no args

	int linkData;
	int openedFile;
	int word_count;
	int inRed;
	int outRed;
	int pipeLocal;

	int dup_out;
	int close_out;
	int output_dest;
	int flag;
	int mode;
	
	stdin_result = dup(STDIN_FILENO);	
	stdout_result = dup(STDOUT_FILENO);
	

	(void) printf("p2: ");

	for(;;) {

		word_count = parse();
	
		fflush(stdout);

		/*
		Beginning of Flag functionality
		*/
		
		if(strcmp(newargv[0], "\n") == 0){
			break;
		}

	
		//| - this flag is the beginning of our vertical piping
		if (pipeFlag != 0) {
		    	int verticalPipe(int pipeFlag){

			pid_t pid;
			pid_t childpid;
			pid_t gchildpid;

			int p[20];

   			fflush(stdout);		//fflush
			fflush(stderr);
   			
			if((childpid = fork()) == 0) {
				int kidTrack = 0;			
		
				while(kidTrack < pipeFlag) {	
					pipe(p + 2*kidTrack);

					fflush(stdout);			//fflush
					fflush(stderr);			
					if((gchildpid = fork()) == 0) { 
						kidTrack++;		

						if(kidTrack <= pipeFlag) {	
							if(dup2(p[2*(kidTrack)-1], STDOUT_FILENO) < 0) {					
								exit(EXIT_FAILURE);
							}
							close(p[(2*kidTrack - 1)]);
						}
						if(kidTrack == pipeFlag) {
							if(outFlag != 0) { 
								dup2(inRed, STDIN_FILENO);
								close(inRed);
							}
						}	
						continue;
					}
			
					if(kidTrack != pipeFlag) {
						if(dup2(p[2*kidTrack], STDIN_FILENO) < 0)
							exit(EXIT_FAILURE);
						close(p[(2*kidTrack)+1]);
					}

					if(kidTrack == 0) {
						if(outFlag != 0) {
							dup2(outRed, STDOUT_FILENO);
							close(outRed);	
						}
					}
					break;
				}
				if(kidTrack == pipeFlag) {
			if((execvp(newargv[0], newargv)) < 0) {
				fprintf(stderr, "%s: Error\n", newargv[0]);
				exit(0);
			}
		}

		if(ampersandFlag != 0 && inFlag == 0) {
			int i;
			int flags = O_RDONLY;
			if((i = open("/dev/null", flags)) < 0) {
				fprintf(stderr, "error dev/null\n");
				exit(0);
			}
			dup2(i,STDIN_FILENO);
			close(i);
		}

		if(ampersandFlag != 0) {
			printf("%s [%d]\n", *newargv, childpid);
			ampersandFlag = 0;
		}
		else {
			for(;;) {
			pid = wait(NULL);
			if(pid = childpid);
				break;
				}
			}
		}
			
}


		}
 
		//#	
		if(hashtagFlag > 0 && word_count == 0){
			hashtagFlag = 0;
			continue;
		}
		
		//&
		if(ampersandFlag != 0){
			(void) fflush(stdout);
			if((kidpid = fork()) == -1){
				perror("fork error");
				exit(EXIT_FAILURE);

			} 
			else if(kidpid == 0) { 
				(void) fflush(stdout);
				execvp(newargv[0], newargv);
				perror("execvp error"); 
				exit(EXIT_FAILURE);

			} 
			ampersandFlag = 0;
			continue;
		}
		

		//<
		if(inFlag == 1){
			
		int inFileFunc(){

			char* filename = infile;

			flag = O_RDONLY | O_CREAT;
			mode = S_IROTH | S_IRUSR | S_IRGRP;

			if((output_dest=open(filename, flag, mode)) < 0){
				perror("error");
				return output_dest;
			}
			if((dup_out = dup2(output_dest, STDIN_FILENO)) < 0){
				perror("error");
				return dup_out;
			}
			if((close_out = close(output_dest)) < 0){
				perror("error");
			} 
			else {

				return close_out;
			}



		}
		if(openedFile = ((inFileFunc()) < 0)){
				perror("error");
				fflush(stdout);
				continue;
			}
			inFlag = 0;




		}


		//>
		if(outFlag == 1){

		int outFileFunc(){
	
			char* filename = outfile;

			flag = O_RDWR | O_CREAT;
			mode = S_IROTH | S_IRUSR | S_IRGRP;
			if((output_dest=open(filename, flag, mode)) < 0){
				perror("error");
				return output_dest;
			}
			if((dup_out = dup2(output_dest, STDOUT_FILENO)) < 0){
				perror("error");
			}
			if((close_out = close(output_dest)) < 0){
				perror("error");
			} 
			else {

				return close_out;
			}
		}
		if(openedFile = ((outFileFunc()) < 0)){
				perror("error");
				fflush(stdout);
				continue;
			}
			outFlag = 0;

		}
		
		//EOF -- be careful with killpg()!
		if(word_count == EOF){
			printf("\nEOF found!\n");
			killpg(getpgrp(), SIGTERM);
			break;
		}


		/*
		cd command
		*/

		if(strcmp(newargv[0], "cd") == 0){
			if(newargv[1] == NULL){
				chdir(home);
			} 
			else if(newargv[2] != NULL){
				printf("Argument error");
			} 
			else {
				chdir(newargv[1]);
				newargv[1] = NULL;
			}

			continue;
		}

		/*
		MV command
		*/

		if(strcmp(newargv[0], "MV") == 0){
			if(newargv[1] == NULL || newargv[2] == NULL || newargv[3] != NULL){
				printf("MV error");
			} 
			else {
				if((linkData = link(newargv[1], newargv[2])) < 0){
					perror("link error");
					continue;
				}

				if((linkData = unlink(newargv[1])) < 0){
					perror("unlink error");
				}
			}

			continue;
		}


		//fork() start

		if((kidpid = fork()) == -1){ 
			perror("fork error");
			exit(EXIT_FAILURE);

		} 

		else if (kidpid == 0) { 
			
			execvp(newargv[0], newargv);
			perror("exec error");   
           		exit(EXIT_FAILURE);

		} 
		else {

			for(;;){
				dead_child = wait(NULL);
				if (dead_child == kidpid)
				{
					break;
				}
			}
			(void) dup2(stdin_result, STDIN_FILENO); 
			(void) dup2(stdout_result, STDOUT_FILENO); 
			(void) close(stdin_result);
			(void) close(stdout_result);

		}
	}

	printf("p2 terminated.\n");
	return 0;
}





	/*
	parse() function
	*/
int parse(){

	int word_size; 
	int count = 0; 
	int i = 0; 

	while((size = getword(w + move)) != -10){

		if(*(w + move) == '>' || *(w + move) == '<'){
			if(*(w + move) == '>'){
				move+=2;
				outFlag = 1;
				size = getword(w + move); 
				outfile = (w + move); 
			} 
			else{
				move+=2;
				inFlag = 1;
				size = getword(w + move); 
				infile = (w + move); 
			}			
			word_size = abs(size);
			move += word_size + 1;
			count++;
			continue;
		}

		if(*(w + move) == '|'){
			move+=2;
			pipeFlag = i;
			count++;
			continue;
		}

		if(*(w + move) == '#' && count == 0){
			hashtagFlag = 1;
			move+=2;
			continue;
		}


		if(*(w + move) == '&'){
			move+=2;
			ampersandFlag = i;
			return EOF;
		}


		if(size == 0 && count == 0){
			return EOF;
		}

		newargv[i] = w + move;
		i++;
		newargv[i] = NULL;

		word_size = abs(size);
		move += word_size + 1;
		count++;

	}

	newargv[i] = '\0'; 
	return count;
}






