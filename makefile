LD_FLAGS :=  lib/libcommon.a lib/libcore.a lib/libglad.a -lGL -lglfw3 -pthread -ldl -lGL -lrt -lXrandr -lXi -lXinerama -lX11 -lm
CC_FLAGS :=  -Wall -O1

SRC_GLAD := $(wildcard src/glad/*.c)
OBJ_GLAD := $(patsubst %.c, %.o, $(SRC_GLAD))

SRC_COMMON := $(wildcard src/common/*.c)
OBJ_COMMON := $(patsubst %.c, %.o, $(SRC_COMMON))

SRC_CORE := $(wildcard src/core/*.c)
OBJ_CORE := $(patsubst %.c, %.o, $(SRC_CORE))

SRC_APP := $(wildcard src/app/*.c)


.PHONY: shader
shader:
	(cd shader; python3 convert.py)

%.o: %.c
	gcc $(CC_FLAGS) -c $< -o $@ -Iinclude

glad: $(OBJ_GLAD)
	mkdir -p lib
	ar -crs lib/lib$@.a $^
	(cd src/glad; rm *.o)

common: $(OBJ_COMMON)
	mkdir -p lib
	ar -crs lib/lib$@.a $^
	(cd src/common; rm *.o)

core: $(OBJ_CORE)
	mkdir -p lib
	ar -crs lib/lib$@.a $^
	(cd src/core; rm *.o)

application: $(SRC_APP)
	gcc -Iinclude $(CC_FLAGS) -o $@ $^ $(LD_FLAGS) 

.PHONY: run
run:
	./application

all: common glad core application
