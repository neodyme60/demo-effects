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

  res = SDL_ConvertSurface(surface, surface->format, surface->flags);

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
  float skip_columns, rest;
  Uint16 i,j;
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

  skip_columns = percentage / 100.0;

  /* copy scanlines left of middle column */

  j = (original->w >> 1) - 1;
  rest = skip_columns;
  o.x = j;
  c.x = j;

  for (i = j + 1; i > 0; --i)
    {
      if (rest >= 1.0)
	{
	  SDL_BlitSurface(original, &o, copy, &c);
  	  c.x = --j;
	  rest -= 1.0;	 
	}
      rest += skip_columns;
      --o.x;
    }
  
  /* copy scanlines right of middle column */

  j = original->w >> 1;
  rest = skip_columns;
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
      
      rest += skip_columns;
      ++o.x;
    }
}

void TDEC_scaley_copy_image(SDL_Surface* original, SDL_Surface* copy, Uint8 percentage)
{
  float skip_scanlines, rest;
  Uint16 i, j;
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

  j = (original->h >> 1) - 1;
  rest = skip_scanlines;
  o.y = j;
  c.y = j;

  for (i = j + 1; i > 0; --i)
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

  j = original->h >> 1;
  rest = skip_scanlines;
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

void TDEC_scale_copy_hscanline(SDL_Surface* original, SDL_Surface* copy, Uint16 scanline_index, Uint8 percentage)
{
  float skip_pixels, rest;
  Uint16 i, j, l;
  short k;
  Uint32 index = scanline_index * original->pitch;
  Uint8* original_pixel = (Uint8*)original->pixels + index;
  Uint8* copy_pixel = (Uint8*)copy->pixels + index;
  Uint8* o = original_pixel;
  Uint8* c = copy_pixel;

  if (original->w != copy->w)
    {
      printf("TDEC_scale_copy_hscanline: original and copy image are not of the same width \n");
      return;
    }
  else if (scanline_index >= original->h)
    {
      printf("TDEC_scaley_copy_hscanline: invalid scanline_index \n");
      return;
    }
  
  if (percentage == 0)
    {
      SDL_Rect c = {0, scanline_index, copy->w, 1};
      SDL_FillRect(copy, &c, SDL_MapRGB(copy->format, 0,0,0));
      return;
    }

  if (percentage >= 100)
    {
      SDL_Rect o = {0, scanline_index, original->w, 1};
      SDL_Rect c = {0, scanline_index, copy->w, 1};
      
      SDL_BlitSurface(original, &o, copy, &c);
      
      return;
    }

  SDL_LockSurface(copy);

  skip_pixels = percentage / 100.0;

  j = (original->w >> 1) - 1;
  
  index = j * original->format->BytesPerPixel;
  original_pixel += index;
  copy_pixel += index;

  rest = skip_pixels;

  /* copy pixels left from middle pixel */

  for (i = (original->w >> 1); i > 0; --i)
    {
      if (rest >= 1.0)
	{
	  for (k = original->format->BytesPerPixel - 1; k >= 0; --k)
	      *(copy_pixel + k) = *(original_pixel + k);
	  
	  copy_pixel -=  copy->format->BytesPerPixel;
	  rest -= 1.0;
	}
      
      original_pixel -=  original->format->BytesPerPixel;
      rest += skip_pixels;
    }
  
  /* copy pixels right from middle pixel */

  index = j * original->format->BytesPerPixel;
  original_pixel = o + index;
  copy_pixel = c + index;
  
  rest = skip_pixels;

  for (i = original->w >> 1; i < original->w; ++i)
    {
      if (rest >= 1.0)
	{
	  for (l = 0; l < original->format->BytesPerPixel; ++l)
	    *(copy_pixel + l) = *(original_pixel + l);

	  copy_pixel +=  copy->format->BytesPerPixel;	  
	  rest -= 1.0;	  
	}

      original_pixel +=  original->format->BytesPerPixel;
      rest += skip_pixels;
    }

  SDL_UnlockSurface(copy);
}

void TDEC_scale_hscanline(SDL_Surface* surface, Uint16 scanline_index, Uint8 percentage)
{
  SDL_Surface* s;
  SDL_Rect o = {0, scanline_index, surface->w, 1};

  s = TDEC_copy_surface(surface);
  TDEC_scale_copy_hscanline(surface, s, scanline_index, percentage);

  SDL_BlitSurface(s, &o, surface, &o);

  SDL_FreeSurface(s);
}

void TDEC_scale_copy_vscanline(SDL_Surface* original, SDL_Surface* copy, Uint16 scanline_index, Uint8 percentage)
{
  float skip_pixels, rest;
  Uint16 i, j, l;
  short k;
  Uint32 index;
  Uint8* original_pixel = (Uint8*)original->pixels + (scanline_index * original->format->BytesPerPixel);
  Uint8* copy_pixel = (Uint8*)copy->pixels + (scanline_index * copy->format->BytesPerPixel);
  Uint8* o = original_pixel;
  Uint8* c = copy_pixel;

  if (original->h != copy->h)
    {
      printf("TDEC_scale_copy_vscanline: original and copy image are not of the same height \n");
      return;
    }
  else if (scanline_index >= original->w)
    {
      printf("TDEC_scaley_copy_vscanline: invalid scanline_index: %i \n", scanline_index);
      return;
    }
  
  if (percentage == 0)
    {
      SDL_Rect c = {scanline_index, 0, 1, copy->h};
      SDL_FillRect(copy, &c, SDL_MapRGB(copy->format, 0,0,0));
      return;
    }

  if (percentage >= 100)
    {
      SDL_Rect o = {scanline_index, 0, 1, original->h};
      SDL_Rect c = {scanline_index, 0, 1, copy->h};
      
      SDL_BlitSurface(original, &o, copy, &c);
      
      return;
    }

  SDL_LockSurface(copy);

  skip_pixels = percentage / 100.0;

  j = ((original->h >> 1) - 1) * original->pitch;
  
  original_pixel += j;
  copy_pixel += j;
  rest = skip_pixels;

  /* copy pixels above the middle pixel */

  for (i = original->h >> 1; i > 0; --i)
    {
      if (rest >= 1.0)
	{
	  for (k = original->format->BytesPerPixel - 1; k >= 0; --k)
	      *(copy_pixel + k) = *(original_pixel + k);
	  
	  copy_pixel -= copy->pitch;
	  rest -= 1.0;
	}
      
      original_pixel -= original->pitch;
      rest += skip_pixels;
    }
  
  /* copy pixels below of the middle pixel */

  j += original->format->BytesPerPixel;
  original_pixel = o + j;
  copy_pixel = c + j;
  rest = skip_pixels;

  for (i = original->h >> 1; i < original->h; ++i)
    {
      if (rest >= 1.0)
	{
	  for (l = 0; l < original->format->BytesPerPixel; ++l)
	    *(copy_pixel + l) = *(original_pixel + l);

	  copy_pixel += copy->pitch;	  
	  rest -= 1.0;	  
	}

      original_pixel += original->pitch;
      rest += skip_pixels;
    }

  SDL_UnlockSurface(copy);
}

void TDEC_scale_vscanline(SDL_Surface* surface, Uint16 scanline_index, Uint8 percentage)
{
  SDL_Surface* s;
  SDL_Rect o = {scanline_index, 0, 1, surface->h};

  s = TDEC_copy_surface(surface);
  TDEC_scale_copy_vscanline(surface, s, scanline_index, percentage);

  SDL_BlitSurface(s, &o, surface, &o);

  SDL_FreeSurface(s);
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

SDL_Surface* TDEC_create_heightmap(SDL_Surface *source)
{
  Uint16 i, j;
  Uint32 x; 
  Uint8 index;
  SDL_Palette *p;
  SDL_Surface *s;
  Uint8 *image;

  if (!source->format->palette) /* not an 8-bit source surface */
    return (SDL_Surface*)0;

  s = TDEC_copy_surface(source);

  p = s->format->palette;

  /* sort palette from dark to light colors using selection sort */

  for (i = 0; i < 256; ++i)
    {
      SDL_Color *c = p->colors + i;
      index = i;

      for (j = i + 1; j < 256; ++j)
	{
	  SDL_Color *d = p->colors + j;
	  Uint8 biggest = 0;

	  /* what is the biggest value? */
	  if (c->r >= c->g && c->r >= c->b)
	    {
	      /* r is biggest */
	      biggest = c->r;
	    }
	  else if (c->g >= c->r && c->g >= c->b)
	    {
	      /* g is biggest */
	      biggest = c->g;
	    }
	  else if (c->b >= c->r && c->b >= c->g)
	    {
	      /* b is biggest */
	      biggest = c->b;
	    }

	  if (d->r <= biggest && d->g <= biggest && d->b <= biggest)
	    {
	      if (d->r == biggest || d->g == biggest || d->b == biggest)
		{
		  if (d->r + d->g + d->b < c->r + c->g + c->b)
		    {
		      /* found darker color */
		      index = j;
		      c = d;
		    }
		}
	      else
		{
		  /* found darker color */
		  index = j;
		  c = d;
		}
	    }
	}

      image = (Uint8*)s->pixels;
      p->colors[index] = p->colors[i];
      p->colors[i].r = p->colors[i].g = p->colors[i].b = i;
      
      /* swap pixels in image */
      
      for (x = 0; x < s->w * s->h; ++x)
	{
	  if (*image == index)
	    *image = i;
	  image++;
	}
    }

  return s;
}

SDL_Surface* TDEC_create_blackandwhite(SDL_Surface* source)
{
  SDL_Color colors[256];
  Uint16 i, j; 
  Uint8 index;
  SDL_Palette *p = source->format->palette;
  SDL_Surface *s;

  if (!p) /* not an 8-bit source surface */
    return (SDL_Surface*)0;

  /* copy palette */

  for (i = 0; i < 256; ++i)
      colors[i] = *(p->colors + i);

  /* sort palette from dark to light colors using selection sort */

  for (i = 0; i < 256; ++i)
    {
      SDL_Color *c = colors + i;
      SDL_Color temp;
      index = i;

      for (j = i + 1; j < 256; ++j)
	{
	  SDL_Color *d = colors + j;
	  Uint8 biggest = 0;

	  /* what is the biggest value? */
	  if (c->r >= c->g && c->r >= c->b)
	    {
	      /* r is biggest */
	      biggest = c->r;
	    }
	  else if (c->g >= c->r && c->g >= c->b)
	    {
	      /* g is biggest */
	      biggest = c->g;
	    }
	  else if (c->b >= c->r && c->b >= c->g)
	    {
	      /* b is biggest */
	      biggest = c->b;
	    }

	  if (d->r <= biggest && d->g <= biggest && d->b <= biggest)
	    {
	      if (d->r == biggest || d->g == biggest || d->b == biggest)
		{
		  if (d->r + d->g + d->b < c->r + c->g + c->b)
		    {
		      /* found darker color */
		      index = j;
		      c = d;
		    }
		}
	      else
		{
		  /* found darker color */
		  index = j;
		  c = d;
		}
	    }
	}
      if (index > i)
	{
	  temp = colors[i];
	  colors[i] = colors[index];
	  colors[index] = temp;
	}
    }

  /* copy image and set black to white palette */

  s = TDEC_copy_surface(source);

  p = s->format->palette;

  for (i = 0; i < 256; ++i)
    {
      for (j = 0; j < 256; ++j)
	{
	  SDL_Color* c = (p->colors + j);
	  if (colors[i].r == c->r && colors[i].g == c->g && colors[i].b == c->b)
	    {
	      c->r = i;
	      c->g = i;
	      c->b = i;
	      break;
	    }
	}
    }

  return s;
}

void TDEC_mozaiek_surface( SDL_Surface* s, Uint16 blocksize)
{
  if (blocksize > 1 && !(blocksize % 2)) 
    TDEC_rquaddivide(s, 0, 0, s->w, s->h, blocksize);
}

void TDEC_rquaddivide(SDL_Surface *s, Uint16 startx, Uint16 starty, Uint16 width, Uint16 height, Uint16 size)
{
  if (width <= size || height <= size)
    {
      Uint16 i, j;
      Uint32 tempx, tempy;
      Uint32 tempmax = s->h * s->pitch ;
      Uint8 color = *((Uint8*)s->pixels + starty * s->pitch + startx + (width >> 1) + (width >> 1) * s->pitch);
      
      SDL_LockSurface(s);
      for (i = 0; i < width; ++i)
	{
	  tempx = startx + i;
	  for (j = 0; j < height; ++j)
	    {
	      tempy = starty * s->pitch + j * s->pitch;
	      if (tempx + tempy < tempmax)
		*((Uint8*)s->pixels + tempx + tempy) = color;
	      else
		break;
	    }
	}
      SDL_UnlockSurface(s);
    }
  else
    {
      if (width % 2)
	width++;
      width >>= 1;
      if (height % 2)
	height++;
      height >>= 1;
      TDEC_rquaddivide(s, startx, starty, width, height, size);    
      TDEC_rquaddivide(s, startx + width, starty, width, height, size);
      TDEC_rquaddivide(s, startx, starty + height, width, height, size);
      TDEC_rquaddivide(s, startx + width, starty + height, width, height, size);
    }
}
