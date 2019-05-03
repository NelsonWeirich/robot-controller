CC=gcc

LIB=./lib
INLCUDE=./include
SRC=./src
OBJ=./obj

LIBFLAGS=
CFLAGS= -pthread -Wall
DEPS =
EXEC = cliente

all: $(EXEC)
	@echo "Iniciando compilacao"
	@echo "Compilacao finalizada!"

$(EXEC): $(DEPS)
	$(CC) -o $@ $(SRC)/cliente.c -I$(INCLUDE) -L$(LIB) $(CFLAGS)

$(DEPS):
	$(CC) -c $(SRC) $(CFLAGS) -I $(INCLUDE) -o $(OBJ)
	tar -cru $(LIB) $(OBJ)

clean:
	rm -rf *.* $(EXEC) $(OBJ)/*.o $(LIB)/*.a
	@echo "Limpeza completa"
