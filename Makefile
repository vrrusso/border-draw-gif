all:
	gcc -o t *.c -Wall
clear: 
	rm *.o
run:t
	./t <$(IN)

