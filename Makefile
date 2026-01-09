src := $(wildcard src/*.c)
output := bin/todo

all: debug clean run

bin:
	@mkdir -p bin

# ------------------------------------------------------------------
compileflags := -std=c2x -Iinclude ${src} -o ${output}

debugflags := -O0 -Wall -Wshadow -Werror
debug: bin
	@${CC} ${compileflags} ${debugflags}

releaseflags = -O2
release: bin
	@$(CC) ${compileflags} ${releaseflags}
#-------------------------------------------------------------------

clean:
	@rm -rf bin

run:
	./${output}
