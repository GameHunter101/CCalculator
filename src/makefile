IDIR = ../include
CC = clang
CFLAGS = -I$(IDIR) -Wall -Wextra -g

ODIR = ./obj
LDIR = ../lib

LIBS=

_DEPS = parser.h stack.h individuals.h ast.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o parser.o stack.o ast.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

calculator: $(OBJ)
	$(CC) -o ../bin/$@.exe $^ $(CFLAGS) $(LIBS)
