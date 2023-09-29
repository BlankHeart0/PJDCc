objects = PJDCc.o Scanner.o Parser.o AbstractSyntaxTree.o \
		  Register.o CodeGenerator.o SymbolTable.o Iteration.o


pjdcc: $(objects)
	g++ -o pjdcc $(objects)



PJDCc.o: Token.h Scanner.h Parser.h CodeGenerator.h


Scanner.o: Token.h Scanner.h

Parser.o: Token.h  AbstractSyntaxTree.h Parser.h
AbstractSyntaxTree.o: AbstractSyntaxTree.h

CodeGenerator.o: AbstractSyntaxTree.h Register.h Iteration.h SymbolTable.h CodeGenerator.h  
Register.o: Register.h
SymbolTable.o: SymbolTable.h
Iteration.o: Iteration.h




.PHONY: clean
clean:
	-rm pjdcc $(objects)
	-rm ./test_create/*