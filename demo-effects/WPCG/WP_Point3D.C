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

#include "WP_Point3D.h"
#include "WP_Matrix3D.h"
#include "WP_Vector3D.h"

/* COPY CONSTRUCTOR */
WP_Point3D& WP_Point3D::operator=(const WP_Point3D& p)
{
	data[0] = p.data[0];
	data[1] = p.data[1];
	data[2] = p.data[2];
	return *this;
}

WP_Point3D& WP_Point3D::operator*=(const WP_Matrix3D& m)
{
	WP_Point3D copy = *this;
	data[0] = m.data[0] * copy.data[0] + m.data[4] * copy.data[1] + m.data[8] * copy.data[2] + m.data[12];
	data[1] = m.data[1] * copy.data[0] + m.data[5] * copy.data[1] + m.data[9] * copy.data[2] + m.data[13];
	data[2] = m.data[2] * copy.data[0] + m.data[6] * copy.data[1] + m.data[10] * copy.data[2] + m.data[14];

	return *this;
}

WP_Point3D& WP_Point3D::operator+=(const WP_Vector3D& v)
{
  data[0] += v.data[0]; 
  data[1] += v.data[1];
  data[2] += v.data[2];
  return *this;
}

WP_Point3D& WP_Point3D::operator-=(const WP_Vector3D& v)
{
  data[0] -= v.data[0]; 
  data[1] -= v.data[1];
  data[2] -= v.data[2];
  return *this;
}

WP_Vector3D WP_Point3D::operator+(const WP_Point3D& p) const
{
	return WP_Vector3D(data[0] + p.data[0], data[1] + p.data[1], data[2] + p.data[2]);
}

WP_Point3D WP_Point3D::operator+(const WP_Vector3D& v) const
{
	return WP_Point3D(data[0] + v.data[0], data[1] + v.data[1], data[2] + v.data[2]);
}

WP_Vector3D WP_Point3D::operator-(const WP_Point3D& p) const
{
	return WP_Vector3D(data[0] - p.data[0], data[1] - p.data[1], data[2] - p.data[2]);
}

WP_Point3D WP_Point3D::operator-(const WP_Vector3D& v) const
{
	return WP_Point3D(data[0] - v.data[0], data[1] - v.data[1], data[2] - v.data[2]);
}

void WP_Point3D::draw() const
{
	glBegin(GL_POINTS);
		glVertex3fv(data);
	glEnd();
}

void WP_Point3D::set(const WP_Vector3D& v)
{
	data[0] = v.data[0]; 
	data[1] = v.data[1]; 
	data[2] = v.data[2];
}

WP_Vector3D WP_Point3D::toVector() const
{
	return WP_Vector3D(data[0], data[1], data[2]);
}

