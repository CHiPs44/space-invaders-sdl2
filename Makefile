# cf. https://stackoverflow.com/questions/1433204/how-do-i-use-extern-to-share-variables-between-source-files

# Minimal makefile

PROGRAM = space-invaders
FILES.c = src/aliens.c src/debug.c src/font.c src/graphics.c src/items.c src/main.c src/player.c src/saucer.c src/scene.c src/shields.c src/sprite.c
FILES.h = include/*.h
FILES.o = ${FILES.c:.c=.o}

CC      = gcc
CFLAGS  = -std=c11 -g -O3 -Wall -Wextra -Werror -Wstrict-prototypes -Wmissing-prototypes -pedantic
LDFLAGS =
# LDLIBS  = $(sdl2-config --cflags --libs) -lSDL2_image
LDLIBS  = -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lSDL2_image

all:    ${PROGRAM}

${PROGRAM}: ${FILES.o}
	${CC} -o $@ ${CFLAGS} ${FILES.o} ${LDFLAGS} ${LDLIBS}

.o: ${FILES.h}

# If it exists, prog1.dSYM is a directory on macOS
DEBRIS = a.out core *~ *.dSYM
RM_FR  = rm -fr

clean:
	${RM_FR} ${FILES.o} ${PROGRAM} ${DEBRIS}

# EOF
