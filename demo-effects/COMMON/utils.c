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

#include "utils.h"

SDL_Surface* TDEC_copy_surface(SDL_Surface* surface)
{
  SDL_Surface* res;
  SDL_Surface* s = SDL_CreateRGBSurface(surface->flags, surface->w, surface->h, (surface->pitch / surface->w)  << 3, 
					rmask, gmask, bmask, amask);
  if (!s)
    return 0;

  res = SDL_DisplayFormat(s);
  SDL_FreeSurface(s);
  return res;
}

SDL_Surface* TDEC_copy_image(SDL_Surface* surface)
{
  Uint32 i;
  SDL_Surface* res = SDL_CreateRGBSurface(surface->flags, surface->w, surface->h, (surface->pitch / surface->w)  << 3, 
					rmask, gmask, bmask, amask);

  SDL_SetPalette(res, SDL_LOGPAL | SDL_PHYSPAL, surface->format->palette->colors, 0, surface->format->palette->ncolors);

  SDL_LockSurface(res);
  
  /* copy pixels */

  for (i = 0; i < surface->format->BytesPerPixel * surface->w * surface->h; ++i)
    {
      *((Uint8*)res->pixels + i) = *((Uint8*)surface->pixels + i);
    }
  
  SDL_UnlockSurface(res);
  return res;
}

Uint8 TDEC_fadeout_palette(SDL_Palette* palette, Uint8 rate)
{
  Uint16 i;
  Uint16 faded = 0;

  for (i = 0; i < palette->ncolors; ++i)
    {
      SDL_Color* c = palette->colors + i;

      if (c->r > 0)
	{  
	  if (c->r > rate)
	    {
	      c->r -= rate;
	      faded++;
	    }
	  else
	    c->r = 0;
	}

      if (c->g > 0)
	{  
	  if (c->g > rate)
	    {
	      c->g -= rate;
	      faded++;
	    }
	  else
	    c->g = 0;
	}

      if (c->b > 0)
	{  
	  if (c->b > rate)
	    {
	      c->b -= rate;
	      faded++;
	    }
	  else
	    c->b = 0;
	}
    }

  return faded == 0;
}

Uint8 TDEC_fadeout( SDL_Surface* s, Uint8 rate )
{
  Uint8 res;
  SDL_Palette* p = s->format->palette;
  
  res = TDEC_fadeout_palette(p, rate);
  SDL_SetPalette(s, SDL_PHYSPAL, p->colors, 0, p->ncolors);
  return res;
}

Uint8 TDEC_fadein_palette(SDL_Palette* source, SDL_Palette* dest, Uint8 rate)
{
  Uint16 i;
  Uint16 full = 0;

  for (i = 0; i < source->ncolors && i < dest->ncolors; ++i)
    {
      SDL_Color* cs = source->colors + i;
      SDL_Color* cd = dest->colors + i;

      if (cd->r < cs->r)
	{  
	  if (cd->r < cs->r - rate)
	    {
	      cd->r += rate;
	      full++;
	    }
	  else
	    cd->r = cs->r;
	}

      if (cd->g < cs->g)
	{  
	  if (cd->g < cs->g - rate)
	    {
	      cd->g += rate;
	      full++;
	    }
	  else
	    cd->g = cs->g;
	}

      if (cd->b < cs->b)
	{  
	  if (cd->b < cs->b - rate)
	    {
	      cd->b += rate;
	      full++;
	    }
	  else
	    cd->b = cs->b;
	}
     }

  return full == 0;
}

Uint8 TDEC_fadein( SDL_Surface* s, SDL_Palette* d, Uint8 rate )
{
  Uint8 res;
  SDL_Palette* ps = s->format->palette;
  
  res = TDEC_fadein_palette(d, ps, rate);
  SDL_SetPalette(s, SDL_LOGPAL | SDL_PHYSPAL, ps->colors, 0, ps->ncolors);
  return res;
}

void TDEC_blacken_palette(SDL_Surface* s)
{
  Uint16 i;
  SDL_Palette* p = s->format->palette;

  for (i = 0; i < p->ncolors; ++i)
    {
      SDL_Color* c = p->colors + i;
      c->r = 0;
      c->g = 0;
      c->b = 0;
    }

  SDL_SetPalette(s, SDL_PHYSPAL | SDL_LOGPAL, p->colors, 0, p->ncolors);
}

void TDEC_scaley_image(SDL_Surface* original, SDL_Surface* copy, Uint8 percentage)
{
  float skip_scanlines, rest;
  Uint16 j;
  int i;
  SDL_Rect o = {0, 0, original->w, 1};
  SDL_Rect c = {0, 0, copy->w, 1};

  if (original->h != copy->h)
    {
      printf("TDEC_scaley_image: original and copy image are not of the same height \n");
      return;
    }
  
  SDL_FillRect(copy, 0, SDL_MapRGB(copy->format, 0, 0, 0));

  if (percentage == 0)
    return;

  if (percentage >= 100)
    {
      for (j = 0; j < original->h; ++j)
	{
	  SDL_BlitSurface(original, &o, copy, &c);
	  ++o.y;
	  ++c.y;
	}
      return;
    }

  skip_scanlines = percentage / 100.0;

  /* copy scanlines above middle scanline */

  j = original->h >> 1;
  rest = 0.0;
  o.y = j;
  c.y = j;

  for (i = j; i >= 0; --i)
    {
      if (rest >= 1.0)
	{
	  SDL_BlitSurface(original, &o, copy, &c);
  	  c.y = --j;
	  rest -= 1.0;	 
	}
      rest += skip_scanlines;
      --o.y;
    }
  
  /* copy scanlines below middle scanline */

  j = (original->h >> 1) + 1;
  rest = 0.0;
  o.y = j;
  c.y = j;

  for (i = j; i < original->h; ++i)
    {
      if (rest >= 1.0)
	{
	  SDL_BlitSurface(original, &o, copy, &c);
	  c.y = ++j;
	  rest -= 1.0;	  
	}
      
      rest += skip_scanlines;
      ++o.y;
    }
}

void TDEC_flipx_copy_image(SDL_Surface* original, SDL_Surface* copy)
{
  Uint16 i;
  SDL_Rect o = {0, 0, original->w, 1};
  SDL_Rect c = {0, 0, copy->w, 1};

  if (original->h != copy->h)
    {
      printf("TDEC_flipx_image: original and copy image are not of the same height \n");
      return;
    }

  for (i = 0; i < original->h; ++i)
    {
      o.y = i;
      c.y = original->h - 1 - i;
      SDL_BlitSurface(original, &o, copy, &c);
    }
}

void TDEC_flipx_image(SDL_Surface* surface)
{
  SDL_Surface* s;
  Uint32 i;

  s = TDEC_copy_image(surface);
  TDEC_flipx_copy_image(surface, s);

  SDL_LockSurface(surface);
  
  /* copy pixels */

  for (i = 0; i < s->format->BytesPerPixel * s->w * s->h; ++i)
    {
      *((Uint8*)surface->pixels + i) = *((Uint8*)s->pixels + i);
    }
  
  SDL_UnlockSurface(surface);

  SDL_FreeSurface(s);
}
