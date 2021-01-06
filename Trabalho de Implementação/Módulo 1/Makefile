FLAG_DEBUG = DEBUG
FLAG_TESTE = --teste

all: FLAGS = $(FLAG_DEBUG)
all: build
all: run

debug: FLAGS = $(FLAG_DEBUG)
debug: build
debug: run

teste: FLAGS = $(FLAG_TESTE)
teste: build
teste: run_teste


build:
	gcc -o main main.c merge_sort_concorrente.c teste_merge_sort_concorrente.c -Wall -pthread

run:
	./main 10 4 $(FLAGS)

run_teste:
	./main $(FLAGS)
