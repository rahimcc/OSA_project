
SRC=detecter.c DetecLib.c
detecter: $(SRC)
	gcc	$(SRC) -fprofile-arcs -ftest-coverage -o detecter

gcov: $(SRC)
	gcov $(SRC)

clean: 	
	rm -f *.log *.tmp
	rm -f *.gc*
	rm detecter