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

#include "fps.h"

void TDEC_set_fps(unsigned char _fps)
{
	fps = _fps;
	Tick_Interval = ((1000 / fps) / 10) * 10;
	Min_Tick = ((1000 / fps) / 10) * 10;
}

void TDEC_print_fps()
{
  printf("Preferred FPS was: %i\n", fps);
  printf("Real FPS was : %i\n", 1000 / (Ticks / Drawn_Frames));
}

void TDEC_new_time()
{
     next = SDL_GetTicks() + Tick_Interval;
}

int TDEC_get_fps()
{
  return 1000 / (Ticks / Drawn_Frames);
}

int TDEC_fps_ok()
{
  now = SDL_GetTicks();
  Ticks += Tick_Interval;
  
  if (now < next)
    {
      /*frame rate is met*/
      
	/*      
		Uint8 diff = next - now;
      		if (diff >= 10)
		SDL_Delay(diff);
	*/

	SDL_Delay(next - now);
    }
  else if (next != now)
    {
      /*drop frame*/
      return 0;
    }

  Drawn_Frames++;
  return 1;
}

void TDEC_init_timer()
{
  SDL_InitSubSystem(SDL_INIT_TIMER);
}

