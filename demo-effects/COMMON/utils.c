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

  res = SDL_DisplayFormat(surface);

  if (res->format->palette)
    SDL_SetPalette(res, SDL_LOGPAL | SDL_PHYSPAL, surface->format->palette->colors, 0, surface->format->palette->ncolors);

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
  
  res = TDEC_fadein_palette(d,ps,rate);
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

void TDEC_scalex_copy_image(SDL_Surface* original, SDL_Surface* copy, Uint8 percentage)
{
  float skip_scanlines, rest;
  Uint16 j;
  int i;
  SDL_Rect o = {0, 0, 1, original->h};
  SDL_Rect c = {0, 0, 1, original->h};

  if (original->w != copy->w)
    {
      printf("TDEC_scalex_image: original and copy image are not of the same width \n");
      return;
    }
  
  SDL_FillRect(copy, 0, SDL_MapRGB(copy->format, 0, 0, 0));

  if (percentage == 0)
    return;

  if (percentage >= 100)
    {
      SDL_BlitSurface(original, 0, copy, 0);
      return;
    }

  skip_scanlines = percentage / 100.0;

  /* copy scanlines left of middle scanline */

  j = original->w >> 1;
  rest = 0.0;
  o.x = j;
  c.x = j;

  for (i = j; i >= 0; --i)
    {
      if (rest >= 1.0)
	{
	  SDL_BlitSurface(original, &o, copy, &c);
  	  c.x = --j;
	  rest -= 1.0;	 
	}
      rest += skip_scanlines;
      --o.x;
    }
  
  /* copy scanlines right of middle scanline */

  j = (original->w >> 1) + 1;
  rest = 0.0;
  o.x = j;
  c.x = j;

  for (i = j; i < original->w; ++i)
    {
      if (rest >= 1.0)
	{
	  SDL_BlitSurface(original, &o, copy, &c);
	  c.x = ++j;
	  rest -= 1.0;	  
	}
      
      rest += skip_scanlines;
      ++o.x;
    }
}

void TDEC_scaley_copy_image(SDL_Surface* original, SDL_Surface* copy, Uint8 percentage)
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
      SDL_BlitSurface(original, 0, copy, 0);
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

void TDEC_scale_copy_image(SDL_Surface* original, SDL_Surface* copy, Uint8 percentage)
{
  SDL_Surface* temp = TDEC_copy_surface(copy);
  TDEC_scalex_copy_image(original, temp, percentage);
  TDEC_scaley_copy_image(temp, copy, percentage);
  SDL_FreeSurface(temp);
}

void TDEC_scalex_image(SDL_Surface* surface, Uint8 percentage)
{
  SDL_Surface* s;

  s = TDEC_copy_surface(surface);
  TDEC_scalex_copy_image(surface, s, percentage);

  SDL_BlitSurface(s, 0, surface, 0);

  SDL_FreeSurface(s);
}

void TDEC_scaley_image(SDL_Surface* surface, Uint8 percentage)
{
  SDL_Surface* s;

  s = TDEC_copy_surface(surface);
  TDEC_scaley_copy_image(surface, s, percentage);

  SDL_BlitSurface(s, 0, surface, 0);

  SDL_FreeSurface(s);
}

void TDEC_scale_image(SDL_Surface* surface, Uint8 percentage)
{
  TDEC_scalex_image(surface, percentage);
  TDEC_scaley_image(surface, percentage);
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

  s = TDEC_copy_surface(surface);
  TDEC_flipx_copy_image(surface, s);

  SDL_BlitSurface(s, 0, surface, 0);

  SDL_FreeSurface(s);
}

void TDEC_flipy_copy_image(SDL_Surface* original, SDL_Surface* copy)
{
  Uint16 i;
  SDL_Rect o = {0, 0, 1, original->h};
  SDL_Rect c = {0, 0, 1, copy->h};

  if (original->w != copy->w)
    {
      printf("TDEC_flipy_image: original and copy image are not of the same width \n");
      return;
    }

  for (i = 0; i < original->w; ++i)
    {
      o.x = i;
      c.x = original->w - 1 - i;
      SDL_BlitSurface(original, &o, copy, &c);
    }
}

void TDEC_flipy_image(SDL_Surface* surface)
{
  SDL_Surface* s;

  s = TDEC_copy_surface(surface);
  TDEC_flipy_copy_image(surface, s);
  
  SDL_BlitSurface(s, 0, surface, 0);

  SDL_FreeSurface(s);
}
