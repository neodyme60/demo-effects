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

#include "fire.h"

static SDL_Surface *surface;
static SDL_Color colors[256];
static unsigned short width;
static unsigned short height;
static unsigned short xpos;
static unsigned short ypos;
static Uint8 *fire;

void TDEC_init_fire(SDL_Surface* s, unsigned short _width, unsigned short _height, unsigned short _xpos, unsigned short _ypos)
{
  unsigned int i;

  surface = s;

  if (_width > surface->w || _width == 0)
    {
      width = surface->w;
    }
  else
    {
      width = _width;
    }

  if (_height > surface->h || _height == 0)
    {
      height = surface->h;
    }
  else
    {
      height = _height;
    }

  if (_xpos > surface->w || _xpos + width >= surface->w)
    {
      xpos = 0;
    }
  else
    {
      xpos = _xpos;
    }

  if (_ypos > surface->h || _ypos + height >= surface->h)
    {
      ypos = 0;
    }
  else
    {
      ypos = _ypos;
    }

  /* create fire array */

  fire = (Uint8*)calloc(width * height, sizeof(Uint8));
 
  /* create a suitable fire palette, this is crucial for a good effect */
  /* black to blue, blue to red, red to yellow, yellow to white*/
  
 for (i = 0; i < 32; ++i)
    {
      /* black to blue, 32 values*/
      colors[i].b = i << 1;

      /* blue to red, 32 values*/
      colors[i + 32].r = i << 3;
      colors[i + 32].b =  64 - (i << 1);

      /*red to yellow, 32 values*/
      colors[i + 64].r = 255;
      colors[i + 64].g = i << 3;

      /* yellow to white, 162 */
      colors[i + 96].r = 255;
      colors[i + 96].g = 255;
      colors[i + 96].b = i << 2;
      colors[i + 128].r = 255;
      colors[i + 128].g = 255;
      colors[i + 128].b = 64 + (i << 2);
      colors[i + 160].r = 255;
      colors[i + 160].g = 255;
      colors[i + 160].b = 128 + (i << 2);
      colors[i + 192].r = 255;
      colors[i + 192].g = 255;
      colors[i + 192].b = 192 + i;
      colors[i + 224].r = 255;
      colors[i + 224].g = 255;
      colors[i + 224].b = 224 + i;
    } 
 
 if (surface->format->palette)
   {
     SDL_SetPalette(surface, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256); 
   }
}

void TDEC_draw_fire()
{
  Uint16 temp;
  int i,j, index;  

  if (SDL_MUSTLOCK(surface))
    {
      SDL_LockSurface(surface);
    }

  /* draw random bottom line in fire array */
  
  j = width * (height - 1);
  for (i = 0; i < width; ++i)
    {
      int random = 1 + (int)(16.0 * (rand()/(RAND_MAX+1.0)));
      if (random > 9) /* the lower the value, the intenser the fire, compensate a lower value with a higher decay value*/
	fire[j + i] = 255; /*maximum heat*/
      else
	fire[j + i] = 0;
    }  
  
  /* move fire upwards, start at bottom*/

  while (j >= width)
    {
      for (i = 0; i < width; ++i)
	{
	  if (i == 0) /* at the left border */
	    {
	      temp = fire[j];
	      temp += fire[j + 1];
	      temp += fire[j - width];
	      temp /=3;
	    }
	  else if (i == width - 1) /* at the right border */
	    {
	      temp = fire[j + i];
	      temp += fire[j - 1];
	      temp += fire[j + i - 1];
	      temp /= 3;
	    }
	  else
	    {
	      temp = fire[j + i];
	      temp += fire[j + i + 1];
	      temp += fire[j + i - 1];
	      temp += fire[j - width + i];
	      temp >>= 2;
	    }

	  if (temp > 28)
	    {
	      temp -= 1; /* decay */
	    }
	  else
	    {
	      temp = 0;
	    }
	  
	  fire[j - width + i] = temp;
	}
      j -= width;
    }      

  /* draw fire array to surface */

  for (i = 0; i < height; ++i)
    {
      for (j = 0; j < width; ++j)
	{
	  Uint16 temp = width * i;
	  TDEC_put_pixel(surface, xpos + j , ypos + i, SDL_MapRGB(surface->format, colors[fire[temp + j]].r, 
								  colors[fire[temp + j]].g, colors[fire[temp + j]].b));
	}
    }
  
  if (SDL_MUSTLOCK(surface))
    {
      SDL_UnlockSurface(surface);
    }
}

void TDEC_free_fire()
{
  free(fire);
}
