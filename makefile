CC := gcc# i686-w64-mingw32-gcc
LIBFOLDER := lib# libwin32

LD_FLAGS_WIN32 := $(LIBFOLDER)/libcommon.a  $(LIBFOLDER)/libcore.a  $(LIBFOLDER)/libglad.a -lglfw3 -pthread -lm -lgdi32 -lopengl32 -Wl,-subsystem,windows

LD_FLAGS :=  $(LIBFOLDER)/libcommon.a  $(LIBFOLDER)/libcore.a  $(LIBFOLDER)/libglad.a -lGL -lglfw3 -pthread -ldl -lGL -lrt -lXrandr -lXi -lXinerama -lX11 -lm
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
	$(CC) $(CC_FLAGS) -c $< -o $@ -Iinclude

glad: $(OBJ_GLAD)
	mkdir -p $(LIBFOLDER)
	ar -crs  $(LIBFOLDER)/lib$@.a $^
	(cd src/glad; rm *.o)

common: $(OBJ_COMMON)
	mkdir -p  $(LIBFOLDER)
	ar -crs  $(LIBFOLDER)/lib$@.a $^
	(cd src/common; rm *.o)

core: $(OBJ_CORE)
	mkdir -p  $(LIBFOLDER)
	ar -crs  $(LIBFOLDER)/lib$@.a $^
	(cd src/core; rm *.o)

application: $(SRC_APP)
	$(CC) -Iinclude $(CC_FLAGS) -o $@ $^ $(LD_FLAGS) 

.PHONY: run
run:
	./application

all: common glad core application


mono2src:
	mkdir -p ./bin
	gcc -o ./bin/$@ ./tools/$@.c -lm 


.PHONY: font
font:
	./bin/mono2src assets/font.png src/app/fontsrc.h
