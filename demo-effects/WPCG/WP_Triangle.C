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

#include "WP_Vertex.h"
#include "WP_Triangle.h"

void 
WP_Triangle::drawOpenGL() const
{	
  glNormal3fv(vertices[0]->normal.data);
  glTexCoord2fv(vertices[0]->texCoords);	
  glVertex3fv(vertices[0]->point.data);
  
  glNormal3fv(vertices[1]->normal.data);
  glTexCoord2fv(vertices[1]->texCoords);	
  glVertex3fv(vertices[1]->point.data);
  
  glNormal3fv(vertices[2]->normal.data);
  glTexCoord2fv(vertices[2]->texCoords);	
  glVertex3fv(vertices[2]->point.data);
}

void 
WP_TriangleStrip::drawOpenGL(const WP_Vertex *verticesFrameA, const WP_Vertex *verticesFrameB, scalar interpolation) const
{
  unsigned int i, j;
  if (verticesFrameB)
    {
      glBegin(GL_TRIANGLE_STRIP);
      for(i = 0, j =0; i < numberIndices; i++, j += 2)
	{
	  WP_Vertex v = verticesFrameA[indices[i]];
	  glNormal3fv(v.normal.data);
	  glTexCoord2fv(texCoords + j);
	  v.lerp3D(verticesFrameB + indices[i], interpolation);
	  glVertex3fv(v.point.data);
	}
      glEnd();
    }
  else
    {
      glBegin(GL_TRIANGLE_STRIP);
      for(i = 0, j = 0; i < numberIndices; i++, j += 2)
	{
	  const WP_Vertex *v = verticesFrameA + indices[i];
	  glNormal3fv(v->normal.data);
	  glTexCoord2fv(texCoords + j);
	  glVertex3fv(v->point.data);
	}    
      glEnd();
    }
}

void 
WP_TriangleFan::drawOpenGL(const WP_Vertex *verticesFrameA, const WP_Vertex *verticesFrameB, scalar interpolation) const
{
  unsigned int i, j;
  if (verticesFrameB)
    {
      glBegin(GL_TRIANGLE_FAN);
      for(i = 0, j = 0; i < numberIndices; i++, j += 2)
	{
	  WP_Vertex v = verticesFrameA[indices[i]];
	  glNormal3fv(v.normal.data);
	  glTexCoord2fv(texCoords + j);
	  v.lerp3D(verticesFrameB + indices[i], interpolation);
	  glVertex3fv(v.point.data);
	}
      glEnd();
    }
  else
    {
      glBegin(GL_TRIANGLE_FAN);
      for(i = 0, j = 0; i < numberIndices; i++, j += 2)
	{
	  const WP_Vertex *v = verticesFrameA + indices[i];
	  glNormal3fv(v->normal.data);
	  glTexCoord2fv(texCoords + j);
	  glVertex3fv(v->point.data);
	}
      glEnd();
    }
}
