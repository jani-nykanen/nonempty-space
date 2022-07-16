LD_FLAGS :=  lib/libcommon.a lib/libcore.a lib/libglad.a -lGL -lglfw3 -lm -ldl
CC_FLAGS :=  -Wall -O1

SRC_GLAD := $(wildcard src/glad/*.c)
OBJ_GLAD := $(patsubst %.c, %.o, $(SRC_GLAD))

SRC_COMMON := $(wildcard src/common/*.c)
OBJ_COMMON := $(patsubst %.c, %.o, $(SRC_COMMON))


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