#makefile for Lexer
#gcc219
#George Cooper
#CSE 109

OBJ = token.o lexer.o parser.o microc.o symbolTable.o stack.o
OPTS = -g -c -Wall -Werror

microc: $(OBJ)
	g++ -o microc $(OBJ)

parser.o: parser.cc parser.h lexer.o token.o
	g++ $(OPTS) parser.cc

microc.o: microc.cc token.o lexer.o parser.o
	g++ $(OPTS) microc.cc

token.o: token.cc token.h
	g++ $(OPTS) token.cc

lexer.o: lexer.cc lexer.h
	g++ $(OPTS) lexer.cc

lextest.o: lextest.cc
	g++ $(OPTS) lextest.cc

symbolTable.o: symbolTable.cc symbolTable.h stack.o
	g++ $(OPTS) symbolTable.cc

stack.o: stack.cc stack.h
	g++ $(OPTS) stack.cc

clean:
	rm -f *.o *~