
/*
 * writer.c
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ctype.h"
#include "token.h"
#include "writer.h"

int isDefun = 0;
int bran_label_counter = 0;
int if_label_counter = 0;
int current_stack = 0;
int else_flag = 0;
	
int write_asm (token *theToken, FILE *theFile, stack *theStack){
	char* get_top_two = "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n\tLDR R2 R6 #0\n\tADD R6 R6 #1\n";
	
	char* prologue = "\t;; prologue ;;\n\tADD R6 R6 #-3\n\tSTR R7 R6 #1\n\tSTR R5 R6 #0\n\tADD R5 R6 #0\n";
	
	char* epilogue = "\t;; epilogue ;;\n\tLDR R7 R6 #0\n\tSTR R7 R5 #2\n\tADD R6 R5 #0\n\tLDR R5 R6 #0\n\tLDR R7 R6 #1\n\tADD R6 R6 #3\n\tRET\n";
	
	char* storeResult = "\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n";

	char* bran = "\tSTR R0 R6 #-1\n\tADD R6 R6 #-1\n";

	char* cmp = "\tCMP R1 R2\n";

	int arg_offset = 2;
	
	switch(theToken->type) {
		case DEFUN:
			isDefun = 1;
			break;
		
		//keep track of DEFUN for IDENT
		case IDENT:
			if(isDefun == 1) {
				fprintf (theFile, "\t\t .CODE\n\t\t .FALIGN\n");
				fprintf (theFile, "%s\n", theToken->str);
				fprintf (theFile, "%s", prologue);
				fprintf (theFile, "\t;; function body\n");
				isDefun = 0;
			} else {
				fprintf (theFile, "\tJSR %s\n\tADD R6 R6 #-1\n", theToken->str);
			}
			break;
			
		case RETURN:
			fprintf (theFile, "%s", epilogue);
			break;
		
		case PLUS:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "\tADD R1 R1 R2\n");
			fprintf (theFile, "%s", storeResult);
			break;
		
		case MINUS:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "\tSUB R1 R1 R2\n");
			fprintf (theFile, "%s", storeResult);
			break;
		
		case MUL:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "\tMUL R1 R1 R2\n");
			fprintf (theFile, "%s", storeResult);
			break;
		
		case DIV:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "\tDIV R1 R1 R2\n");
			fprintf (theFile, "%s", storeResult);
			break;
		
		case MOD:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "\tMOD R1 R1 R2\n");
			fprintf (theFile, "%s", storeResult);
			break;
		
		case AND:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "\tAND R1 R1 R2\n");
			fprintf (theFile, "%s", storeResult);
			break;
		
		case OR:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "\tOR R1 R1 R2\n");
			fprintf (theFile, "%s", storeResult);
			break;
		
		case NOT:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "\tNOT R1 R1 R2\n");
			fprintf (theFile, "%s", storeResult);
			break;
		
		case LT:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "%s", cmp);
			fprintf (theFile, "\tBRn ");
			bran_label_counter++;
			fprintf (theFile, "LT_%d\n", bran_label_counter);
			fprintf (theFile, "\tCONST R0 #0\n");
			fprintf (theFile, "%s", bran);
			fprintf (theFile, "\tJMP END_LT_%d\n", bran_label_counter);
			fprintf (theFile, "LT_%d\n", bran_label_counter);
			fprintf (theFile, "\tCONST R0 #1\n");
			fprintf (theFile, "%s", bran);
			fprintf (theFile, "END_LT_%d\n", bran_label_counter);
			break;
		
		case LE:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "%s", cmp);
			fprintf (theFile, "\tBRnz ");
			bran_label_counter++;
			fprintf (theFile, "LE_%d\n", bran_label_counter);
			fprintf (theFile, "\tCONST R0 #0\n");
			fprintf (theFile, "%s", bran);
			fprintf (theFile, "\tJMP END_LE_%d\n", bran_label_counter);
			fprintf (theFile, "LE_%d\n", bran_label_counter);
			fprintf (theFile, "\tCONST R0 #1\n");
			fprintf (theFile, "%s", bran);
			fprintf (theFile, "END_LE_%d\n", bran_label_counter);
			break;
		
		case EQ:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "%s", cmp);
			fprintf (theFile, "\tBRz ");
			bran_label_counter++;
			fprintf (theFile, "EQ_%d\n", bran_label_counter);
			fprintf (theFile, "\tCONST R0 #0\n");
			fprintf (theFile, "%s", bran);
			fprintf (theFile, "\tJMP END_EQ_%d\n", bran_label_counter);
			fprintf (theFile, "EQ_%d\n", bran_label_counter);
			fprintf (theFile, "\tCONST R0 #1\n");
			fprintf (theFile, "%s", bran);
			fprintf (theFile, "END_EQ_%d\n", bran_label_counter);
			break;
		
		case GE:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "%s", cmp);
			fprintf (theFile, "\tBRzp ");
			bran_label_counter++;
			fprintf (theFile, "GE_%d\n", bran_label_counter);
			fprintf (theFile, "\tCONST R0 #0\n");
			fprintf (theFile, "%s", bran);
			fprintf (theFile, "\tJMP END_GE_%d\n", bran_label_counter);
			fprintf (theFile, "GE_%d\n", bran_label_counter);
			fprintf (theFile, "\tCONST R0 #1\n");
			fprintf (theFile, "%s", bran);
			fprintf (theFile, "END_GE_%d\n", bran_label_counter);
			break;
		
		case GT:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "%s", cmp);
			fprintf (theFile, "\tBRp ");
			bran_label_counter++;
			fprintf (theFile, "GT_%d\n", bran_label_counter);
			fprintf (theFile, "\tCONST R0 #0\n");
			fprintf (theFile, "%s", bran);
			fprintf (theFile, "\tJMP END_GT_%d\n", bran_label_counter);
			fprintf (theFile, "GT_%d\n", bran_label_counter);
			fprintf (theFile, "\tCONST R0 #1\n");
			fprintf (theFile, "%s", bran);
			fprintf (theFile, "END_GT_%d\n", bran_label_counter);
			break;
		
		case IF:
			if_label_counter ++;
			push(theStack, if_label_counter, 1);
			fprintf(theFile, "\tLDR R1 R6 #0\n\tADD R6 R6 #1\n");
			fprintf(theFile, "\tCMPI R1 #0\n");
			fprintf(theFile, "\tBRz ELSE_%d\n", if_label_counter);
			break;
		
		case ELSE:
			current_stack = theStack->first->n;
			else_flag = 1;
			fprintf(theFile, "\tJMP ENDIF_%d\n", current_stack);
			fprintf(theFile, "ELSE_%d\n", current_stack);
			break;
		
		case ENDIF:
			current_stack = theStack->first->n;
			pop(theStack);
			//keep track of if we have seen an ELSE
			if (else_flag == 1) {
				fprintf(theFile, "ENDIF_%d\n", current_stack);
			}
			else {
				fprintf(theFile, "ELSE_%d\n", current_stack);
				fprintf(theFile, "ENDIF_%d\n", current_stack);
			}
			break;
		
		case DROP:
			fprintf (theFile, "\tADD R6 R6 #1\n");
			break;
		
		case DUP:
			fprintf (theFile, "\tLDR R1 R6 #0\n\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n");
			break;
		
		case SWAP:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n\tSTR R2 R6 #-1\n\tADD R6 R6 #-1\n");
			break;
		
		case ROT:
			fprintf (theFile, "%s", get_top_two);
			fprintf (theFile, "\tLDR R3 R6 #0\n\tADD R6 R6 #1\n");
			fprintf (theFile, "\tSTR R2 R6 #-1\n\tADD R6 R6 #-1\n\tSTR R1 R6 #-1\n\tADD R6 R6 #-1\n\tSTR R3 R6 #-1\n\tADD R6 R6 #-1\n");
			break;
		
		case ARG:
			fprintf (theFile, "\tLDR R3 R5 #%d\n", (theToken->arg_no) + arg_offset);
			fprintf (theFile, "\tSTR R3 R6 #-1\n\tADD R6 R6 #-1\n");
			break;
		
		case LITERAL:
			if(((theToken->literal_value) >= -256) && ((theToken->literal_value) <= 256)) {	
				//only CONST
				fprintf (theFile, "\tCONST R3 #%d\n", theToken->literal_value);
				fprintf (theFile, "\tSTR R3 R6 #-1\n\tADD R6 R6 #-1\n");
			} else {
				//CONST + HICONST
				int lower_shifted = (theToken->literal_value);
				int upper_shifted = ((theToken->literal_value) >> 8);
				int lower = lower_shifted & 0xFF;
				int upper = upper_shifted & 0xFF;
				fprintf (theFile, "\tCONST R3 #%d\n", lower);
				fprintf (theFile, "\tHICONST R3 #%d\n", upper);
				fprintf (theFile, "\tSTR R3 R6 #-1\n\tADD R6 R6 #-1\n");
			}
			break;
		
		case BROKEN_TOKEN:
			//print out error message and keep going
			fprintf (theFile, "\tError: broken token\n");
			return 1;
	}
	
	return 0;
}
