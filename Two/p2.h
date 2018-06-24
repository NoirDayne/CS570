/*
Nash Granstrom
Carroll CS570
Assignment #2
Due: 3/7/2018
*/


#include <stdio.h>
#include "getword.h"
#define MAXITEM 100 /* max number of words per line */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>


void sigHandler(){}

int parse();
int verticalPipe();
int inFileFunc();
int outFileFunc();






