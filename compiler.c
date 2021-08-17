#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ctype.h"
#include "token.h"
#include "writer.h"

#define MAX_LENGTH 250

int main(int argc, char* argv[]) {
	FILE* inputFile;
	FILE* outputFile;
	token* theToken;
	stack theStack;
	init_stack(&theStack);
	char *filename = malloc(strlen(argv[1]) + sizeof(char) * 2);
	
	filename = strncpy(filename, argv[1], strlen(argv[1])-2);
	strcat(filename, ".asm");
	printf("%s", filename);

	inputFile = fopen(argv[1], "r");
	if(inputFile == NULL) {
		printf("ERROR: input File is null\n");
		return 0;
	}

	outputFile = fopen(filename, "w");
	char line [MAX_LENGTH];
	char* tokens;
	
	while(!feof(inputFile)) {
		fgets(line , 250, inputFile);
		if (strcmp(line, "\n") == 0 || line[0] == ';') {
      		continue;
      	}
      	tokens = strtok(line , " \n\t\f\v\r");
      	while(tokens != NULL) {
			if(tokens[0] == ';') {
				break;
			}
			theToken = malloc(sizeof(*theToken));
			int valid_token = read_token(theToken, tokens);
			if(valid_token == 0) {
				int valid_writer = write_asm(theToken, outputFile, &theStack);
				if(valid_writer == 1) {
					free(filename);
					free(theToken);
					return 0;
				}
				
			} else {
				free(filename);
				free(theToken);
				return 0;
			}
			free(theToken);
			tokens = strtok(NULL, " \n\t\f\v\r");
		}
	}
	free(filename);
	fclose(outputFile);
	fclose(inputFile);

	return 0;

}
