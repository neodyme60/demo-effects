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

#include "video.h"

int TDEC_init_video( Uint16 width, Uint16 height, int bpp, int flags)
{
  /* First, initialize SDL's video and timer subsystem. */
  
  if( SDL_Init( SDL_INIT_VIDEO) < 0 ) 
    {
      /* Failed, exit. */
      fprintf( stderr, "Video initialization failed: %s\n",
	       SDL_GetError( ) );
      return 0;
    }
  
  /* Let's get some video information. */
  info = SDL_GetVideoInfo( );
  
  if( !info ) {
    /* This should probably never happen. */
    fprintf( stderr, "Video query failed: %s\n",
             SDL_GetError( ) );
    return 0;
  }
  
  /*
   * Set the video mode
   */
  if( (screen = SDL_SetVideoMode( width, height, bpp, flags )) == 0 ) 
    {
      /* 
       * This could happen for a variety of reasons,
       * including DISPLAY not being set, the specified
       * resolution not being available, etc.
       */
      fprintf( stderr, "Video mode set failed: %s\n",
	       SDL_GetError( ) );
      return 0;
    }

  return 1;
}





