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

#ifndef WP_Matrix3D_H
#define WP_Matrix3D_H

#include "WP_Def.h"

namespace WPCG
{
/**
 * this class represents a 3D matrix\n
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
class WP_Matrix3D
{
public:
	friend class WP_Vector3D;
	friend class WP_Point3D;

	WP_Matrix3D();
	~WP_Matrix3D(){};	
	
	/**
	 * @param type the type of the matrix. TRANSLATION_MATRIX for a translation matrix and SCALING_MATRIX for a scaling matrix. Any other value creates the identity matrix
	 * @param x the x value of the translation or scaling operation
	 * @param y the y value of the translation or scaling operation
	 * @param z the z value of the translation or scaling operation
	 */
	WP_Matrix3D(const byte type, scalar x, scalar y, scalar z);

	/**
	 * @param type the type of the matrix. TRANSLATION_MATRIX for a translation matrix and SCALING_MATRIX for a scaling matrix. Any other value creates the identity matrix
	 * @param v a WP_Vector3D object containing the x, y and z values to be used for translation or scaling
	 */
	WP_Matrix3D(const byte type, WP_Vector3D* v);

	/**
	 * @param type the type of the matrix. X_ROTATION_MATRIX, Y_ROTATION_MATRIX and Z_ROTATION_MATRIX are supported. Other type values creates the identity matrix
	 * @param angle the angle of the rotation around the selected axis
	 */
	WP_Matrix3D(const byte type, scalar angle);

	/**
	 * this constructor builds a camera transformation matrix by which world coordinates are transformed to camera coordinates. This is the same matrix as the OpenGL call gluLookAt() builds and is usefull if a camera implementation is created without the use of gluLookAt(). See F.S. Hill JR, Computer Graphics using OpenGL page 363-365
	 * @param eye a WP_Vector3D object containing the camera's eye position
	 * @param u a WP_Vector3D object representing the direction of the x-axis of the camera's coordinate system
	 * @param v a WP_Vector3D object representing the direction of the y-axis of the camera's coordinate system
	 * @param n a WP_Vector3D object representing the direction of the z-axis of the camera's coordinate system
	 */
	WP_Matrix3D(WP_Vector3D eye, const WP_Vector3D& u, const WP_Vector3D& v, const WP_Vector3D& n);

	/**
	 * @param f an array of 4 rows and 4 columns
	 */
	WP_Matrix3D(scalar f[4][4]); 

	/**
	 * @param f a pointer to an array of 16 floats. The floats should be ordered per row so indices 0-3 are the indices in the four columns on the first row, just like in OpenGL
	 */
	WP_Matrix3D(scalar* f);

	/**
	 * assignment operator
	 * @param m the matrix containing the to be copied values
	 * @return a reference to this matrix containing the copied values
	 */
	WP_Matrix3D& operator=(const WP_Matrix3D& m);

	/**
	 * this operator postmultiplies the current matrix by matrix m
	 * @param m the matrix which will be multiplied by the current matrix
	 * @return a new matrix containing the multiplication of the current matrix and m
	 */
	WP_Matrix3D operator*(const WP_Matrix3D & m) const;

	/**
	 * this operator adds m to the current matrix
	 * @param m the matrix which will be added to the current matrix
	 * @return a new matrix containing the addition of the current matrix and m
	 */
	WP_Matrix3D operator+(const WP_Matrix3D & m) const;

	/**
	 * this operator subtracts m from the current matrix
	 * @param m the matrix which will be subtracted from the current matrix
	 * @return a new matrix containing the subtraction of the current matrix and m
	 */
	WP_Matrix3D operator-(const WP_Matrix3D & m) const;

	/**
	 * this operator multiplies every matrix cell by a scalar
	 * @param s the scalar by which every matrix cell is multiplied
	 * @return a new matrix containing the multiplied cells
	 */
	WP_Matrix3D operator*(scalar s) const;

