build:
	gcc battleship.c -o battleship -lncurses

run:
	./battleship harta.txt

clean:
	rm battleship

