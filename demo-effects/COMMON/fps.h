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

#ifndef FPS_H
#define FPS_H

#include "SDL/SDL.h"

static Uint32 Ticks = 0, Drawn_Frames = 0, next, now;
static unsigned char fps = 40;
static Uint16 Tick_Interval = 20 ;
static Uint16 Min_Tick = 20;

void TDEC_set_fps(unsigned char _fps);
void TDEC_print_fps();
void TDEC_new_time();
int TDEC_fps_ok();
void TDEC_init_timer();

#endif
