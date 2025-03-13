CC = clang
TARGET = emul2560
CCFLAGS = $()

PACT_TEMP_DIR = /var/tmp/pact
PACT_DIR = /var/pact

$(TARGET): $(wildcard *.c) $(wildcard */*.c)
	$(CC) $(CCFLAGS) -o $@ $^