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

#include <stdlib.h>
#include <math.h>

#include "scroller.h"
#include "jumpingscroller.h"

static SDL_Surface *surface;
static short aSin[540];

typedef struct 
{
  short xpos;
  Uint16 sin_index;
  Uint16 font_pos;
} LETTER; 

static LETTER *letters;
static Uint8 nletters;
static Uint16 displacement;
static Uint16 width;
static Uint16 height;
static SDL_Rect frect;
static SDL_Rect srect;
static char scroll_id;

void TDEC_init_jumping_scroller(SDL_Surface *s, char *_text, char *font, char *_characters,
				Uint8 character_width, Uint8 character_height,
				Uint16 _width, Uint16 _height)
{
  float rad;
  Uint16 i, j;

  width = _width;
  height = _height;
  short centery = height / 2;

  if ((scroll_id = TDEC_add_scroller(_text, font, _characters, character_width, character_height)) == -1)
    {
      printf("Error, initiating jumping scroller\n");
      return;
    }

  surface = s;
 
  /*create sin lookup table */

  for (i = 0, j = 0; i < 180; i++)
    {
      rad =  (float)j * 0.0174532; 
      aSin[i] = centery - (short)((sin(rad) * (height / 2.0)));

      if (!( (i + 1) % 2))
	{
	  j++;
	}
    } 
  for (i = 90, j = 90; i < 270; i++)
    {
      rad =  (float)j * 0.0174532; 
      aSin[i + 90] = centery - (short)((sin(rad) * (height / 2.0)));

      if (!( (i + 1) % 2))
	{
	  j++;
	}
    }
 
  for (i = 180; i < 270; i++)
    {
      rad =  (float)i * 0.0174532; 
      aSin[i + 180] = centery - (short)((sin(rad) * (height / 2.0)));
    } 
  for (i = 270; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      aSin[i + 180] = centery - (short)((sin(rad) * (height / 2.0)));
    }

  nletters = width / character_width;
  letters = (LETTER*)malloc(nletters * sizeof(LETTER));
  
  /* reset letters */
  for (i = 0; i < nletters; ++i)
    {
      letters[i].xpos = -character_width;
    }

  displacement = 0;

  frect.h = TDEC_get_character_height(scroll_id);
  srect.w = TDEC_get_character_width(scroll_id);
  srect.h = TDEC_get_character_height(scroll_id);
}

void TDEC_draw_jumping_scroller(void)
{
  Uint16 i;
  
  /* print character? */
  if (displacement > 20)
    {
      /* init new character */
      
      for (i = 0; i < nletters; ++i)
	{
	  /* find an unused letter */
	  
	  if (letters[i].xpos < -TDEC_get_character_width(scroll_id))
	    {
	      SDL_Rect *r = TDEC_get_font_char(scroll_id);
	      letters[i].xpos = width;
	      letters[i].sin_index = 0;
	      letters[i].font_pos = r->x;
	      break;
	    }
	}
      
      displacement = 0;
    }
  
  displacement += 2;
  
  /* clear screen */
  
  SDL_FillRect(surface, 0, SDL_MapRGB(surface->format, 0, 0, 0));
  
  /* update letter positions */
  
  for (i = 0; i < nletters; ++i)
    {
      letters[i].xpos -= 3;
      if (letters[i].xpos  > -TDEC_get_character_width(scroll_id))
	{
	  letters[i].sin_index += 8;
	  letters[i].sin_index %= 540;
	  
	  if (letters[i].xpos < 0)
	    {
	      /* correct font and position when reaching left border */
	      
	      short diff = letters[i].xpos;
	      frect.x = letters[i].font_pos - 1 - diff;
	      frect.w = TDEC_get_character_width(scroll_id) +  diff;
	      srect.x = 0;
	    }
	  else
	    {
	      frect.x = letters[i].font_pos - 1;
	      frect.w = TDEC_get_character_width(scroll_id);
	      srect.x = letters[i].xpos;
	    }

	  srect.y = aSin[letters[i].sin_index];
	  
	  TDEC_draw_font_char(scroll_id, &frect, surface, &srect);
	}
    }
}

void TDEC_free_jumping_scroller(void)
{
  free(letters);
  TDEC_free_scroller(scroll_id);
}
