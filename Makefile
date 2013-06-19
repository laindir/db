CFLAGS=-Wall -Wextra -pedantic
LDLIBS=-ldb
RONNFLAGS=--manual='User Commands' --organization=laindir

all: db

%: %.ronn
	ronn --roff $(RONNFLAGS) $<

%.html: %.ronn
	ronn --html $(RONNFLAGS) $<

.PHONY: all
