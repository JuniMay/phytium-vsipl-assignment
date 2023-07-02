
TARGET = radar
CC = cc
PY = python3
INC_DIR = -I./src -I./vsipl/include 
LIB_DIR = -L./vsipl/lib
LIBS = -lvsip -lfftw3f -lm
CFLAGS = $(INC_DIR) $(LIB_DIR) $(LIBS) -g

SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean all run

all: $(TARGET)

run: $(TARGET)
	mkdir -p data
	./$(TARGET)
	$(PY) ./plot.py

clean:
	rm -f $(TARGET) $(OBJ)
	rm -rf data