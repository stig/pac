all: pac
.PHONY: all clean

SRC := $(wildcard *.c) $(wildcard *.h)
OBJ := $(patsubst %.c,%.o,$(filter %.c, $(SRC)))
LIBS := -lncurses

%.o: %.c
	cc -c -o $@ $<

pac: $(OBJ)
	cc -o $@ $(LIBS) $(OBJ)

clean:
	-rm $(OBJ) pac
