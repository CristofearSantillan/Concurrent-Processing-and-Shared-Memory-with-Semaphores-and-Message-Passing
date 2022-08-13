all: swim_mill fish pellet

swim_mill: swim_mill.c
	gcc -o swim_mill swim_mill.c

fish: fish.c
	gcc -o fish fish.c

pellet: pellet.c
	gcc -o pellet pellet.c

clean:
	rm swim_mill fish pellet