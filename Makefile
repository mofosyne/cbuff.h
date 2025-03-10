CC      ?= cc
CP      ?= cp -f
RM      ?= rm -f
MKDIR   ?= mkdir -p
INSTALL ?= install

PREFIX  ?= /usr/local

#CFLAGS += -Wall -std=c99 -pedantic
CFLAGS += -Wall -std=c11 -pedantic

.PHONY:
all: test

.PHONY: test
test: cbuff_uint8_test

.PHONY: readme_update
readme_update:
	# Library Version (From clib package metadata)
	jq -r '.version' clib.json | xargs -I{} sed -i 's|<version>.*</version>|<version>{}</version>|' README.md
	jq -r '.version' clib.json | xargs -I{} sed -i 's|<versionBadge>.*</versionBadge>|<versionBadge>![Version {}](https://img.shields.io/badge/version-{}-blue.svg)</versionBadge>|' README.md

.PHONY: format
format:
	# pip install clang-format
	clang-format -i *.c
	clang-format -i *.h

.PHONY:
cbuff_uint8_test: cbuff_uint8_test.c cbuff_uint8.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o cbuff_uint8_test cbuff_uint8_test.c
	./cbuff_uint8_test

.PHONY:
%.o: %.c
	$(CC) $(DEP_FLAG) $(CFLAGS) $(LDFLAGS) -o $@ -c $<

.PHONY:
clean:
	rm -f cbuff_uint8_test
