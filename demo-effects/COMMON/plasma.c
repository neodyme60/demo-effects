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

#include <math.h>

#include "utils.h"
#include "plasma.h"

static SDL_Surface *surface;
static Uint16 pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0, tpos1, tpos2, tpos3, tpos4;
static int aSin[512];
static SDL_Color colors[256];
static Uint8 pixelspcolor;

void TDEC_init_plasma(SDL_Surface *s, Uint8 pixels_per_color)
{
  int i;
  float rad;
 
  surface = s;
  pixelspcolor = pixels_per_color;

  /*create sin lookup table */
  for (i = 0; i < 512; i++)
    {
      rad =  ((float)i * 0.703125) * 0.0174532; /* 360 / 512 * degree to rad, 360 degrees spread over 512 values to be able to use AND 512-1 instead of using modulo 360*/
      aSin[i] = sin(rad) * 1024; /*using fixed point math with 1024 as base*/
    }
  
  /* create color palette */
  for (i = 0; i < 64; ++i)
    {
      colors[i].r = i << 2;
      colors[i].g = 255 - ((i << 2) + 1); 
      colors[i+64].r = 255;
      colors[i+64].g = (i << 2) + 1;
      colors[i+128].r = 255 - ((i << 2) + 1);
      colors[i+128].g = 255 - ((i << 2) + 1);
      colors[i+192].g = (i << 2) + 1; 
    } 

  if (surface->format->palette)
    {
      SDL_SetPalette(surface, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256); 
    }
}

void TDEC_draw_plasma()
{
  Uint16 i,j;
  Uint8 index, k, l;
  int x;  
  
  /* Lock the screen for direct access to the pixels */
  if (SDL_MUSTLOCK(surface))
    {
      SDL_LockSurface(surface);
    }
  
  tpos4 = pos4;
  tpos3 = pos3;
  
  for (i = 0; i + pixelspcolor - 1 < surface->h; i += pixelspcolor)
    {
      tpos1 = pos1 + 5;
      tpos2 = pos2 + 3;
      
      tpos3 &= 511;
      tpos4 &= 511;
      
      for (j = 0; j  + pixelspcolor - 1 < surface->w;)
	{
	  tpos1 &= 511;
	  tpos2 &= 511;
	  
	  x = aSin[tpos1] + aSin[tpos2] + aSin[tpos3] + aSin[tpos4]; //actual plasma calculation
	  
	  index = 128 + (x >> 4); //fixed point multiplication but optimized so basically it says (x * (64 * 1024) / (1024 * 1024)), x is already multiplied by 1024
	  
	  for (k = 0; k < pixelspcolor; ++j, ++k)
	    {
	      for (l = 0; l < pixelspcolor; ++l)
		{
		  TDEC_put_pixel(surface, j, i + l, SDL_MapRGB(surface->format, colors[index].r, colors[index].g, colors[index].b));
		}
	    }
	  
	  tpos1 += 5; 
	  tpos2 += 3; 
	}
      
      tpos4 += 3;
      tpos3 += 1;
    }
  
  /* move plasma */
  
  pos1 +=9;
  pos3 +=8;
  
  if (SDL_MUSTLOCK(surface))
    {
      SDL_UnlockSurface(surface);
    }
}
  
