objects = PJDCc.o Scanner.o Parser.o AbstractSyntaxTree.o Interpreter.o


pjdcc: $(objects)
	g++ -o pjdcc $(objects)


PJDCc.o: Token.h Scanner.h Parser.h
Scanner.o: Token.h Scanner.h
Parser.o: Token.h Parser.h AbstractSyntaxTree.h Interpreter.h
AbstractSyntaxTree.o: AbstractSyntaxTree.h
Interpreter.o: AbstractSyntaxTree.h Interpreter.h


.PHONY: clean
clean:
	-rm pjdcc $(objects)