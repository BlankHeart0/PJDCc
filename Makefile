objects = PJDCc.o Scanner.o Parser.o AbstractSyntaxTree.o \
		  Register.o CodeGenerator.o SymbolTable.o


pjdcc: $(objects)
	g++ -o pjdcc $(objects)



PJDCc.o: Token.h Scanner.h Parser.h CodeGenerator.h


Scanner.o: Token.h Scanner.h

Parser.o: Token.h  AbstractSyntaxTree.h Parser.h
AbstractSyntaxTree.o: AbstractSyntaxTree.h

CodeGenerator.o: AbstractSyntaxTree.h Register.h SymbolTable.h CodeGenerator.h  
Register.o: Register.h
SymbolTable.o: SymbolTable.h





.PHONY: clean
clean:
	-rm pjdcc $(objects)