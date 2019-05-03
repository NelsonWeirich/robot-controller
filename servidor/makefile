CC=gcc

LIB=./lib
INLCUDE=./include
SRC=./src
OBJ=./obj

LIBFLAGS= -llperiodic
CFLAGS= -pthread -lrt
EXEC = servidor
DEPS = lperiodic

all: $(EXEC)
	@echo "Iniciando compilacao"
	@echo "Compilacao finalizada!"

$(EXEC): $(DEPS)
	$(CC) -o $@ $(SRC)/servidor.c -I $(INCLUDE)/lperiodic.h -L $(LIB)/lperiodic.a $(CFLAGS)

$(DEPS):
	$(CC) -c $(SRC)/lperiodic.c $(CFLAGS) -I $(INCLUDE)/lperiodic.h -o $(OBJ)/lperiodic.o
	ar rv $(LIB)/liblperiodic.a $(OBJ)/lperiodic.o

clean:
	rm -rf *.* $(EXEC) $(OBJ)/*.o $(LIB)/*.a
	@echo "Limpeza completa"