	/**
	 * this operator divides every matrix cell by a scalar
	 * @param s the scalar by which every matrix cell is divided
	 * @return a new matrix containing the divided cells
	 */
	WP_Matrix3D operator/(scalar s) const;

	/**
	 * this operator inverts every matrix cell
	 */
	WP_Matrix3D& operator-();

	/**
	 * this operator postmultiplies the current matrix by m and stores the result in this object
	 * @param m the matrix which will be multiplied by this matrix
	 * @return a reference to this object
	 */
	WP_Matrix3D& operator*=(const WP_Matrix3D & m);

	/**
	 * this operator adds m to the current matrix and stores the result in this object
	 * @param m the matrix which will be added to this matrix
	 * @return a reference to this object
	 */
	WP_Matrix3D& operator+=(const WP_Matrix3D & m);

	/**
	 * this operator subtracts m from this matrix and stores the result in this object
	 * @param m the matrix which will be subtracted from this matrix
	 * @return a reference to this object
	 */
	WP_Matrix3D& operator-=(const WP_Matrix3D & m);

	/**
	 * this operator multiplies each matrix cell by a scalar and stores the result in this object
	 * @param s the scalar by which every matrix cell is multiplied
	 * @return a reference to this object
	 */
	WP_Matrix3D& operator*=(scalar s);

	/**
	 * this operator divides each matrix cell by a scalar and stores the result in this object
	 * @param s the scalar by which every matrix cell is divided
	 * @return a reference to this object
	 */
	WP_Matrix3D& operator/=(scalar s);

	/**
	 * this function creates the identity matrix
	 */
	void createIdentity();

	/**
	 * this function premultiplies this matrix by m, so the matrix multiplication m * this matrix is computed
	 * @param m the matrix by which this matrix is premultiplied
	 */
	void preMultiply(const WP_Matrix3D& m);

	/**
	 * this function transposes this matrix
	 */
	void transpose();

	/**
	 * this function prints this 3D matrix to stdout
	 */
	void print() const;

	/**
	 * this function computes the determinant of this matrix. Currently only inversion of homogenous matrices is supported
	 * @return the determinant. 0.0f in case of non-homogenous matrix
	 */
	scalar determinant();

	/**
	 * this function computes the inverse of this matrix. Currently only inversion of homogenous matrices is supported
	 * a boolean indicating if an inverse exist and if the current matrix was inversed succesfully, false if non homogenous matrix
	 */
	bool inverse();
	
	/**
	 * this function determines if this matrix is non singular
	 * @return a boolean indicating if this matrix is non singular
	 */
	bool isNonSingular() 
	  { 
	    scalar d = determinant(); 
	    return d <= -0.00001 || d >= 0.00001;
	  };

	/**
	 * this function determines if this matrix is the identity matrix
	 * @return a boolean indicating if this matrix is the identity matrix
	 */
	bool isIdentity();

	/**
	 * this function determines if this matrix is homogenous (lower row is 0 0 0 1)
	 * @return a boolean indicating if this matrix is homogenous
	 */
	bool isHomogenous();

	/**
	 * an array of 16 floats representing the matrix's cells. The cells are orientated from top to bottom and from left to right, so for example the cell in the fourth column of the first row is located at index 12. This orientation is the same as the matrices in OpenGL
	 */
	scalar data[16];

private:

	/**
	 * this function creates the matrix belonging to a rotation around the x-axis of <i>angle</i> degrees
	 * @param angle the angle in degrees of the rotation
	 */
	void createXRotation(scalar angle);

	/**
	 * this function creates the matrix belonging to a rotation around the y-axis of <i>angle</i> degrees
	 * @param angle the angle in degrees of the rotation
	 */
	void createYRotation(scalar angle);
	
	/**
	 * this function creates the matrix belonging to a rotation around the z-axis of <i>angle</i> degrees
	 * @param angle the angle in degrees of the rotation
	 */
	void createZRotation(scalar angle);
};
}
#endif

