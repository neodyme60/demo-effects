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

/* copying */
SDL_Surface* TDEC_copy_surface(SDL_Surface* surface);

/* palette fading */
Uint8 TDEC_fadeout( SDL_Surface* s, Uint8 rate);
Uint8 TDEC_fadein( SDL_Surface* s, SDL_Palette* d, Uint8 rate );
void TDEC_blacken_palette(SDL_Surface* s);

/* image scaling */
void TDEC_scalex_copy_image(SDL_Surface* original, SDL_Surface* copy, Uint8 percentage);
void TDEC_scaley_copy_image(SDL_Surface* original, SDL_Surface* copy, Uint8 percentage);
void TDEC_scale_copy_image(SDL_Surface* original, SDL_Surface* copy, Uint8 percentage);
void TDEC_scalex_image(SDL_Surface* surface, Uint8 percentage);
void TDEC_scaley_image(SDL_Surface* surface, Uint8 percentage);
void TDEC_scale_image(SDL_Surface* surface, Uint8 percentage);
void TDEC_scale_copy_scanline(SDL_Surface* original, SDL_Surface* copy, Uint16 scanline_index, Uint8 percentage);
void TDEC_scale_scanline(SDL_Surface* surface, Uint16 scanline_index, Uint8 percentage);

/* image flipping */
void TDEC_flipx_image(SDL_Surface* surface);
void TDEC_flipy_image(SDL_Surface* surface);
void TDEC_flipx_copy_image(SDL_Surface* original, SDL_Surface* copy);
void TDEC_flipy_copy_image(SDL_Surface* original, SDL_Surface* copy);

/* local functions */
static Uint8 TDEC_fadeout_palette(SDL_Palette* palette, Uint8 rate);
static Uint8 TDEC_fadein_palette(SDL_Palette* source, SDL_Palette* dest, Uint8 rate);

/* variables */

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
static const Uint32 r_mask = 0xFF000000; 
static const Uint32 g_mask = 0x00FF0000;
static const Uint32 b_mask = 0x0000FF00;
static const Uint32 a_mask = 0x000000FF;
#else
static const Uint32 r_mask = 0x000000FF; 
static const Uint32 g_mask = 0x0000FF00;
static const Uint32 b_mask = 0x00FF0000;
static const Uint32 a_mask = 0xFF000000;
#endif

#endif
