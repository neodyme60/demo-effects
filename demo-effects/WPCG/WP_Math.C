/* Copyright (C) 2001 W.P. van Paassen - peter@paassen.tmfweb.nl

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


#include <cmath>
#include "WP_Math.h"

WP_Math* WP_Math::_instance = 0;

WP_Math::WP_Math()
{
  scalar rad;
  for (int i = 0; i < 360; i++)
    {
      rad = degreeToRad(i);
      aSin[i] = sin(rad);
      aCos[i] = cos(rad);
    }
}

scalar WP_Math::round(scalar s) const
{
  scalar f = (scalar)floor(s);
  if ((s - 0.5) < f)
    return f;
  return (scalar)ceil(s);
}

scalar WP_Math::degreeToRad(int angle) const
{
  return ((scalar)angle) * DEGRAD;
}

scalar WP_Math::fDegreeToRad(scalar angle) const
{
  return angle * DEGRAD;
}
	
int WP_Math::radToDegree(scalar rad) const
{
  int result = (int)(round(rad * RADDEG));
  
  if (result < 0)
    {
      result %= 360;
      return result + 360;
    }
  
  if (result >= 360)
    {
      result %= 360;
      return result - 360;
    }

  return result % 360;
}

scalar WP_Math::fRadToDegree(scalar rad) const
{
  return rad * RADDEG;
}		

