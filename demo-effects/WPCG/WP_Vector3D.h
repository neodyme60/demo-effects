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

#ifndef WP_Vector3D_H
#define WP_Vector3D_H

#include "WP_Def.h"

namespace WPCG
{
//forward declaration
class WP_Matrix3D;				

/**
 * this class represents a 3D vector\n
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
class WP_Vector3D
{
public:
	WP_Vector3D();
	
	/**
	 * @param x the x component of the vector
	 * @param y the y component of the vector
	 * @param z the z component of the vector
	 */
	WP_Vector3D(scalar x, scalar y, scalar z);

	/**
	 * @param s an array of 3 scalars representing the vector values
	 */
	WP_Vector3D(scalar* s);

	~WP_Vector3D(){};

	// copy constructor

	WP_Vector3D(const WP_Vector3D& v)
	  {
	    data[0] = v.data[0];
	    data[1] = v.data[1];
	    data[2] = v.data[2];
	  }

	/**
	 * assignment operator
	 * @param v the WP_Vector3D object which is copied to this object
	 * @return a reference to this object containing the copied vector values
	 */
	WP_Vector3D& operator=(const WP_Vector3D& v);

	/**
	 * this operator computes the dot product of the two 3D vectors
	 * @param v a WP_Vector3D object representing a vector by which this vector is multiplied to get the dot product
	 * @return the dot product
	 */
	scalar operator*(const WP_Vector3D& v) const;

	/**
	 * this operator subtracts a vector from this vector
	 * @param v a WP_Vector3D object representing the vector which is subtracted from this vector
	 * @return a WP_Vector3D object representing the vector after subtraction
	 */
	WP_Vector3D operator-(const WP_Vector3D& v) const;

	/**
	 * this operator adds a vector to this vector
	 * @param v a WP_Vector3D object representing the vector which is added to this vector
	 * @return a WP_Vector3D object representing the vector after addition
	 */
	WP_Vector3D operator+(const WP_Vector3D& v) const;

	/**
	 * this operator scales the vector by multiplying its values by a scalar
	 * @param s the scalar by which this vector is scaled
	 * @return a WP_Vector3D object representing the scaled vector
	 */
	WP_Vector3D operator*(scalar s) const;

	/**
	 * this operator scales the vector by dividing its values by a scalar
	 * @param s the scalar by which this vector is scaled
	 * @return a WP_Vector3D object representing the scaled vector
	 */
	WP_Vector3D operator/(scalar s) const;

	 
	/**
	 * this operator inverts the vector
	 * @return a reference to this WP_Vector3D object containing the inverted values
	 */
	WP_Vector3D& operator-();

	/**
	 * this operator subtracts a vector from this vector and assigns the result to this vector
	 * @param v a WP_Vector3D object representing the vector which is subtracted from this vector
	 * @return a reference to this WP_Vector3D object containing the new values
	 */
	WP_Vector3D& operator-=(const WP_Vector3D& v);

	/**
	 * this operator adds a vector to this vector and assigns the result to this vector
	 * @param v a WP_Vector3D object representing the vector which is added to this vector
	 * @return a reference to this WP_Vector3D object containing the new values
	 */
	WP_Vector3D& operator+=(const WP_Vector3D& v);

	/**
	 * this operator scales this vector by using multiplication
	 * @param s the scalar by which this vector is scaled
	 * @return a reference to this WP_Vector3D object containing the new values
	 */
	WP_Vector3D& operator*=(scalar s);

	/**
	 * this operator scales this vector by using division
	 * @param s the scalar by which this vector is scaled
	 * @return a reference to this WP_Vector3D object containing the new values
	 */
	WP_Vector3D& operator/=(scalar s);
	
	/**
	 * this operator multiplies this vector by a 3D matrix
	 * @param m a WP_Matrix3D object which represents the 3D matrix by which this vector is multiplied
	 * @return a reference to this WP_Vector3D object containing the new values
	 */ 
	WP_Vector3D& operator*=(const WP_Matrix3D& m);

	/**
	 * this operator multiplies this vector by a 3D matrix
	 * @param m a WP_Matrix3D object which represents the 3D matrix by which this vector is multiplied
	 * @return a WP_Vector3D object containing the new values
	 */ 
	WP_Vector3D operator*(const WP_Matrix3D& m) const;

	/**
	 * this function computes the crossproduct of two vectors (which is a vector) and assigns it to this vector
	 * @param v a WP_Vector3D object representing the vector, used for computing the crossproduct
	 * @return a boolean indicating if the computation of the crossproduct was possible and a success
	 */
	bool crossProduct(const WP_Vector3D& v);

	/**
	 * this function determines if two vectors are orthogonal (dot product is +- 0.0)
	 * @param v a WP_Vector3D object representing the second vector for calculating if the two vectors are orthogonal
	 * @return a boolean indicating if the two vectors are orthogonal
	 */
	bool orthogonal(const WP_Vector3D& v) const;

	/**
	 * this function computes the length of the vector
	 * @return the lenght of the vector
	 */
	scalar length() const;

	/**
	 * this function normalizes the vector
	 * @return the lenght of the vector <b>before</b> normalizing
	 */
	scalar normalize();

	/**
	 * this function computes the angle in degrees between two vectors
	 * @param v a WP_Vector3D object representing the second vector for calculating the angle between two vectors
	 * @return the angle in degrees
	 */
	scalar getDegreesBetween(const WP_Vector3D& v) const;

	/**
	 * this function computes the angle in radians between two vectors
	 * @param v a WP_Vector3D object representing the second vector for calculating the angle between two vectors
	 * @return the angle in radians
	 */
	scalar getRadsBetween(const WP_Vector3D& v) const;

	/**
	 * this function prints this 3D vector to stdout
	 */
	void print() const;

	/**
	 * an array of 4 scalars holding the vector's values. data[0] = x, data[1] = y, data[2] = z, data[3] = 0.0f
	 */
	scalar data[4];
};
}
#endif

