test_file: test_file.o file.o
	gcc test_file.o file.o -o test_file	

test_file.o: test_file.c file.h
	gcc -c test_file.c

file.o: file.c file.h
	gcc -c file.c

serveur: TP-DIST.o file.o split.o
	gcc TP-DIST.o file.o split.o -o serveur

client: TP-DIST-CL.o
	gcc TP-DIST-CL.o -o client

TP-DIST-CL.o: TP-DIST-CL.c
	gcc -c TP-DIST-CL.c

TP-DIST.o: TP-DIST.c file.h split.h
	gcc -c TP-DIST.c

split.o: split.c split.h
	gcc -c split.c

clean:
	rm -f *.o Test*

