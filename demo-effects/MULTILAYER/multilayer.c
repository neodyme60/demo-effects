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

static char plasma;
static char fire;
static char copper;
static char stars;
static char lens;
static Uint8 change = 1;

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

void restart_sine(void)
{
  if (change)
    {
      TDEC_disable_layer(plasma);
      TDEC_disable_layer(fire);
      TDEC_enable_layer(stars);
      TDEC_enable_layer(copper);
      change = 0;
    }
  else
    {
      TDEC_disable_layer(stars);
      TDEC_disable_layer(copper);
      TDEC_enable_layer(plasma);
      TDEC_enable_layer(fire);
      change = 1;
    }
}

void init()
{
  Uint32 i;
  char *text = " TDEC Demo ";
  char *text2 = "Run-Time Pluggable Multi Effects and Filter system ";

  if ((plasma = TDEC_add_effect(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/PLASMA/plasma", 
		      TDEC_NO_RESTART_CALLBACK, 4)) == -1)
    {
      exit(1);
    }
  if (TDEC_add_effect(SCREEN_WIDTH, 200, 0, SCREEN_HEIGHT - (100 + 32) , 0xFF, "../PLUGINS/JUMPINGSCROLLER/jumpingscroller",
		      TDEC_NO_RESTART_CALLBACK, text2, TDEC_FONT1, TDEC_FONT1_CHARACTERS, 16, 32, SCREEN_WIDTH, 100) == -1)
    {
      exit(1);
    }
  if ((fire = TDEC_add_effect(SCREEN_WIDTH, 60, 0, SCREEN_HEIGHT - 60, 0xFF, "../PLUGINS/FIRE/fire", 
		      TDEC_NO_RESTART_CALLBACK, SCREEN_WIDTH, 60, 0, 0)) == -1)
    {
      exit(1);
    }
  if (TDEC_add_effect(SCREEN_WIDTH, 150, 0, 0, 0xFF, "../PLUGINS/SINESCROLLER/sinescroller",
		      &restart_sine, text, TDEC_FONT1, TDEC_FONT1_CHARACTERS, 16, 32, 40, 2) == -1)
    {
      exit(1);
    }
  if ((stars = TDEC_add_effect(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/STARFIELD/starfield",
		      TDEC_NO_RESTART_CALLBACK, 750)) == -1)
    {
      exit(1);
    }

  if ((lens = TDEC_add_effect(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0xFF, "../PLUGINS/LENS/lens",
    TDEC_NO_RESTART_CALLBACK)) == -1)
    {
      exit(1);
    }

  if ((copper = TDEC_add_effect(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0x80, "../PLUGINS/COPPERBARS/copperbars",
		      TDEC_NO_RESTART_CALLBACK, 100)) == -1)
    {
      exit(1);
    }

  TDEC_disable_layer(stars);
  TDEC_disable_layer(copper);
  
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
      printf("Run-Time Pluggable Multi Effects and Filter system - W.P. van Paassen - 2003\n");
      return -1;
    }

 if (!TDEC_set_video(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_DOUBLEBUF | SDL_ANYFORMAT | SDL_HWSURFACE | SDL_HWPALETTE | SDL_SRCALPHA/* |SDL_FULLSCREEN*/))
   quit(1);

 TDEC_init_timer();
 TDEC_set_fps(60);

 SDL_WM_SetCaption("Run-Time Pluggable Multi Effects and Filter system", "");
  
 init();

 while( 1 ) 
   {
     TDEC_new_time();
     
     process_events();
     
     TDEC_draw_layers();
     TDEC_set_layer_alpha(copper, TDEC_get_layer_alpha(copper) + 1);
     TDEC_fps_ok();
   }
}





