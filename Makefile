# Makefile — build do player + testes em Linux/macOS (gcc/clang).
# Uso: make          compila player e testes
#      make test     roda todos os testes
#      make clean    remove binários e artefatos

CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
CFLAGS  += -g

SRC     := src/main.c src/playlist.c src/io.c
HDR     := src/playlist.h src/io.h
UNIT    := test_add test_nav test_search
ALL_BIN := player $(UNIT) test_fronteira

all: player $(UNIT) test_fronteira

player: $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(SRC) -o $@

test_add: tests/test_playlist_add.c src/playlist.h
	$(CC) $(CFLAGS) -I src $< src/playlist.c -o $@

test_nav: tests/test_playlist_navigation.c src/playlist.h
	$(CC) $(CFLAGS) -I src $< src/playlist.c -o $@

test_search: tests/test_playlist_search_sort.c src/playlist.h
	$(CC) $(CFLAGS) -I src $< src/playlist.c -o $@

test_fronteira: tests/test_fronteira_texto.c
	$(CC) $(CFLAGS) -I src $< -o $@

test: player $(UNIT) test_fronteira
	@for t in $(UNIT); do ./$$t || exit 1; done
	./test_fronteira ./player

clean:
	rm -f $(ALL_BIN) tmp_*.in tmp_*.in.out

.PHONY: all test clean
