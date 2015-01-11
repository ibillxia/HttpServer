CC = g++
CFLAGS = -g -O2
CFLAGS_EXTRA = -lpthread
BIN_FILE = xhttp
OBJ_FILES = HttpServer.o
SRC_FILES = HttpServer.cpp

all: $(BIN_FILE)
$(BIN_FILE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(BIN_FILE) $(CFLAGS_EXTRA)
$(OBJ_FILES):
	$(CC) $(CFLAGS) -c $(SRC_FILES)

clean:
	rm $(OBJ_FILES) $(BIN_FILE)
