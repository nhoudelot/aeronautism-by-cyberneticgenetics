CC = gcc
CFLAGS = -Wall -W -pedantic
CLIBS = -lGL -lGLU -lX11 -lXrandr -lm -lpng -lvorbisfile -lasound

DEMO = aeronautism# demo name here

$(DEMO): main.o texture.o mesh3d.o cg_music.o cg_x11.o cg_time.o shader.o particle.o poligonexplode.o $(DEMO).o
	$(CC) $(CLIBS) *.o -o $(DEMO)
	mv $(DEMO) ../

# Common demo stuff
main.o: main.c
	$(CC) main.c -c
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
$(DEMO).o: mesh3d.h texture.h $(DEMO).c $(DEMO).h
	$(CC) $(CFLAGS) $(DEMO).c -c

clean:
	@echo Cleaning up...
	@rm -f *.o *~ core* 
	@echo Done.

