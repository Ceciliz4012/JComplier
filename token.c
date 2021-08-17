/*
 * token.c
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ctype.h"
#include "token.h"
#include "writer.h"

#define MAX_LENGTH 250

int read_token (token *theToken, char* tokens){
	int i;
	signed int decimal;
			if (strcmp(tokens, "defun") == 0) {
        		theToken->type = DEFUN;
        		//printf("defun\n");
        		return 0;

    		} 
    		else if (strcmp(tokens, "return") == 0) {
        		theToken->type = RETURN;
        		//printf("return\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "+") == 0) {
        		theToken->type = PLUS;
        		//printf("+\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "-") == 0) {
        		theToken->type = MINUS;
        		//printf("-\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "*") == 0) {
        		theToken->type = MUL;
        		//printf("*\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "/") == 0) {
        		theToken->type = DIV;
        		//printf("/\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "%") == 0) {
        		theToken->type = MOD;
        		//printf("%%\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "and") == 0) {
        		theToken->type = AND;
        		//printf("and\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "or") == 0) {
        		theToken->type = OR;
        		//printf("or\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "not") == 0) {
        		theToken->type = NOT;
        		//printf("not\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "lt") == 0){
        		theToken->type = LT;
        		//printf("lt\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "le") == 0) {
        		theToken->type = LE;
        		//printf("le\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "eq") == 0) {
        		theToken->type = EQ;
        		//printf("eq\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "ge") == 0) {
        		theToken->type = GE;
        		//printf("ge\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "gt") == 0) {
        		theToken->type = GT;
        		//printf("gt\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "if") == 0) {
        		theToken->type = IF;
        		//printf("if\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "else") == 0) {
        		theToken->type = ELSE;
        		//printf("else\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "endif") == 0) {
        		theToken->type = ENDIF;
        		//printf("endif\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "drop") == 0) {
        		theToken->type = DROP;
        		//printf("drop\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "dup") == 0) {
        		theToken->type = DUP;
        		//printf("dup\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "swap") == 0) {
        		theToken->type = SWAP;
        		//printf("swap\n");
        		return 0;
    		}
    		else if (strcmp(tokens, "rot") == 0) {
        		theToken->type = ROT;
        		//printf("rot\n");
        		return 0;
    		}
    		if (strlen(tokens) > 3  && strlen(tokens) < 6) {
				if (tokens[0] == 'a' && tokens[1] == 'r' && tokens[2] == 'g') {
					char num[2];
    				for(i = 3; i < strlen(tokens); i++) {
						if(!isdigit(tokens[i])) {
							theToken->type = BROKEN_TOKEN;
							//printf("ERROR: Illegl arg\n");
							return 1;
						}
						num[i-3] = tokens[i];
					}
					int argnum = atoi(num);
					if(argnum > 20) {
						theToken->type = BROKEN_TOKEN;
						//printf("argument number out of bounds\n");
						return 1;
					}
					theToken->type = ARG;
					theToken->arg_no = argnum;
					//printf("arg%d\n", argnum);
					return 0;
				}
    		}
    		
    		if (tokens[0] == '0' && tokens[1] == 'x') {
    			for(i = 2; i < strlen(tokens); i++) {
					if(!isxdigit(tokens[i])) {
						theToken->type = BROKEN_TOKEN;
						//printf("ERROR: broken token!\n");
						return 1;
					}
				}
    			char *ptr;
    			theToken->type = LITERAL;
    			theToken->literal_value = strtol(tokens , &ptr , 16);
    			//printf("0x%d\n", theToken->literal_value);
    			return 0;
    		}
    		else if (isdigit(tokens[0]) != 0 || tokens[0] == '-') {
    			for(i = 1; i < strlen(tokens); i++) {
					if(!isdigit(tokens[i])) {
						theToken->type = BROKEN_TOKEN;
						//printf("ERROR: Illegl number\n");
						return 1;
					}
				}
				theToken->type = LITERAL;
				sscanf(tokens,"%d", &decimal);
				theToken->literal_value = decimal;
				//printf("%d\n", decimal);
				return 0;
    		}
    		else {
    			if(!isalpha(tokens[0])) {
    				theToken->type = BROKEN_TOKEN;
					//printf("ERROR: Illegal leading char\n");
					return 1;
				}

				for(i = 1; i < strlen(tokens); i++) {
					if(!(isalnum(tokens[i]) || tokens[i] == '_')) {
						theToken->type = BROKEN_TOKEN;
						//printf("ERROR: Illegal name\n");
						return 1;
					}
				}
				//IDENT
				theToken->type = IDENT;
				strcpy(theToken->str, tokens);
				//printf("IDENT(%s)\n", tokens);
				return 0;
    		}
	return 1;

}


