all: clean
	@rm -f ../run/*.txt
	@gcc geocnet.c -I/usr/include/igraph -c
	@gcc geocnetApp.c geocnet.o -I/usr/include/igraph /usr/lib/x86_64-linux-gnu/libigraph.so -o ../run/geocnetApp.x

dbg:
	@gcc -g geocnet.c -I/usr/include/igraph -c
	@gcc -g geocnetApp.c geocnet.o -I/usr/include/igraph /usr/lib/x86_64-linux-gnu/libigraph.so -o ../run/geocnetApp.x
clean:
	@rm -f *.o *.x
