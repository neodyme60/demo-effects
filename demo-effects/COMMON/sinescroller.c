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

#include "scroller.h"
#include "sinescroller.h"

static SDL_Surface* scroll_surface;
static short aSin[360];
static SDL_Surface *surface;
static Uint16 displacement;
static SDL_Rect srect2;
static SDL_Rect drect;
static SDL_Rect srect;
static SDL_Rect frect;
static Uint16 sine_index = 0;
static Uint8 pixels;
static char scroll_id;

void TDEC_init_sine_scroller(SDL_Surface *s, char *_text, char *font, char *_characters,
			     Uint8 character_width, Uint8 character_height, Uint16 amplitude, Uint8 pixel_width)
{
  float rad;
  Uint16 i, centery;
  SDL_Surface *temp;

  if ((scroll_id = TDEC_add_scroller(_text, font, _characters, character_width, character_height)) == -1)
    {
      printf("Error, initiating sine scroller\n");
      return;
    }

  surface = s;
  centery = surface->h >> 1;
  pixels = pixel_width;

  srect2.x = srect2.y = drect.x = drect.y = srect.y = frect.y = 0;
  srect2.w = drect.w = pixels;
  srect2.h = drect.h = frect.h = srect.h = character_height;
  srect.x = 2;
  srect.w = surface->w + character_width;
  frect.x = surface->w;
  frect.w = character_width;

  /*create sin lookup table */
  for (i = 0; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      aSin[i] = centery + (short)((sin(rad) * (float)amplitude));
    }
  
  /* create scroll surface, this surface must be wider than the surface width to be able to place the characters */
  temp = SDL_CreateRGBSurface(surface->flags, surface->w + character_width, character_height, surface->format->BitsPerPixel, 
			      surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask); 
  scroll_surface = SDL_ConvertSurface(temp, surface->format, surface->flags);
  SDL_FreeSurface(temp);

  displacement = 0;
}

void TDEC_draw_sine_scroller(void)
{
  Uint16 i;
  
  /* scroll scroll_surface to the left */
  SDL_BlitSurface(scroll_surface, &srect, scroll_surface, 0);

  displacement += 2;
      
  if (displacement > 30)
    {
      TDEC_draw_font_char(scroll_id, TDEC_get_font_char(scroll_id), scroll_surface, &frect);
      displacement = 0;
    }
    
  /* clean sinus area */
  SDL_FillRect(surface, 0, SDL_MapRGB(surface->format, 0, 0, 0));
   
  /* create sinus in scroll */
  for (i = 0; i < surface->w; i += pixels)
    {
      srect2.x = drect.x = i;
      drect.y = aSin[(sine_index + i) % 360];
      SDL_BlitSurface(scroll_surface, &srect2, surface, &drect);
    }
  sine_index += 6;
  sine_index %= 360;
}

void TDEC_free_sine_scroller(void)
{
  SDL_FreeSurface(scroll_surface);
  TDEC_free_scroller(scroll_id);
}



