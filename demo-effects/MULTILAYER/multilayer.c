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

  TDEC_free_layers();
  
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
  Uint32 i;
  char *text = " tDeC TdEc ";
  char *text2 = "The Demo Effects Collection ";

  if (!TDEC_add_layer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/PLASMA/plasma", 4))
    {
      exit(1);
    }
  if (!TDEC_add_layer(SCREEN_WIDTH, 200, 0, SCREEN_HEIGHT - (100 + 32) , 0xFF, "../PLUGINS/JUMPINGSCROLLER/jumpingscroller",  text2, TDEC_FONT1, TDEC_FONT1_CHARACTERS, 16, 32, SCREEN_WIDTH, 100))
    {
      exit(1);
      }
  if (!TDEC_add_layer(SCREEN_WIDTH, 60, 0, SCREEN_HEIGHT - 60, 0xFF, "../PLUGINS/FIRE/fire", SCREEN_WIDTH, 60, 0, 0))
    {
      exit(1);
      }
  if (!TDEC_add_layer(SCREEN_WIDTH, 150, 0, 0, 0xFF, "../PLUGINS/SINESCROLLER/sinescroller",  text, TDEC_FONT1, TDEC_FONT1_CHARACTERS, 16, 32, 40, 2))
    {
      exit(1);
      }


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
      printf("Multi layered run-time pluggable effects - W.P. van Paassen - 2003\n");
      return -1;
    }

 if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_DOUBLEBUF | SDL_ANYFORMAT | SDL_HWSURFACE | SDL_HWPALETTE | SDL_SRCALPHA /*|SDL_FULLSCREEN*/))
   quit(1);

 TDEC_init_timer();
 TDEC_set_fps(60);

 SDL_WM_SetCaption("Multi layered run-time pluggable effects", "");
  
 init();

 while( 1 ) 
   {
     TDEC_new_time();
     
     process_events();
     
     TDEC_draw_layers();
     
     TDEC_fps_ok();
   }
}





