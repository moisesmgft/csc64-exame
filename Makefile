CC = gcc
CFLAGS = -O3 -std=c99 -fopenmp

# Pasta build
BUILD_DIR = build/
DB_DIR = output/db/

# Executáveis
EXEC_PARALLEL = $(BUILD_DIR)parallel.exe
EXEC_DB = $(BUILD_DIR)db.exe

# Objetos
OBJ_PARALLEL = $(BUILD_DIR)parallel.o
OBJ_DB = $(BUILD_DIR)db.o

# Criação da pasta build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(DB_DIR):
	mkdir -p $(DB_DIR)

# Compilação principal
all: $(BUILD_DIR) $(DB_DIR) $(EXEC_PARALLEL) $(EXEC_DB) $(DB_DIR)

# Criação do executável parallel.exe
$(EXEC_PARALLEL): $(OBJ_PARALLEL)
	$(CC) $(CFLAGS) -o $@ $^

# Criação do executável db.exe
$(EXEC_DB): $(OBJ_DB)
	$(CC) $(CFLAGS) -o $@ $^

# Compilação dos objetos
$(OBJ_PARALLEL): src/parallel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DB): src/db.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Limpeza
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DB_DIR)

# Executar apenas o db
db: $(EXEC_DB)
	./$(EXEC_DB)
