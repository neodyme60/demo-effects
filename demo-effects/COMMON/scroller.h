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

#ifndef SCROLLER_H
#define SCROLLER_H

#include "SDL/SDL.h"

extern void TDEC_init_scroller(char *_text, char *font, char *_characters, 
			       Uint8 character_width, Uint8 character_height);

extern SDL_Rect* TDEC_get_font_char(void);
extern void TDEC_free_scroller(void);
extern Uint8 TDEC_scroller_ready(void);
extern Uint8 TDEC_get_character_width(void);
extern Uint8 TDEC_get_character_height(void);
extern void TDEC_draw_font_char(SDL_Rect *font_rect, SDL_Surface *destination, SDL_Rect *dest_rect);
static Uint16 TDEC_compute_font_pos(char scroll_char);

#endif
