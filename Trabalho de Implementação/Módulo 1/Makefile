FLAG_DEBUG = DEBUG
FLAG_TESTE = --teste

all: FLAGS = $(FLAG_DEBUG)
all: build

debug: FLAGS = $(FLAG_DEBUG)
debug: build

teste: FLAGS = $(FLAG_TESTE)
teste: build


build:
	gcc -o main  main.c -Wall -pthread
	./main $(FLAGS)

