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

#ifndef LAYERS_H
#define LAYERS_H

#include "SDL/SDL.h"

#define NLAYERS 10
#define TDEC_BACKGROUND_LAYER 0

extern unsigned int TDEC_add_layer(Uint16 width, Uint16 height, Uint16 xstart, Uint16 ystart, Uint8 alpha);
extern void TDEC_draw_layers();
extern void TDEC_init_layering();
extern void TDEC_reset_layering();
extern SDL_Surface* TDEC_get_layer(Uint32 index);
extern void TDEC_flatten_layers();
extern void TDEC_clear_layer(Uint32 index);

#endif