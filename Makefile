practice: main.o dictionary.o
	gcc -Wall -Werror -o practice main.o dictionary.o

%.o: %.c
	gcc -c -Wall -Werror -o $@ $<

clean:
	rm -f *.o practice