CC=gcc
CFLAGS=-pthread -lrt
DEPS =
EXEC = servidor
OBJ = servidor.o

all: $(EXEC)
	@echo "Iniciando compilacao"
	@echo "Compilacao finalizada!"
%.o: %.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o
	@echo "Limpeza completa"
