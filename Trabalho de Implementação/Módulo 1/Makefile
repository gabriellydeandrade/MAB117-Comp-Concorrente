FLAG_DEBUG = DEBUG
FLAG_TESTE = --teste

all: FLAGS = $(FLAG_DEBUG)
all: build

debug: FLAGS = $(FLAG_DEBUG)
debug: build

teste: FLAGS = $(FLAG_TESTE)
teste: build


build:
	gcc -o main main.c merge_sort_concorrente.c teste_merge_sort_concorrente.c -Wall -pthread
	./main $(FLAGS)

