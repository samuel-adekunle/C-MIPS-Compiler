  
CPPFLAGS += -std=c++17 -W -Wall -g -ggdb3 -Wno-unused-parameter
CPPFLAGS += -I inc

bin/c_compiler : bin/compiler scripts/wrapper.sh
	cp scripts/wrapper.sh bin/c_compiler
	chmod u+x bin/c_compiler

bin/compiler : src/compiler.cpp src/code_gen/code_gen.cpp src/code_gen/types.cpp src/code_gen/scope.cpp src/lang/AST.cpp src/lang/y.tab.c src/lang/lex.yy.c src/code_gen/utilities.cpp 
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/compiler $^

bin/parseTree: src/parseTree.cpp src/code_gen/types.cpp src/lang/AST.cpp src/lang/y.tab.c src/lang/lex.yy.c src/code_gen/utilities.cpp 
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/parseTree $^

bin/grammar_tree_maker: src/utils/GrammarTreeMaker/grammar_tree_maker.go
	mkdir -p bin
	cd src/utils/GrammarTreeMaker && \
	go build -o ../../../bin/grammar_tree_maker

src/lang/lex.yy.c: src/lang/y.tab.c
	lex -o src/lang/lex.yy.c src/lang/ANSII_C.lex

src/lang/y.tab.c:
	yacc -b src/lang/y src/lang/ANSII_C.y -d -Wno-yacc

clean:
	rm -f src/*.o
	rm -f bin/*
	rm -f src/lang/y.tab.c
	rm -f src/lang/lex.yy.c
	rm -f src/lang/y.tab.h
	find ./out -type f -not -name 'grammar_tree.png' -delete
