/*
 * LD_PRELOAD interposer library for SDL to prevent full-screen grabbing.
 *
 * - Replaces SDL_FULLSCREEN flag with SDL_NOFRAME in SDL_SetVideoMode.
 *
 * Copyright David Reiss.  MIT License
 */

#define _GNU_SOURCE
#include <assert.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdint.h>


#define SDL_FULLSCREEN 0x80000000u
#define SDL_NOFRAME 0x00000020u

void* SDL_SetVideoMode(int width, int height, int bitsperpixel, uint32_t flags) {
  static void*(*real_func)(int,int,int,uint32_t) = NULL;

  if (real_func == NULL) {
    real_func = dlsym(RTLD_NEXT, "SDL_SetVideoMode");
    assert(real_func != NULL);
  }

  if (flags & SDL_FULLSCREEN) {
    flags &= ~SDL_FULLSCREEN;
    flags |= SDL_NOFRAME;
  }

  return real_func(width, height, bitsperpixel, flags);
}
