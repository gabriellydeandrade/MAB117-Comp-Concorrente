all: run

run: build
	@./aproximacao_pi 1000000000 4

build:
	gcc -o aproximacao_pi  aproximacao_pi.c  -Wall -pthread -lm