jc: compiler.c stack.c stack.h writer.c writer.h token.c token.h
	clang -Wall -g -o jc compiler.c stack.c writer.c token.c
	
clean: 
	rm -f jc *.o
