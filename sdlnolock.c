/*
 * LD_PRELOAD interposer library for SDL to prevent full-screen grabbing.
 *
 * - Replaces SDL_FULLSCREEN flag with SDL_NOFRAME in SDL_SetVideoMode.
 * - Replaces SDL_GRAB_ON mode with SDL_GRAB_OFF in SDL_WM_GrabInput.
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


typedef enum {
  SDL_GRAB_QUERY = -1,
  SDL_GRAB_OFF = 0,
  SDL_GRAB_ON = 1
} SDL_GrabMode;

SDL_GrabMode SDL_WM_GrabInput(SDL_GrabMode mode) {
  static SDL_GrabMode(*real_func)(SDL_GrabMode) = NULL;

  if (real_func == NULL) {
    real_func = dlsym(RTLD_NEXT, "SDL_WM_GrabInput");
    assert(real_func != NULL);
  }

  if (mode == SDL_GRAB_ON) {
    mode = SDL_GRAB_OFF;
  }

  return real_func(mode);
}
