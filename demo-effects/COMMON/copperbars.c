/* Copyright (C) 2002-2003 W.P. van Paassen - peter@paassen.tmfweb.nl

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

#include <math.h>
#include "copperbars.h"

static SDL_Color copper[46];
static Uint16 aSin[360];
static SDL_Surface *surface;
static Uint16 red = 96, red2 = 0, red3 = 88, red4 = 0, red5 = 80, red6 = 0, red7 = 72, red8 = 0, 
  white = 64, white2 = 0, white3 = 56, white4 = 0, white5 = 48, white6 = 0, white7 = 40, white8 = 0, 
  blue = 32, blue2 = 0, blue3 = 24, blue4 = 0, blue5 = 16, blue6 = 0, blue7 = 8, blue8 = 0;

void TDEC_init_copperbars(SDL_Surface *s, Uint16 sine_amplitude)
{
  int i, centery;
  float rad;

  surface = s;
  centery = surface->h >> 1;

  /*create sin lookup table */
  for (i = 0; i < 360; i++)
    {
      rad =  (float)i * 0.0174532; 
      aSin[i] = centery + (sin(rad) * (float)sine_amplitude);
    }

  /* red copper bar */

  copper[1].r = 0x22;
  copper[2].r = 0x44;
  copper[3].r = 0x66;
  copper[4].r = 0x88;
  copper[5].r = 0xaa;
  copper[6].r = 0xcc;
  copper[7].r = 0xee;
  copper[8].r = 0xff;
  copper[9].r = 0xee;
  copper[10].r = 0xcc;
  copper[11].r = 0xaa;
  copper[12].r = 0x88;
  copper[13].r = 0x66;
  copper[14].r = 0x44;
  copper[15].r = 0x22;

 /* white copper bar */

  copper[16].r = 0x22;
  copper[16].g = copper[16].r;
  copper[16].b = copper[16].g;
  copper[17].r = 0x44;
  copper[17].g = copper[17].r;
  copper[17].b = copper[17].g;  
  copper[18].r = 0x66;
  copper[18].g = copper[18].r;
  copper[18].b = copper[18].g;
  copper[19].r = 0x88;
  copper[19].g = copper[19].r;
  copper[19].b = copper[19].g;
  copper[20].r = 0xaa;
  copper[20].g = copper[20].r;
  copper[20].b = copper[20].g;
  copper[21].r = 0xcc;
  copper[21].g = copper[21].r;
  copper[21].b = copper[21].g;
  copper[22].r = 0xee;
  copper[22].g = copper[22].r;
  copper[22].b = copper[22].g;
  copper[23].r = 0xff;
  copper[23].g = copper[23].r;
  copper[23].b = copper[23].g;
  copper[24].r = 0xee;
  copper[24].g = copper[24].r;
  copper[24].b = copper[24].g;
  copper[25].r = 0xcc;
  copper[25].g = copper[25].r;
  copper[25].b = copper[25].g;
  copper[26].r = 0xaa;
  copper[26].g = copper[26].r;
  copper[26].b = copper[26].g;
  copper[27].r = 0x88;
  copper[27].g = copper[27].r;
  copper[27].b = copper[27].g;
  copper[28].r = 0x66;
  copper[28].g = copper[28].r;
  copper[28].b = copper[28].g;
  copper[29].r = 0x44;
  copper[29].g = copper[29].r;
  copper[29].b = copper[29].g;
  copper[30].r = 0x22;
  copper[30].g = copper[30].r;
  copper[30].b = copper[30].g;

 /* blue copper bar */

  copper[31].b = 0x22;
  copper[32].b = 0x44;
  copper[33].b = 0x66;
  copper[34].b = 0x88;
  copper[35].b = 0xaa;
  copper[36].b = 0xcc;
  copper[37].b = 0xee;
  copper[38].b = 0xff;
  copper[39].b = 0xee;
  copper[40].b = 0xcc;
  copper[41].b = 0xaa;
  copper[42].b = 0x88;
  copper[43].b = 0x66;
  copper[44].b = 0x44;
  copper[45].b = 0x22;
  
  if (surface->format->palette)
    {
      SDL_SetPalette(surface, SDL_LOGPAL | SDL_PHYSPAL, copper, 0, 46); 
    }
}

static void TDEC_draw_copper(SDL_Rect* r, Uint8 add)
{
  Uint8 i;
  
  for (i = 0; i < 15; ++i)
    {
      SDL_FillRect(surface, r, SDL_MapRGB(surface->format, copper[i + add].r, copper[i + add].g, copper[i + add].b));
      r->y++;
    }
}

void TDEC_draw_copperbars(void)
{
  SDL_Rect drect, crect;

  drect.x = 0;
  drect.w = surface->w;
  drect.h = 1;
  
  crect.x = 0;
  crect.w = surface->h;
  crect.h = 15;

  SDL_FillRect(surface, 0, SDL_MapRGB(surface->format, 0, 0, 0));
  
  /*draw copperbars back to front*/
  
  drect.y = aSin[blue7];
  blue8 = drect.y;
  blue7 += 2;
  blue7 %= 360;
  
  TDEC_draw_copper(&drect, 31);
  
  drect.y = aSin[blue5];
  blue6 = drect.y;
  blue5 += 2;
  blue5 %= 360;
  
  TDEC_draw_copper(&drect, 31);
  
  drect.y = aSin[blue3];
  blue4 = drect.y;
  blue3 += 2;
  blue3 %= 360;
  
  TDEC_draw_copper(&drect, 31);
  
  drect.y = aSin[blue];
  blue2 = drect.y;
  blue += 2;
  blue %= 360;
  
  TDEC_draw_copper(&drect, 31);
  
  drect.y = aSin[white7];
  white8 = drect.y;
  white7 += 2;
  white7 %= 360;
  
  TDEC_draw_copper(&drect, 16);
  
  drect.y = aSin[white5];
  white6 = drect.y;
  white5 += 2;
  white5 %= 360;
  
  TDEC_draw_copper(&drect, 16);
  
  drect.y = aSin[white3];
  white4 = drect.y;
  white3 += 2;
  white3 %= 360;
  
  TDEC_draw_copper(&drect, 16);
  
  drect.y = aSin[white];
  white2 = drect.y;
  white += 2;
  white %= 360;
  
  TDEC_draw_copper(&drect, 16);
  
  drect.y = aSin[red7];
  red8 = drect.y;
  red7 += 2;
  red7 %= 360;
  
  TDEC_draw_copper(&drect, 1);
  
  drect.y = aSin[red5];
  red6 = drect.y;
  red5 += 2;
  red5 %= 360;
  
  TDEC_draw_copper(&drect, 1);
  
  drect.y = aSin[red3];
  red4 = drect.y;
  red3 += 2;
  red3 %= 360;
  
  TDEC_draw_copper(&drect, 1);
  
  drect.y = aSin[red];
  red2 = drect.y;
  red += 2;
  red %= 360;
  
  TDEC_draw_copper(&drect, 1);
}





