CC = gcc
WIND = windres
CFLAGS = -Wall -g -fdiagnostics-color=always -I include
SRCS = src/main.c src/cube.c src/io_utils.c
RC_SRCS = assets/resources.rc
RC_OBJ = bin/resources.o

# 1. Detecta o Sistema Operacional
ifeq ($(OS),Windows_NT)
    TARGET = bin/rubiks_cube.exe
    CLEAN_CMD = del /f bin\*.exe
    CLEAN_OBJ = del /f bin\*.o
    MKDIR_BIN = if not exist bin mkdir bin
else
    TARGET = bin/rubiks_cube
    CLEAN_CMD = rm -f bin/*
    CLEAN_OBJ = rm -f bin/*.o
    MKDIR_BIN = mkdir -p bin
endif

all: setup $(TARGET)

setup:
	@$(MKDIR_BIN)

$(TARGET): $(SRCS) $(RC_OBJ)
	$(CC) $(CFLAGS) $(SRCS) $(RC_OBJ) -o $(TARGET)
ifeq ($(OS),Windows_NT)
	@if exist bin\resources.o del /f bin\resources.o
else
	@rm -f bin/resources.o
endif

$(RC_OBJ): $(RC_SRCS)
	$(WIND) $(RC_SRCS) -o $(RC_OBJ)

clean:
	@$(CLEAN_CMD)