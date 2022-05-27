Template: program.asm IO.inc
	nasm -f macho64 -Ox program.asm
	ld -static program.o -o program
	rm program.o
