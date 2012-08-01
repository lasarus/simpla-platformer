CC := gcc
CCFLAGS := -lSDL -lm

SRCDIRS := src
SOURCES := $(foreach DIR, $(SRCDIRS), $(wildcard $(DIR)/*.c))
TARGET := bin/simpla-p

.PHONY: all
.PHONY: clean

all: $(TARGET)

clean:
	rm -f $(TARGET)

$(TARGET): $(SOURCES)
	mkdir -p $(@D)
	$(CC) -o $(TARGET) $(SOURCES) $(CCFLAGS)
