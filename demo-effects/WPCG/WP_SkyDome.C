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

#include <math.h>
#include <iostream.h>
#include "WP_Math.h"
#include "WP_Vertex.h"
#include "WP_GLState.h"
#include "WP_SkyDome.h"

WP_SkyDome::WP_SkyDome(scalar rad, int delta_azimuth, int delta_latitude):displayID(0)
{
  //creates a skydome

  WP_Math* math = WP_Math::getInstance();

  scalar azimuth, latitude;
  scalar d_azimuth, d_latitude;

  d_azimuth = math->degreeToRad(delta_azimuth);
  d_latitude = math->degreeToRad(delta_latitude);

  int strips = 360 / delta_azimuth;
  int stacks = 90 / delta_latitude;
  int number_vertices = strips * stacks * 4;

  vertices = new WP_Vertex[number_vertices];

  int n = 0;

   //create display list of triangle strips

  displayID = glGenLists(1);
  glNewList(displayID, GL_COMPILE);
  glPushMatrix();
  glCullFace(GL_FRONT);
  WP_GLState::getInstance()->disableLighting();
  glRotatef(-90.0, 1, 0, 0);

  glBegin(GL_TRIANGLE_STRIP);

  scalar color = 0.0;

  for (latitude = 0.0; latitude <= HALF_PI - d_latitude; latitude += d_latitude)
    {
      for (azimuth = 0.0; azimuth <= DOUBLE_PI - d_azimuth; azimuth += d_azimuth)
	{
	  double sin_latitude = sin(latitude);
	  double sin_latitude_plus = sin(latitude + d_latitude);
	  double cos_azimuth = cos(azimuth);
	  double cos_azimuth_plus = cos(azimuth + d_azimuth);
	  double sin_azimuth = sin(azimuth);
	  double sin_azimuth_plus = sin(azimuth + d_azimuth);
	  double cos_latitude = cos(latitude);
	  double cos_latitude_plus = cos(latitude + d_latitude);

	  glColor3f(0.0, 0.0, color);
	  vertices[n++].point = WP_Point3D(rad * sin_latitude * cos_azimuth,  rad * sin_latitude * sin_azimuth, rad * cos_latitude);
	  glVertex3fv(vertices[n - 1].point.data);

	  glColor3f(0.0, 0.0, color);
	  vertices[n++].point = WP_Point3D(rad * sin_latitude_plus * cos_azimuth,  rad * sin_latitude_plus * sin_azimuth, rad * cos_latitude_plus);
	  glVertex3fv(vertices[n - 1].point.data);

	  glColor3f(0.0, 0.0, color);
	  vertices[n++].point = WP_Point3D(rad * sin_latitude * cos_azimuth_plus,  rad * sin_latitude * sin_azimuth_plus, rad * cos_latitude);
	  glVertex3fv(vertices[n - 1].point.data);

	  if (latitude > -HALF_PI && latitude < HALF_PI)
	    {
	      glColor3f(0.0, 0.0, color);
	      vertices[n++].point = WP_Point3D(rad * sin_latitude_plus * cos_azimuth_plus,  rad * sin_latitude_plus * sin_azimuth_plus, rad * cos_latitude_plus);
	      glVertex3fv(vertices[n - 1].point.data);
	    }
	}
      color += 0.6;
    }

  glEnd();

  WP_GLState::getInstance()->enableLighting();
  glCullFace(GL_BACK);
  glPopMatrix();
  glEndList();
}

WP_SkyDome::~WP_SkyDome()
{
  if (vertices)
    {
      delete[] vertices;
    }

  if (displayID != 0)
    {
      glDeleteLists(displayID, 1);
    }
}

void WP_SkyDome::drawSkyDome(const WP_Point3D& p) const
{
  glPushMatrix();
  glTranslatef(p.data[0], p.data[1], p.data[2]);
  glCallList(displayID);
  glPopMatrix();
}


