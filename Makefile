PROJECT_NAME   := cointab
SRC_DIR        := src
LIB_DIR        := lib
BUILD_DIR      := build
CJSON_SRC_DIR  := $(LIB_DIR)/cJSON
LIBS           := $(wildcard $(LIB_DIR)/*)
LIB_HEADERS    :=
LIB_HEADERS    += $(CJSON_SRC_DIR)/cJSON.h
SOURCES        :=
SOURCES        += $(SRC_DIR)/$(PROJECT_NAME).c
SOURCES        += $(wildcard $(SRC_DIR)/ct_*.c)
SOURCES        += $(CJSON_SRC_DIR)/cJSON.c
SOURCE_HEADERS :=
SOURCE_HEADERS += $(wildcard $(SRC_DIR)/ct_*.h)
PROGRAM        := $(BUILD_DIR)/$(PROJECT_NAME)

CC     := gcc
CFLAGS :=
CFLAGS += -std=c99
CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -O0
CFLAGS += -I$(CJSON_SRC_DIR)
CFLAGS += -I/usr/local/opt/openssl@1.1/include
LDLIBS :=
LDLIBS += -lcurl
LDLIBS += -lncurses
LDLIBS += -lwebsockets

all: $(PROGRAM)
.PHONY: all

$(PROGRAM): $(SOURCES) $(SOURCE_HEADERS) | $(BUILD_DIR)
	$(CC) -o $@ $(SOURCES) $(CFLAGS) $(LDLIBS)

$(LIB_DIR)/cJSON/cJSON.h: $(LIB_DIR)
	git submodule update --init --recursive --remote

$(LIB_DIR) $(BUILD_DIR):; mkdir $@

run: $(PROGRAM); ./$<
.PHONY: run

test: $(PROGRAM); valgrind --leak-check=full $<
.PHONY: test

clean:; $(RM) -rf $(BUILD_DIR)
.PHONY: clean

