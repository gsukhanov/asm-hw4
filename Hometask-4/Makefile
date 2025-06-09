all: build/main

build/main: build/functions.o build
	@read -p "Choose equation solution method [split/chord/tangent/mix]: " METHOD; \
	if [ "$$METHOD" = "split" ]; then \
		gcc -no-pie -m32 src/main.c build/functions.o -o build/main -DSPLIT; \
	elif [ "$$METHOD" = "chord" ]; then \
		gcc -no-pie -m32 src/main.c build/functions.o -o build/main -DCHORD; \
	elif [ "$$METHOD" = "tangent" ]; then \
		gcc -no-pie -m32 src/main.c build/functions.o -o build/main -DTANGENT; \
	elif [ "$$METHOD" = "mix" ]; then \
		gcc -no-pie -m32 src/main.c build/functions.o -o build/main -DMIX; \
	else \
		echo -e "Error! Unknown method!\n"; exit 1; \
	fi

build/functions.o: build
	nasm -f elf32 src/functions.asm -o build/functions.o

hard: build/hard_main

build/hard_main: build/construct build/extra_functions.o build/interval.o build
	@read -p "Choose equation solution method [split/chord]: " METHOD; \
	if [ "$$METHOD" = "split" ]; then \
		gcc -no-pie -m32 src/hard_main.c build/interval.o build/extra_functions.o -o build/hard_main -DSPLIT; \
	elif [ "$$METHOD" = "chord" ]; then \
		gcc -no-pie -m32 src/hard_main.c build/interval.o build/extra_functions.o -o build/hard_main -DCHORD; \
	else \
		echo -e "Error! Unknown method!\n"; exit 1; \
	fi
	
build/extra_functions.o: build/extra_functions.asm build
	nasm -f elf32 build/extra_functions.asm -o build/extra_functions.o

build/extra_functions.asm: build
	echo "Error! Functions were not created!"
	false

build/interval.o: build/interval.c build
	gcc -no-pie -m32 -c build/interval.c -o build/interval.o

build/interval.c: build
	echo "Error! Intervals were not created!"
	false

build/construct: build
	gcc src/construct.c -o build/construct
	./build/construct

build:
	mkdir -p build

.PHONY: clean
clean:
	rm -rf build