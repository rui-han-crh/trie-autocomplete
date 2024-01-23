CC=g++

CFLAGS=-std=c++17 -lstdc++ -c -Wall

all: autocomplete

autocomplete: main.o trie.o node.o
	$(CC) main.o trie.o node.o -o autocomplete

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

trie.o: trie.cpp
	$(CC) $(CFLAGS) trie.cpp

node.o: node.cpp
	$(CC) $(CFLAGS) node.cpp

clean:
	rm -rf *o autocomplete