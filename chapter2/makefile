CC=gcc
IDIR=./
CFLAGS=-I$(IDIR)
CFLAGS:=-std=gnu99
LDIR=
ODIR=obj
LIBS=-lm
DEBUG=-g

_DEPS=sort.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=test.o insert_sort.o selection_sort.o merge_sort.o merge_sort2_3-2.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) $(DEBUG) -c -o $@ $< $(CFLAGS)

test_sort:$(OBJ)
	$(CC) $(DEBUG) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY:clean

clean:
	rm -f test_sort $(ODIR)/*.o *~ core $(INCDIR)/*~
