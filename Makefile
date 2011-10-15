all: sdlnolock.so

sdlnolock.so: sdlnolock.o
	$(CC) -shared -fpic -o $@ $<

CFLAGS = -fpic -g -O0 -Wall -Wextra -Werror

.PHONY: clean

clean:
	$(RM) sdlnolock.so sdlnolock.o
