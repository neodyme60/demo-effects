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

#ifndef JUMPING_SCROLLER_H
#define JUMPING_SCROLLER_H

#include "SDL/SDL.h"

extern void TDEC_init_jumping_scroller(SDL_Surface *s, char *_text, char *font, char *_characters,
				Uint8 character_width, Uint8 character_height,
				Uint16 _width, Uint16 _height);

extern void TDEC_draw_jumping_scroller(void);
extern void TDEC_free_jumping_scroller(void);

#endif
