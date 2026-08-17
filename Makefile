CC = gcc
CFLAGS = -Wall -g -fdiagnostics-color=always -I include
SOURCES = src/main.c src/cube.c src/io_utils.c

# 1. Detecta o Sistema Operacional
ifeq ($(OS),Windows_NT)
    TARGET = bin/rubiks_cube.exe
    CLEAN_CMD = del /f bin\*.exe
    MKDIR_BIN = if not exist bin mkdir bin
else
    TARGET = bin/rubiks_cube
    CLEAN_CMD = rm -f bin/*
    MKDIR_BIN = mkdir -p bin
endif

all: setup $(TARGET)

setup:
	@$(MKDIR_BIN)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	@$(CLEAN_CMD)