CFLAGS = -I /opt/homebrew/include/ -L /opt/homebrew/lib/ -l SDL3 -Wall -Wpedantic

all: cardEngine

cardEngine: cardEngine.c
	clang cardEngine.c ${CFLAGS} -o cardEngine
clean:
	rm -f cardEngine