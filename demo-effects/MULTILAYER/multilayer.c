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

/*note that the code has not been fully optimized*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

void quit( int code )
{
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_Quit( );

  TDEC_free_fire();
  TDEC_free_jumping_scroller();
  TDEC_reset_layering();
  TDEC_print_fps();
  
  /* Exit program. */
  exit( code );
}

void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym )
      {
      case SDLK_ESCAPE:
        quit( 0 );
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
            /* Handle quit requests*/
            quit( 0 );
            break;
	}
    }
}

void init()
{
  Uint32 i, fire_index, jscroller_index;
  char *text = " A jumping scroller - pretty retro isn't it? - cheers -   W.P. van Paassen -       starting over again in -      9  8  7  6  5  4  3  2  1  ";
  char *characters = " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

  TDEC_init_layering();
  TDEC_init_plasma(TDEC_get_layer(TDEC_BACKGROUND_LAYER));
  jscroller_index = TDEC_add_layer(SCREEN_WIDTH, 132, 0, SCREEN_HEIGHT - 132, 0xFF);
  TDEC_init_jumping_scroller(TDEC_get_layer(jscroller_index), text, "../GFX/font.pcx", characters, 16, 32, SCREEN_WIDTH, 100);
  fire_index = TDEC_add_layer(SCREEN_WIDTH, 60, 0, 300, 0xFF);
  TDEC_init_fire(TDEC_get_layer(fire_index), SCREEN_WIDTH, 60, 0, 0);

 /*disable events */
  
  for (i = 0; i < SDL_NUMEVENTS; ++i)
    {
      if (i != SDL_KEYDOWN && i != SDL_QUIT)
	{
	  SDL_EventState(i, SDL_IGNORE);
	}
    }
  
  SDL_ShowCursor(SDL_DISABLE);
}

int main( int argc, char* argv[] )
{
 if (argc > 1)
    {
      printf("Multi layered effects - W.P. van Paassen - 2003\n");
      return -1;
    }

 if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_ANYFORMAT | SDL_HWSURFACE | SDL_HWPALETTE | SDL_SRCALPHA/* | SDL_FULLSCREEN*/))
   quit(1);

 TDEC_init_timer();
 TDEC_set_fps(5);

 SDL_WM_SetCaption("Multi layered effects", "");
  
 init();

 while( 1 ) 
   {
     TDEC_new_time();

     process_events();

     TDEC_draw_plasma();
     TDEC_draw_fire();
     TDEC_draw_jumping_scroller();

     if (TDEC_fps_ok())
       {
	 TDEC_flatten_layers();
	 TDEC_draw_layers();
       }
    }
  
  return 0; /* never reached */
}





