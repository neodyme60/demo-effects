/* Copyright (C) 2003 W.P. van Paassen - peter@paassen.tmfweb.nl

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

#include "scroller.h"

static SDL_Surface *font_surface;
static char *text;
static char *characters;
static char *text_pointer;
static Uint8 cwidth;
static Uint8 cheight;
static SDL_Rect frect;

void TDEC_init_scroller(char *_text, char *font, char *_characters,
			Uint8 character_width, Uint8 character_height)
{
  text = _text;
  text_pointer = text;
  font_surface = (SDL_Surface*)(IMG_Load(font));
  characters = _characters;
  cwidth = character_width;
  cheight = character_height;
  frect.x = 0;
  frect.y = 0;
  frect.w = cwidth - 1;
  frect.h = cheight;
}

/* determine the font character */
Uint16 TDEC_compute_font_pos(char scroll_char)
{
  char* p = characters;
  Uint16 pos = 0;

  if (scroll_char == '\0')
    {
      text_pointer = text;
      scroll_char = *text_pointer++;
    }

  while (*p++ != scroll_char)
    {
      pos += cwidth;
    }

  if (pos > 0)
    return pos - 1;

  return 0;
}

SDL_Rect* TDEC_get_font_char(void) 
{
  /* determine font character according to position in scroll text */
  
  frect.x = TDEC_compute_font_pos(*text_pointer++);

  return &frect;
}

void TDEC_free_scroller(void)
{
  SDL_FreeSurface(font_surface);
}

Uint8 TDEC_scroller_ready(void)
{
  return *text_pointer == '\0';
}

inline Uint8 TDEC_get_character_width(void)
{
  return cwidth;
}

inline Uint8 TDEC_get_character_height(void)
{
  return cheight;
}

inline void TDEC_draw_font_char(SDL_Rect *font_rect, SDL_Surface *destination, SDL_Rect *dest_rect)
{
  SDL_BlitSurface(font_surface, font_rect, destination, dest_rect);
}
