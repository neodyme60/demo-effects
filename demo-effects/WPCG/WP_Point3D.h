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

#ifndef WP_POINT3D_H
#define WP_POINT3D_H

#include "WP_Def.h"

namespace WPCG
{
class WP_Vector3D; //forward declaration
class WP_Matrix3D; //forward declaration

/**
 * this class represents a 3D point\n
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
class WP_Point3D
{
public:

  WP_Point3D(){ data[0] = data[1] = data[2] = 0.0; data[3] = 1.0f;};

  /** 
   * @param x the x coordinate of this point
   * @param y the y coordinate of this point
   * @param z the z coordinate of this point
   */
  WP_Point3D(scalar x, scalar y, scalar z){ data[0] = x; data[1] = y; data[2] = z; data[3] = 1.0f;};
  
  ~WP_Point3D(){};

  //copy constructor

  WP_Point3D(const WP_Point3D& p)
    {
      data[0] = p.data[0];
      data[1] = p.data[1];
      data[2] = p.data[2];
    }

  /**
   * assignment operator
   * @param p the WP_Point3D object which is copied to this object
   * @return a reference to this object containing the copied point values
   */
  WP_Point3D& operator=(const WP_Point3D& p);
  
  /**
   * this operator multiplies the current point by a matrix
   * @param m a WP_Matrix3D object representing the matrix by which this point is multiplied
   * @return a reference to this object containing the new point values after multiplication by the matrix
   */
  WP_Point3D& operator*=(const WP_Matrix3D& m);

  /**
   * this operator adds a vector to this point resulting in a new point 
   * @param v a WP_Vector3D object representing the vector which is added to this point
   * @return a reference to this object containing the new point values after the addition of the vector
   */
  WP_Point3D& operator+=(const WP_Vector3D& v);

  /**
   * this operator subtracts a vector from this point resulting in a new point
   * @param v a WP_Vector3D object representing the vector which is subtracted from this point
   * @return a reference to this object containing the new point values after the subtraction of the vector
   */
  WP_Point3D& operator-=(const WP_Vector3D& v);

  /**
   * this operator substracts a point from this point resulting in a vector
   * @param p a WP_Point3D object representing the point which is subtracted from this point
   * @return a WP_Vector3D object representing the vector
   */
  WP_Vector3D operator-(const WP_Point3D& p) const;

  /**
   * this operator subtracts a vector from this point resulting in a new point
   * @param v a WP_Vector3D object representing the vector which is subtracted from this point
   * @return a WP_Point3D object representing the new point
   */
  WP_Point3D operator-(const WP_Vector3D& v) const;

  /**
   * this operator adds a point to this point resulting in a vector
   * @param p a WP_Point3D object representing the point which is added to this point
   * @return a WP_Vector3D object representing the vector
   */
  WP_Vector3D operator+(const WP_Point3D& p) const;

  /**
   * this operator adds a vector to this point resulting in a new point
   * @param v a WP_Vector3D object representing the vector which is added to this point
   * @return a WP_Point3D object representing the new point
   */
  WP_Point3D operator+(const WP_Vector3D& v) const;
  
  /**
   * this function converts a vector to a point by setting the x, y, and z values of the vector in the point
   * @param v the vector which values are taken and set in this point
   */
  void set(const WP_Vector3D& v);

  /**
   * this function creates a vector from this point's x, y and z values
   * @return a WP_Vector3D object representing the created vector
   */
  WP_Vector3D toVector() const;

  /**
   * this function draws the 3D point to the screen using OpenGL
   */
  void draw() const;
  
  /**
   * an array of scalars representing the homogenous point, so data[0] = x, data[1] = y, data[2] = z and data[3] = 1.0f
   */
  scalar data[4];
};
}
#endif

