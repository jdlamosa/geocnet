all: clean
	@rm -f ../run/*.txt
	@gcc geocnet.c -I/opt/igraphusr/include/igraph -L/opt/igraphusr/lib -ligraph -c
	@gcc geocnetApp.c geocnet.o -I/opt/igraphusr/include/igraph -L/opt/igraphusr/lib -ligraph -o ../run/geocnetApp.x

dbg:
	@gcc -g geocnet.c -I/opt/igraphusr/include/igraph -L/opt/igraphusr/lib -ligraph -c
	@gcc -g geocnetApp.c geocnet.o -I/opt/igraphusr/include/igraph -L/opt/igraphusr/lib -ligraph -o ../run/geocnetApp.x
clean:
	@rm -f *.o *.x