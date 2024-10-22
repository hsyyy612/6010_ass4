# Rename this file `Makefile` and include it in your submission if you use it.

# Replace `parking.c` with the name of your file, or name your file `parking.c`.
#           vvvvvvvvvvvv
parking: parking.c
	gcc -o $@ $^

.PHONY: clean
clean:
	rm -f parking
