#variables de compilation
SHELL	:= /bin/sh
CC 		:= gcc
CFLAGS 	+= -O3 -flto -Wall -W -pedantic -ffast-math
CLIBS 	:= $(shell pkg-config --libs glu x11 xrandr libpng vorbisfile) -lasound -lm

TARGET = aeronautism-by-cyberneticgenetics# demo name here

#variable de nettoyage
RM_F := rm -f

#variables d’installation
INSTALL := install
INSTALL_DIR     := $(INSTALL) -p -d -o root -g root  -m  755
INSTALL_PROGRAM := $(INSTALL) -p    -o root -g root  -m  755

prefix          := /usr
exec_prefix     := $(prefix)
bindir          := $(prefix)/bin

#si code gcc4 compilé via gcc5 le standard par défaut était different
GCCVERSIONGTEQ5 := $(shell expr `gcc -dumpversion | cut -f1 -d.` \>= 5)
ifeq "$(GCCVERSIONGTEQ5)" "1"
    CFLAGS += -std=gnu90
    CXXFLAGS += -fabi-compat-version=2
endif
#compilation en paralelle si disponible
ifneq (,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
 NUMJOBS = $(patsubst parallel=%,%,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
 MAKEFLAGS += -j$(NUMJOBS)
endif

export

all: $(TARGET)

$(TARGET): main.o texture.o mesh3d.o cg_music.o cg_x11.o cg_time.o shader.o particle.o poligonexplode.o aeronautism.o
	$(CC) -flto -o $(TARGET)  *.o  $(CLIBS)

# Common demo stuff
main.o: main.c
	$(CC) -flto main.c -c
texture.o: texture.c texture.h
	$(CC) $(CFLAGS) texture.c -c
mesh3d.o: mesh3d.c mesh3d.h
	$(CC) $(CFLAGS) mesh3d.c -c
cg_music.o: cg_music.c cg_music.h
	$(CC) $(CFLAGS) cg_music.c -c
cg_x11.o: cg_x11.c cg_x11.h
	$(CC) $(CFLAGS) cg_x11.c -c
cg_time.o: cg_time.c cg_time.h
	$(CC) $(CFLAGS) cg_time.c -c
shader.o: shader.c shader.h
	$(CC) $(CFLAGS) shader.c -c
particle.o: particle.c particle.h
	$(CC) $(CFLAGS) particle.c -c
poligonexplode.o: poligonexplode.c poligonexplode.h
	$(CC) $(CFLAGS) poligonexplode.c -c
aeronautism.o: mesh3d.h texture.h aeronautism.c aeronautism.h
	$(CC) $(CFLAGS) aeronautism.c -c

clean:
	@echo Cleaning up...
	-@$(RM_F) *.o *~ core*
	-@$(RM_F) $(TARGET)
	@echo Done.

install: $(TARGET)
	$(INSTALL_DIR) $(DESTDIR)$(bindir)
	-@$(RM_F) $(DESTDIR)$(bindir)/$(TARGET)
	$(INSTALL_PROGRAM) $(TARGET) $(DESTDIR)$(bindir)
