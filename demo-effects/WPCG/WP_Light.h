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

#ifndef WP_LIGHT_H
#define WP_LIGHT_H

//forward declarations
class WP_Point3D;
class WP_Vector3D;

#include "WP_Def.h"
#include "WP_Color.h"

/**
 * this class represents a light in OpenGL\n
 * @author Copyright (C) 2001 W.P. van Paassen   peter@paassen.tmfweb.nl
 *
 *  This program is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
 */

class WP_Light
{ 
 public:

  /**
   * an enumeration of the kind of position stored in a WP_Light object, being point or vector
   */
  enum WP_Light_Pos_Kind {point, vector};

  WP_Light()
    {
      k = point;
      p = new WP_Point3D(0.0, 0.0, 0.0);
    }

  virtual ~WP_Light()
    {
      if (k == point)
	{
	  if (p)
	    {
	      delete p;
	    }
	}
      else if (k == vector)
	{
	  if (v)
	    {
	      delete v;
	    }
	}
    }

  /**
   * this function returns the kind of positional data which is stored in this light, being point or vector (directional lighting)
   * @return the kind of positional data which is stored in this light, being point or vector (directional lighting)
   */
  WP_Light_Pos_Kind getKind() const
    {
      return k;
    }

  /**
   * this function returns a pointer to a WP_Point3D object representing the position of the light
   * @return a pointer to a WP_Point3D object representing the position of the light, 0 in case the position is stored as a vector
   */
  WP_Point3D* getPointPosition()
    {
      if (k == point)
	{
	  return p;
	}
      return 0;
    }

  /**
   * this function returns a pointer to a WP_Vector3D object representing directional (infinite) light, for example the sun
   * @return a pointer to a WP_Vector3D object representing the position of the light, 0 in case the position is stored as a point
   */
  WP_Vector3D* getVectorPosition()
    {
      if (k == vector)
	{
	  return v;
	}
      return 0;
    }

  /**
   * this function sets the position of the light as a point or a vector
   * @param x the x position of the light in the scene
   * @param y the y position of the light in the scene
   * @param z the z position of the light in the scene
   * @param h the homogenous component of the position of the light. If this is 0, the light is a directional light and the light is not in the scene but remote (like for example the sun) and is represented by a vector, if this is 1 it is represented as a point
   */
  void setPosition(float x, float y, float z, float h)
    {
      if (h == 1.0)
	{
	  if (k == vector)
	    {
	      if (v)
		{
		  delete v;
		}
	    }
	  if (k == point)
	    {
	      if (p)
		{
		  delete p;
		}
	    }

	  k = point;
	  p = new WP_Point3D(x, y, z);
	}
      else
	{
	  if (k == vector)
	    {
	      if (v)
		{
		  delete v;
		}
	    }
	  if (k == point)
	    {
	      if (p)
		{
		  delete p;
		}
	    }

	  k = vector;
	  v = new WP_Vector3D(x, y, z);
	}
    }

  /**
   * the color that this light emmits
   */
  WP_Color color;
 
 protected:

  //an enumeration holding the representation of the position, being a point or a vector
  WP_Light_Pos_Kind k;
  
  /**
   * a union holding either the position of the light in the 3d world represented by a point or represented by a vector (the latter in case of directional lighting)
   */
  union
  {
    WP_Point3D* p;
    WP_Vector3D* v;
  };
};
#endif

