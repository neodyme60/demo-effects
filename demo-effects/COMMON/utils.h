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

#ifndef UTILS_H
#define UTILS_H

#include <SDL/SDL.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
static const Uint32    rmask = 0xff000000;
static const Uint32    gmask = 0x00ff0000;
static const Uint32    bmask = 0x0000ff00;
static const Uint32    amask = 0x000000ff;
#else
static const Uint32    rmask = 0x000000ff;
static const Uint32    gmask = 0x0000ff00;
static const Uint32    bmask = 0x00ff0000;
static const Uint32    amask = 0xff000000;
#endif

SDL_Surface* TDEC_copy_surface(SDL_Surface* surface);
SDL_Surface* TDEC_copy_image(SDL_Surface* surface);
Uint8 TDEC_fadeout_palette(SDL_Palette* palette, Uint8 rate);
Uint8 TDEC_fadeout( SDL_Surface* s, Uint8 rate);
Uint8 TDEC_fadein_palette(SDL_Palette* source, SDL_Palette* dest, Uint8 rate);
Uint8 TDEC_fadein( SDL_Surface* s, SDL_Palette* d, Uint8 rate );
void TDEC_blacken_palette(SDL_Surface* s);
void TDEC_scaley_image(SDL_Surface* original, SDL_Surface* copy, Uint8 percentage);
void TDEC_flipx_copy_image(SDL_Surface* original, SDL_Surface* copy);
void TDEC_flipx_image(SDL_Surface* surface);

#endif
