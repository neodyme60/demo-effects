/* Copyright (C) 2002 W.P. van Paassen - peter@paassen.tmfweb.nl

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/*note that the code has not been optimized*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "tdec.h"

static short aSin[512];
static Uint16 sin_index = 0;
static Uint16 sin_global_index = 0;
static SDL_Surface *image = 0;
static SDL_Surface *flipped_image = 0;

void quit( int code )
{      
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  if (image) 
    SDL_FreeSurface(image);
  if (flipped_image) 
    SDL_FreeSurface(flipped_image);

  SDL_Quit( );

  TDEC_print_fps();
  
  /* Exit program. */
  exit( code );
}

void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym )
      {
      case SDLK_ESCAPE:
	quit(1);
        break;
      default:
        break;
      }
}

void process_events( void )
{
  /* Our SDL event placeholder. */
  SDL_Event event;
  
  /* Grab all the events off the queue. */
  while( SDL_PollEvent( &event ) ) {
    
    switch( event.type ) {
    case SDL_KEYDOWN:
      /* Handle key presses. */
      handle_key_down( &event.key.keysym );
      break;
    case SDL_QUIT:
      /* Handle quit requests */    
      quit(1);
      break;
    }
  }
}

void init()
{
    Uint16 i;
    float rad;

    image = IMG_Load("../GFX/tuxblackbg.png");
    if (!image) {
        fprintf(stderr, "Cannot open file tuxblackbg.png: %s\n", SDL_GetError());
        quit(3);
    }

    SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, image->format->palette->colors, 0, image->format->palette->ncolors);

    flipped_image = IMG_Load("../GFX/tuXperience.png");
    if (!image) {
        fprintf(stderr, "Cannot open file tuXperience.png: %s\n", SDL_GetError());
        quit(3);
    }

    /*create sin lookup table */
    for (i = 0; i < 512; i++)
      {
	rad =  (float)i * 0.0174532 * 0.703125; 
	aSin[i] = (short)((sin(rad) * 100.0));
      }

    /*disable events */
    for (i = 0; i < SDL_NUMEVENTS; ++i) {
	if (i != SDL_KEYDOWN && i != SDL_QUIT) {
	    SDL_EventState(i, SDL_IGNORE);
	}
    }
  
    SDL_ShowCursor(SDL_DISABLE);
}

int main( int argc, char* argv[] )
{
  Uint16 i;

  if (argc > 1) {
    printf("Retro Spiraltwist - W.P. van Paassen - 2002\n");
    return -1;
  }
  
  if (!TDEC_init_video(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE /*| SDL_FULLSCREEN*/))
    quit(1);
  
  TDEC_init_timer();
  
  SDL_WM_SetCaption("Retro Spiraltwist effect ", "");
  
  init();
  
  TDEC_set_fps(25);

  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();
    
      process_events();

      sin_index = sin_global_index;
      
      SDL_FillRect(screen, 0, 0);

      for (i = 0; i < image->h; ++i)
	{
	  if (aSin[sin_index] < 0)
	    TDEC_scale_copy_scanline(flipped_image, screen, i, -aSin[sin_index++]);
	  else
	    TDEC_scale_copy_scanline(image, screen, i, aSin[sin_index++]);

	  sin_index &= 511;
	}
     
      sin_global_index += 4;
      sin_global_index &= 511;
      
      if (TDEC_fps_ok()) 
	{
	  SDL_Flip(screen);
	}
    }
  
  return 0; /* never reached */
}
