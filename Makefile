build:
	gcc -g main.c image.c utils.c commands.c functions.c -Wall -Wextra -std=c99 -pedantic -o image_editor -lm
pack:
	zip -FSr 312CA_AriciuToma_Tema3.zip README Makefile *.c *.h
clean:
	rm image_editor
run:
	./image_editor