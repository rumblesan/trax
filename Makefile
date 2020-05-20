CC                = clang
CFLAGS            = -Wall -g
VPATH             = src
LIBS              = -llua

TEST_C_FLAGS      = $(CFLAGS)
TEST_LIBS         =

MAIN_BUILD_DIR    = build/main
TEST_BUILD_DIR    = build/tests
HEADER_DIRS       = include

INCLUDES          = $(addprefix -I, $(HEADER_DIRS))

CORE_SOURCES      = $(notdir $(wildcard src/core/*.c))
TEST_SOURCES      = $(notdir $(wildcard src/tests/*.c)) $(CORE_SOURCES)
MAIN_SOURCES      = main.c $(CORE_SOURCES)

MAIN_OBJECTS      = $(addprefix $(MAIN_BUILD_DIR)/, $(MAIN_SOURCES:.c=.o))
TEST_OBJECTS      = $(addprefix $(TEST_BUILD_DIR)/, $(TEST_SOURCES:.c=.o))

EXECUTABLE        = trax
TEST_EXECUTABLE   = $(addprefix $(TEST_BUILD_DIR)/, test_$(EXECUTABLE))

.PHONY: clean

all: $(EXECUTABLE)

$(MAIN_BUILD_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(EXECUTABLE): $(MAIN_OBJECTS)
	$(CC) $(LIBS) $(MAIN_OBJECTS) -o $@

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(MAIN_BUILD_DIR)/*
