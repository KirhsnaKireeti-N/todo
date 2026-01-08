files := main.c data.c
output := bin/todo

all: compile clean run

compile: libcsv
	@mkdir -p bin
	@$(CC) -std=c2x -Iinclude ${files} -L. -lcsv -o ${output}

libcsv:
	@$(CC) -c libcsv.c -o libcsv.o -Iinclude
	@$(AR) rcs libcsv.a libcsv.o

clean:
	@rm libcsv.o libcsv.a

run:
	./${output}
