/* Copyright (C) 2002-2003 W.P. van Paassen - peter@paassen.tmfweb.nl

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

#include <math.h>

#include "layers.h"
#include "utils.h"
#include "fade.h"

static SDL_Surface *_fade_surface;

void fade_LTX_init_effect(SDL_Surface *s, void (*restart)(void), va_list parameters)
{
  Uint32 c1 = 0, c2 = 0;
  Uint8 r, g, b, i = 0;

   _fade_surface = s;

   /* find nearest color to black. Surface can't be black because that's the transparant color when blitting, see SDL_SetColorKey */
   
   c1 = SDL_MapRGB(_fade_surface->format, 0, 0, 0);
   SDL_GetRGB(c1, _fade_surface->format, &r, &g, &b);

   for (; i < 255; ++i)
     {
       if ((c2 = SDL_MapRGB(_fade_surface->format, r + i, g + i, b + i)) > c1)
	 {
	   SDL_FillRect(_fade_surface, 0, c2);
	   break;
	 }
     }
}

void fade_LTX_draw_effect(void)
{
}

void fade_LTX_free_effect(void)
{
}

Uint8 fade_LTX_is_filter(void)
{
  return TDEC_NO_FILTER;
}
