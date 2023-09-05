objects = PJDCc.o Scanner.o Parser.o AbstractSyntaxTree.o Interpreter.o \
		  Register.o CodeGenerator.o SymbolTable.o


pjdcc: $(objects)
	g++ -o pjdcc $(objects)


PJDCc.o: Token.h Scanner.h Parser.h CodeGenerator.h
Scanner.o: Token.h Scanner.h
Parser.o: Token.h Parser.h AbstractSyntaxTree.h Interpreter.h
AbstractSyntaxTree.o: AbstractSyntaxTree.h
Interpreter.o: AbstractSyntaxTree.h Interpreter.h
Register.o: Register.h
CodeGenerator.o: Register.h CodeGenerator.h AbstractSyntaxTree.h Token.h SymbolTable.h
SymbolTable.o: SymbolTable.h

.PHONY: clean
clean:
	-rm pjdcc $(objects)