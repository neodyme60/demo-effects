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

#include <iostream>
#include <cmath>
#include "WP_Vector3D.h"
#include "WP_Matrix3D.h"
#include "WP_Math.h"

WP_Vector3D::WP_Vector3D()
{
  data[0] = data[1] = data[2] = data[3] = 0.0;
}

WP_Vector3D::WP_Vector3D(scalar x, scalar y, scalar z)
{
  data[0] = x;
  data[1] = y;
  data[2] = z;
  data[3] = 0.0;
}

WP_Vector3D::WP_Vector3D(scalar* s)
{
  data[0] = s[0];
  data[1] = s[1];
  data[2] = s[2];
  data[3] = 0.0;
}

WP_Vector3D& WP_Vector3D::operator=(const WP_Vector3D& v)
{
  if (this == &v)
    return *this;
  data[0] = v.data[0];
  data[1] = v.data[1];
  data[2] = v.data[2];
  return *this;
}

//dot product
scalar WP_Vector3D::operator*(const WP_Vector3D& v) const
{
  return data[0] * v.data[0] + data[1] * v.data[1] + data[2] * v.data[2]; 
}

WP_Vector3D& WP_Vector3D::operator-()
{
  data[0] = -data[0];
  data[1] = -data[1];
  data[2] = -data[2];
  return *this;
}

WP_Vector3D WP_Vector3D::operator-(const WP_Vector3D& v) const
{
  WP_Vector3D temp;
  temp = *this;
  temp.data[0] -= v.data[0];
  temp.data[1] -= v.data[1];
  temp.data[2] -= v.data[2];
  return temp;
}

WP_Vector3D WP_Vector3D::operator+(const WP_Vector3D& v) const
{
  WP_Vector3D temp;
  temp = *this;
  temp.data[0] += v.data[0];
  temp.data[1] += v.data[1];
  temp.data[2] += v.data[2];
  return temp;
}

WP_Vector3D WP_Vector3D::operator*(scalar s) const
{
  WP_Vector3D temp;
  temp = *this;
  temp.data[0] *= s;
  temp.data[1] *= s;
  temp.data[2] *= s;
  return temp;
}

WP_Vector3D WP_Vector3D::operator/(scalar s) const
{
  WP_Vector3D temp;
  temp = *this;
  temp.data[0] /= s;
  temp.data[1] /= s;
  temp.data[2] /= s;
  return temp;
}

WP_Vector3D& WP_Vector3D::operator*=(scalar f)
{
  data[0] *= f;
  data[1] *= f;
  data[2] *= f;
  return *this;
}

WP_Vector3D& WP_Vector3D::operator/=(scalar f)
{
  data[0] /= f;
  data[1] /= f;
  data[2] /= f;
  return *this;
}

WP_Vector3D& WP_Vector3D::operator*=(const WP_Matrix3D& m)
{
  WP_Vector3D v = *this;
  
  data[0] = m.data[0] * v.data[0] + m.data[4] * v.data[1] + m.data[8] * v.data[2];
  data[1] = m.data[1] * v.data[0] + m.data[5] * v.data[1] + m.data[9] * v.data[2];
  data[2] = m.data[2] * v.data[0] + m.data[6] * v.data[1] + m.data[10] * v.data[2];
  
  return *this;
}

WP_Vector3D WP_Vector3D::operator*(const WP_Matrix3D& m) const
{
  WP_Vector3D temp = *this;
  temp *= m;
  return temp;
}

WP_Vector3D& WP_Vector3D::operator -=(const WP_Vector3D& v)
{
  data[0] = data[0] - v.data[0];
  data[1] = data[1] - v.data[1];
  data[2] = data[2] - v.data[2];
  return *this;
}

WP_Vector3D& WP_Vector3D::operator +=(const WP_Vector3D& v)
{
  data[0] = data[0] + v.data[0];
  data[1] = data[1] + v.data[1];
  data[2] = data[2] + v.data[2];
  return *this;
}

bool WP_Vector3D::crossProduct(const WP_Vector3D& v)
{
  scalar degrees = getDegreesBetween(v);
  if ((degrees > -0.00001 && degrees < 0.00001) || (degrees > 179.99999 && degrees < 180.00001))
    {
      return false;//vectors have an angle of 0 degrees between them, so crossproduct is not possible
    }

  *this = WP_Vector3D(data[1] * v.data[2] - data[2] * v.data[1], data[2] * v.data[0] - data[0] * v.data[2], 
		      data[0] * v.data[1] - data[1] * v.data[0]);
  return true;
}

bool WP_Vector3D::orthogonal(const WP_Vector3D& v) const
{
  scalar dot = *this * v;
  return dot <= 0.00001 && dot >= -0.00001; 
}

scalar WP_Vector3D::length() const
{
  return sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
}

scalar WP_Vector3D::normalize()
{
  scalar l = length();

  data[0] /= l;
  data[1] /= l;
  data[2] /= l;

  return l;
}

scalar WP_Vector3D::getDegreesBetween(const WP_Vector3D& v) const
{
  WP_Math* m = WP_Math::getInstance();
  
  scalar dot = *this * v;
  dot /= length() * v.length();
  return m->fRadToDegree((scalar)acos(dot));
}

scalar WP_Vector3D::getRadsBetween(const WP_Vector3D& v) const
{
  scalar dot = *this * v;
  dot /= length() * v.length();
  return (scalar)acos(dot);
}

void WP_Vector3D::print() const
{
  cout << data[0] << endl << data[1] << endl << data[2] << endl << data[3] << endl;
}
