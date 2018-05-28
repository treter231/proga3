all: bin
	gcc -Wall ./src/main.c ./src/command.c ./src/coder.c -o ./bin/coder -lm
bin:
	mkdir bin

.PHONY : encode, decode, hexdump

encode:
	bin/coder encode bin/points.txt bin/units.bin

decode:
	bin/coder decode bin/units.bin bin/points.txt

hexdump:
	hexdump -C bin/units.bin
