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
#include <list>
#include <iostream.h>
#include <stdlib.h>
#include "WP_Math.h"
#include "WP_Vertex.h"
#include "WP_Quad.h"
#include "WP_Terrain.h"

WP_Terrain::WP_Terrain(int _width, int _height, int number_iterations, scalar resolution, int scale): 
  width(_width), height(_height), displayID(0), height_map(0)
{
  //creates a random 3D terrain

  height_map = new (WP_Vertex*)[height];
  if (!height_map)
    {
      cerr << "Unable to allocate memory for height_map" << endl;
      exit(-1);
    }
  for (int i = 0; i < height; i++)
    {
      height_map[i] = new WP_Vertex[width];
      if (!height_map[i])
	{
	  cerr << "Unable to allocate memory for height_map" << endl;
	  exit(-1);
	}
    }
  
  //create heightmap
  for (int i = 0; i < number_iterations; i++)
    {
      //create hill

      //choose random center in height_map

      int center_x = random() % width;
      int center_y = random() % height;
      int radius = random() % (width >> 1);
      int radius_square = radius << 1;

      //FIXME optimize, cycles through all vertices

      for (int k = 0; k < height; k++)
	{
	  for (int l = 0; l < width; l++)
	    {
	      //raise hill
	      int x = center_x - l;
	      x *= x;
	      int z = center_y - k;
	      z *= z;

	      float y = radius_square - (x + z);
	      if (y > 0.0)
		{
		  if (height_map[k][l].point.data[1] > 0.0)
		    {
		      height_map[k][l].point.data[1] += y;
		    }
		  else
		    {
		      height_map[k][l].point.data[1] = y;
		    }
		}
	    }
	}
    }

  //normalize terrain

  int min = INT_MAX;
  int max = 0;

  for (int k = 0; k < height; k++)
    {
      for (int l = 0; l < width; l++)
	{
	  if (height_map[k][l].point.data[1] > max)
	    {
	      max = (int)height_map[k][l].point.data[1];
	    }
	  else if (height_map[k][l].point.data[1] < min)
	    {
	      min = (int)height_map[k][l].point.data[1];
	    }
	}
    }

  scalar norm = max - min;
  if (norm == 0.0)
    {
      norm = 0.0001;
    }

  for (int k = 0; k < height; k++)
    {
      for (int l = 0; l < width; l++)
	{
	  height_map[k][l].point.data[1] = (height_map[k][l].point.data[1] - min) / norm;
	}
    }

  //flatten terrain
  
  for (int k = 0; k < height; k++)
    {
      for (int l = 0; l < width; l++)
	{
	  height_map[k][l].point.data[1] *= height_map[k][l].point.data[1];
	}
    }

  //finalize height map (by adding x and z values and scaling by spread and resolution

 for (int k = 0; k < height; k++)
    {
      for (int l = 0; l < width; l++)
	{
	  height_map[k][l].point.data[0] = l * resolution;
	  height_map[k][l].point.data[1] *= scale;
	  height_map[k][l].point.data[2] = k * resolution;
	}
    }

  //create quad list for vertex normal calculation (for per vertex lighting)
  list<WP_Quad*> quads;

 for (int k = 0; k < height; k++)
    {
      for (int l = 0; l < width; l++)
	{
	  if (k + 1 < height)
	    {
	      WP_Quad* quad = new WP_Quad();
	      quad->vertices[0] = &height_map[k][l];
	      quad->vertices[1] = &height_map[k + 1][l];
	      quad->vertices[2] = &height_map[k][l + 1];
	      quad->vertices[3] = &height_map[k + 1][l + 1];
	      quads.push_back(quad);
	    }
	}
    }

 //compute quad normals

  list<WP_Quad*>::const_iterator i = quads.begin();
  while (i != quads.end())
    {	
      WP_Vector3D v1 = (*i)->vertices[1]->point - (*i)->vertices[3]->point;
      WP_Vector3D v2 = (*i)->vertices[2]->point - (*i)->vertices[0]->point;
      v1.crossProduct(v2);
      v1.normalize();
      (*i)->normal = v1;
      i++;
    }

  //compute vertex normals

  for (int k = 0; k < height; k++)
    {
      for (int l = 0; l < width; l++)
	{
	  WP_Vector3D normal_sum;
	  list<WP_Quad*>::const_iterator i = quads.begin();
	  WP_Vertex* v = &height_map[k][l];
	  while (i != quads.end())
	    {	
	      if ((*i)->vertices[0] == v || (*i)->vertices[1] == v || (*i)->vertices[2] == v || (*i)->vertices[3] == v)
	      {
	        normal_sum += (*i)->normal;
	      }
	      i++;
	    }
	  normal_sum.normalize();
	  height_map[k][l].normal = normal_sum;
	}
    }
  
  //create display list

  displayID = glGenLists(1);
  glNewList(displayID, GL_COMPILE);
  glPushMatrix();

  //send terrain to OpenGL
  for (int k = 0; k < height; k++)
    {
      glBegin(GL_QUAD_STRIP);
      for (int l = 0; l < width; l++)
	{
	  if (k + 1 < height)
	    {
	      glNormal3fv(height_map[k][l].normal.data);
	      glVertex3fv(height_map[k][l].point.data);  
	      glNormal3fv(height_map[k + 1][l].normal.data);
	      glVertex3fv(height_map[k + 1][l].point.data);
	    }
	}
      glEnd();
    }
 
  glPopMatrix();
  glEndList();

  //clear quads
  i = quads.begin();
  while (i != quads.end())
    {	
      delete (*i);
      i++;
    }

  displacement_x = (width * resolution) / 2;
  displacement_y = (height * resolution) / 2;
}

WP_Terrain::~WP_Terrain()
{
  if (height_map)
    {
      for (int i = 0; i < height; i++)
	{
	  delete[] height_map[i];
	}
      delete[] height_map;
    }

  if (displayID != 0)
    {
      glDeleteLists(displayID, 1);
    }
}

void WP_Terrain::drawTerrain() const
{
  glPushMatrix();
  glTranslatef(middle.data[0] - displacement_x, middle.data[1], middle.data[2] - displacement_y);
  glCallList(displayID);
  glPopMatrix();
}



