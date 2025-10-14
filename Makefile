SPEC_FILE ?= input.txt
METHOD ?= split
HARD ?= no

CC := gcc
CFLAGS := -no-pie -m32
ASMC := nasm
ASMFLAGS := -f elf32

OBJECTS := build/functions.o


ifeq ($(METHOD),split)
DEFINE = -DSPLIT
else ifeq ($(METHOD),chord)
DEFINE = -DCHORD
else
$(error Error! Unknown method: $(METHOD). Available methods are "split" and "chord")
endif

ifeq ($(HARD),yes)
DEFINE  += -DHARD_BUILD
OBJECTS += build/interval.o
endif

.PHONY: test change_var clean

all: integral

integral: build/functions.o build
ifeq ($(METHOD),split)
	@echo "Split method was chosen."
else
	@echo "Chord method was chosen."
endif
	@$(CC) $(CFLAGS) src/main.c $(OBJECTS) -o integral $(DEFINE)
	@echo "Main built. Run with ./integral. Command line options list: ./integral -h"

build/functions.o: build
ifeq ($(HARD), yes)
	@$(ASMC) $(ASMFLAGS) build/extra_functions.asm -o build/functions.o;
else
	@$(ASMC) $(ASMFLAGS) src/functions.asm -o build/functions.o;
endif
	@echo "Assembly functions built."

test: build/test
	@./build/test

build/test: build/test_functions.o build
	@$(CC) $(CFLAGS) src/main.c build/test_functions.o -o build/test -DTEST_LAUNCH
	@echo "Test executable built."

build/test_functions.o: src/test_functions.asm build
	@$(ASMC) $(ASMFLAGS) src/test_functions.asm -o build/test_functions.o
	@echo "Test assembly functions built."

hard: build/construct build/extra_functions.asm build/interval.o
	@HARD=yes make --no-print-directory all

build/extra_functions.asm: build
	@echo "Error! Functions were not created!"
	@false

build/interval.o: build/interval.c build
	@$(CC) $(CFLAGS) -c build/interval.c -o build/interval.o
	@echo "Interval information passed."

build/interval.c: build
	@echo "Error! Intervals were not created!"
	@false

build/construct: build
	@$(CC) src/construct.c -o build/construct
	@./build/construct
	@echo "Constructor sub-process was built and running."

build:
	@mkdir -p build
	@echo "Build directory created."

.PHONY: clean
clean:
	@rm -rf build
	@rm integral
	@echo "Build directory deleted. Executable file deleted."