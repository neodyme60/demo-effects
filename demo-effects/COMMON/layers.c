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

#include "layers.h"
#include "video.h"
#include "utils.h"

typedef struct
{
  SDL_Surface* surface;
  SDL_Rect r;
  Uint8 alpha;
} LAYER;

static LAYER* layers[NLAYERS];
static unsigned char nlayers = 0;

void TDEC_init_layering()
{
  /* check if video was set */
  if (!screen)
    {
      printf("error, video not set, call TDEC_set_video first please\n");
      return;
    }

  /* screen is the display buffer and therefore the background */
  
  LAYER *background = (LAYER*)malloc(sizeof(LAYER));
  background->surface = screen;
  background->r.x = 0;
  background->r.y = 0;
  background->r.w = screen->w;
  background->r.h = screen->h;
  background->alpha = 0xFF;

  layers[0] = background;
  nlayers = 1;

  printf("Layering-Init succesfull\n");
}

void TDEC_reset_layering()
{
  Uint32 i;
  for (i = 0; i < nlayers; ++i)
    {
      free(layers[i]);
    }
  nlayers = 0;
  printf("Layering-Reset succesfull\n");
}

/* FIXME return SDL_Surface */
unsigned int TDEC_add_layer(Uint16 width, Uint16 height, Uint16 xstart, Uint16 ystart, Uint8 alpha)
{
  if (nlayers + 1 <= NLAYERS && nlayers != 0)
    {
      SDL_Surface* s = SDL_CreateRGBSurface(screen->flags, width, height, screen->format->BitsPerPixel, 
					    screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, 
					    screen->format->Amask);
 
      LAYER *l = (LAYER*)malloc(sizeof(LAYER));
      l->surface = SDL_ConvertSurface(s, screen->format, screen->flags);

      /* set palette if any */
      if (l->surface->format->palette)
	{
	  SDL_SetPalette(l->surface, SDL_LOGPAL | SDL_PHYSPAL, l->surface->format->palette->colors, 0, 
			 l->surface->format->palette->ncolors);
	}

      /* set transparant pixel which is black */
      SDL_SetColorKey(l->surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGBA(l->surface->format, 0, 0, 0, 0xFF)); 

      /* set surface alpha value if appropriate*/
      l->alpha = alpha;
      if (alpha != 0xFF)
	{
	  SDL_SetAlpha(l->surface, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
	}
      
      /* set surface dimensions and position */
      l->r.x = xstart;
      l->r.y = ystart;
      l->r.w = width;
      l->r.h = height;
      layers[nlayers++] = l;

      printf("Added layer %i\n", nlayers - 1);

      SDL_FreeSurface(s);
    }
  else if (nlayers == 0)
    {
      printf("error, layering system not init, call TDEC_init_layering first please\n");
    }
  return nlayers - 1;
}

void TDEC_draw_layers()
{
  SDL_Flip(screen);
}

SDL_Surface* TDEC_get_layer(Uint32 index)
{
  /* 0 is the background, is TDEC_BACKGROUND_LAYER and is screen */

  if (index < nlayers)
    {
      return (layers[index])->surface;
    }
}

void TDEC_flatten_layers()
{
  if (nlayers > 1)
    {
      Uint32 i;
      for (i = 1; i < nlayers; ++i)
	{
	  LAYER *l = layers[i];
	  SDL_BlitSurface(l->surface, 0, screen, &l->r);
	}
    }
}

void TDEC_clear_layer(Uint32 index)
{
  if (index < nlayers)
    {
      SDL_FillRect(layers[index]->surface, 0, SDL_MapRGB(layers[index]->surface->format, 0, 0, 0));
    }
}
