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

#include <stdlib.h>

#include "utils.h"
#include "3dstarfield.h"

static SDL_Surface *surface;
static Uint16 nstars;
static Uint16 centerx, centery;
static SDL_Color colors[256];

typedef struct 
{
  float xpos, ypos;
  short zpos, speed;
  Uint8 color;
} STAR;

static STAR *stars;

static void TDEC_init_star(STAR* star, int i)
{
  /* randomly init stars, generate them around the center of the screen */
  
  star->xpos =  -10.0 + (20.0 * (rand()/(RAND_MAX+1.0)));
  star->ypos =  -10.0 + (20.0 * (rand()/(RAND_MAX+1.0)));
  
  star->xpos *= 3072.0; /*change viewpoint */
  star->ypos *= 3072.0;

  star->zpos =  i;
  star->speed =  2 + (int)(2.0 * (rand()/(RAND_MAX+1.0)));

  star->color = i >> 2; /*the closer to the viewer the brighter*/
}

void TDEC_init_3dstarfield(SDL_Surface *s, Uint16 number_of_stars)
{
  Uint16 i;

  surface = s;
  nstars = number_of_stars;

  centerx = surface->w >> 1;
  centery = surface->h >> 1;

  /* create and set palette */
  colors[0].r = 0;
  colors[0].g = 0;
  colors[0].b = 0;

  for (i = 0; i < 255; ++i)
    {
      colors[255 - i].r = i;
      colors[255 - i].g = i;
      colors[255 - i].b = i;
    }
  
  if (surface->format->palette)
    {
      SDL_SetPalette(surface, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256); 
    }

  /*create stars*/
  stars = (STAR*)malloc(nstars * sizeof(STAR));

  for (i = 0; i < nstars; i++)
    {
      TDEC_init_star(stars + i, i + 1);
    }
}

void TDEC_draw_3dstarfield(void)
{
  SDL_Color *c;
  int tempx, tempy;  
  Uint16 i;

  if (SDL_MUSTLOCK(surface))
    {
      SDL_LockSurface(surface);
    }

  /* clear screen */
  
  SDL_FillRect(surface, 0, SDL_MapRGB(surface->format, 0, 0, 0));
    
  /* move and draw stars */
      
  for (i = 0; i < nstars; ++i)
    {
      stars[i].zpos -= stars[i].speed;
      
      if (stars[i].zpos <= 0)
	{
	  TDEC_init_star(stars + i, i + 1);
	}
      
      /*compute 3D position*/
      tempx = (stars[i].xpos / stars[i].zpos) + centerx;
      tempy = (stars[i].ypos / stars[i].zpos) + centery;

      if (tempx < 0 || tempx > surface->w - 1 || tempy < 0 || tempy > surface->h - 1) /*check if a star leaves the screen*/
	{
	  TDEC_init_star(stars + i, i + 1);
	  continue;
	}
      
      c = &colors[stars[i].color];
      TDEC_put_pixel(surface, tempx, tempy, SDL_MapRGB(surface->format, c->r, c->g, c->b));
    }

  if (SDL_MUSTLOCK(surface))
    {
      SDL_UnlockSurface(surface);
    }
}

inline void TDEC_free_3dstarfield(void)
{
  free(stars);
}
