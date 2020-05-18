CC                = clang
CFLAGS            = -Wall -g
LDFLAGS           = -shared
VPATH             = src
LIBS              = -lportmidi -llua

MAIN_BUILD_DIR    = build
HEADER_DIRS       = include

INCLUDES          = $(addprefix -I, $(HEADER_DIRS))

CORE_SOURCES      = $(notdir $(wildcard src/*.c))
MAIN_SOURCES      = $(CORE_SOURCES)

MAIN_OBJECTS      = $(addprefix $(MAIN_BUILD_DIR)/, $(MAIN_SOURCES:.c=.o))

OUTPUTLIB         = portmidi.so

.PHONY: clean

all: $(OUTPUTLIB)

$(MAIN_BUILD_DIR)/%.o: %.c
	$(CC) $(LIBS) $(LDFLAGS) $(CFLAGS) $(INCLUDES) $< -o $@

$(OUTPUTLIB): $(MAIN_OBJECTS)
	$(CC) $(LIBS) $(LDFLAGS) $(LIBS) $(MAIN_OBJECTS) -o $@

clean:
	rm -rf $(OUTPUTLIB)
	rm -rf $(MAIN_BUILD_DIR)/*
