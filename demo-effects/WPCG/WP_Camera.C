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

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include "WP_ObjectManager.h"
#include "WP_Camera.h"

WP_Camera* WP_Camera::_instance = 0;

WP_Camera::WP_Camera(): fixed_object(0), follow_distance(10.0f), 
			follow_angleX(0), follow_angleY(0), follow_angleZ(0), objects_in_frustum(0),
			normal_viewing_volume(true), math(WP_Math::getInstance()),
			state(WP_GLState::getInstance()){}

void WP_Camera::setFrustumAndCamera(scalar _viewAngle, unsigned int width, unsigned int height, scalar _nearPlane, scalar _farPlane, 
				    const WP_Point3D& _eye, const WP_Point3D& _look, const WP_Vector3D& _up)
{
  viewAngle		= _viewAngle;
  aspectRatio		= ((float)width) / ((float)height);
  nearPlane		= _nearPlane;
  farPlane		= _farPlane;
  screen_width = width;
  screen_height = height;
  
  state->projection();
  glPushMatrix();
  glLoadIdentity();
  gluPerspective(viewAngle, aspectRatio, nearPlane, farPlane);
  state->modelview();

  eye = _eye;
  look = _look;
  up = _up;
  n = eye - look;
  u = up;

  if (!u.crossProduct(n))
    {
      cerr << "Unable to compute crossproduct, it's likely that vectors n and u are pointing in the same direction" << endl;
    }
  
  n.normalize();
  u.normalize();
  
  v = n;
  v.crossProduct(u); //no need to check if crossproduct failed because we no for sure that n and u have an angle of 0 degrees between them
  
  setModelViewMatrixGL();
}

void WP_Camera::setModelViewMatrixGL() 
{
  matrix = WP_Matrix3D(eye.toVector(), u, v, n);
  glLoadMatrixf(matrix.data);
  computeFrustum(); //FIXME don't compute the frustum planes everytime, just translate, rotate etc them
}

void WP_Camera::setPickingVolume(int width, int height, int x, int y) 
{
  if (normal_viewing_volume)
    {
      GLint viewport[4];
      
      state->projection();
      glPushMatrix();
      glLoadIdentity();
      
      glGetIntegerv(GL_VIEWPORT,viewport);
      gluPickMatrix(x, viewport[3] - y,
		    width , height, viewport);
      gluPerspective(viewAngle, aspectRatio, nearPlane, farPlane);
      state->modelview();
      normal_viewing_volume = false;
    }
}

void WP_Camera::setRenderVolume() 
{
  if (!normal_viewing_volume)
    {
      state->projection();
      glPopMatrix();
      state->modelview();
      normal_viewing_volume = true;
    }
}
 
//frustum code by Mark Morley (www.markmorley.com)
void WP_Camera::computeFrustum()
{
  scalar   proj[16];
  scalar   modl[16];
  scalar   clip[16];
  scalar   coords[4];
  scalar   t;

  //FIXME instead of recalculating frustum reorientate frustum planes
  
  /* Get the current PROJECTION matrix from OpenGL */
  glGetFloatv( GL_PROJECTION_MATRIX, proj );
  
  /* Get the current MODELVIEW matrix from OpenGL */
  glGetFloatv( GL_MODELVIEW_MATRIX, modl );
  
  /* Combine the two matrices (multiply projection by modelview) */
  clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
  clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
  clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
  clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
  
  clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
  clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
  clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
  clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
  
  clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
  clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
  clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
  clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
  
  clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
  clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
  clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
  clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
  
  /* Extract the numbers for the RIGHT plane */
  coords[0] = clip[ 3] - clip[ 0];
  coords[1] = clip[ 7] - clip[ 4];
  coords[2] = clip[11] - clip[ 8]; 
  coords[3] = clip[15] - clip[12];
  
  /* Normalize the result */
  t = sqrt( coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2] );
  coords[0] /= t;
  coords[1] /= t;
  coords[2] /= t;
  coords[3] /= t;

  frustum[0].Set(-coords[0], -coords[1], -coords[2], -coords[3]);
  
  /* Extract the numbers for the LEFT plane */
  coords[0] = clip[ 3] + clip[ 0];
  coords[1] = clip[ 7] + clip[ 4];
  coords[2] = clip[11] + clip[ 8];
  coords[3] = clip[15] + clip[12];
  
  /* Normalize the result */
  t = sqrt( coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2] );
  coords[0] /= t;
  coords[1] /= t;
  coords[2] /= t;
  coords[3] /= t;

  frustum[1].Set(-coords[0], -coords[1], -coords[2], -coords[3]);
  
  /* Extract the BOTTOM plane */
  coords[0] = clip[ 3] + clip[ 1];
  coords[1] = clip[ 7] + clip[ 5];
  coords[2] = clip[11] + clip[ 9];
  coords[3] = clip[15] + clip[13];
  
  /* Normalize the result */
  t = sqrt( coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2] );
  coords[0] /= t;
  coords[1] /= t;
  coords[2] /= t;
  coords[3] /= t;
  
  frustum[2].Set(-coords[0], -coords[1], -coords[2], -coords[3]);

  /* Extract the TOP plane */
  coords[0] = clip[ 3] - clip[ 1];
  coords[1] = clip[ 7] - clip[ 5];
  coords[2] = clip[11] - clip[ 9];
  coords[3] = clip[15] - clip[13];
  
  /* Normalize the result */
  t = sqrt( coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2] );
  coords[0] /= t;
  coords[1] /= t;
  coords[2] /= t;
  coords[3] /= t;
 
  frustum[3].Set(-coords[0], -coords[1], -coords[2], -coords[3]);
 
  /* Extract the FAR plane */
  coords[0] = clip[ 3] - clip[ 2];
  coords[1] = clip[ 7] - clip[ 6];
  coords[2] = clip[11] - clip[10];
  coords[3] = clip[15] - clip[14];
  
  /* Normalize the result */
  t = sqrt( coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2] );
  coords[0] /= t;
  coords[1] /= t;
  coords[2] /= t;
  coords[3] /= t;
  
  frustum[4].Set(-coords[0], -coords[1], -coords[2], -coords[3]);

  /* Extract the NEAR plane */
  coords[0] = clip[ 3] + clip[ 2];
  coords[1] = clip[ 7] + clip[ 6];
  coords[2] = clip[11] + clip[10];
  coords[3] = clip[15] + clip[14];
  
  /* Normalize the result */
  t = sqrt( coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2] );
  coords[0] /= t;
  coords[1] /= t;
  coords[2] /= t;
  coords[3] /= t;

  frustum[5].Set(-coords[0], -coords[1], -coords[2], -coords[3]);
}

void WP_Camera::slide(scalar deltaU, scalar deltaV, scalar deltaN)
{
  WP_Vector3D translateU(u * deltaU);
  WP_Vector3D translateV(v * deltaV);
  WP_Vector3D translateN(n * deltaN);
  
  WP_Vector3D translate = translateU + translateV + translateN;
  eye += translate;
  setModelViewMatrixGL();
}

//first rotated around X(U), then Y(V) and finally Z(N)
void WP_Camera::rotate(int angleU, int angleV, int angleN)
{
  WP_Vector3D temp;
  scalar cos;
  scalar sin;
  
  if (angleU != 0)
    {
      cos = math->getCos(angleU);
      sin = math->getSin(angleU);
      
      temp = v;
      v = (temp * cos) + (n * sin);
      n = (temp * -sin) + (n * cos);
    }
  if (angleV != 0)
    {
      cos = math->getCos(angleV);
      sin = math->getSin(angleV);
      
      temp = n;
      n = (temp * cos) + (u * sin);
      u = (temp * -sin) + (u * cos);
    }
  if (angleN != 0)
    {
      cos = math->getCos(angleN);
      sin = math->getSin(angleN);
      
      temp = u;
      u = (temp * cos) + (v * sin);
      v = (temp * -sin) + (v * cos);
    }
  
  setModelViewMatrixGL();
}

void WP_Camera::pitch(int angle)
{
  rotate(angle, 0, 0);
}

void WP_Camera::roll(int angle)
{
  rotate(0, 0, angle);
}

void WP_Camera::yaw(int angle)
{
  rotate(0, angle, 0);
}

WP_Ray3D WP_Camera::createRayForTracing(int x, int y) const
{
  WP_Ray3D ray;
  ray.start = eye;

  float H = nearPlane * tan(math->degreeToRad((int)(viewAngle / 2.0f)));
  float W = H * aspectRatio;

  ray.direction = n * -nearPlane + u * (-W + (W * ((x * 2.0f) / (float)screen_width))) + v * (-H + (H * ((y * 2.0f) / (float)screen_height)));
  return ray;
}

void WP_Camera::followObject()
{
  //FIXME only update if object changed orientation

  if (fixed_object)
    {
      WP_Point3D center(fixed_object->getXPos(), fixed_object->getYPos(), fixed_object->getZPos());
      
      //determine new eye point
      eye = center + (fixed_object->dir * follow_distance);

      bool followX = (follow_angleX % 360) != 0;  
      bool followY = (follow_angleY % 360) != 0;  

      if (followX || followY)
	{
	  //because the eye must be rotated about the center, the center must be translated to the origin by translating the eye by that translation
	  WP_Matrix3D translation(TRANSLATION_MATRIX, -center.data[0], -center.data[1], -center.data[2]);
	  eye *= translation;
	  
	  //rotate eye to be in the northern position, in this way the camera will be transformed correctly regardless the object its heading
	  WP_Matrix3D heading(Y_ROTATION_MATRIX, fixed_object->heading);
	  WP_Matrix3D pitch(X_ROTATION_MATRIX, -fixed_object->pitch);
	  eye *= heading;
	  eye *= pitch;
	  
	  if (followX)
	    {
	      WP_Matrix3D x_rotation(X_ROTATION_MATRIX, follow_angleX);
	      eye *= x_rotation;
	    }
	  if (followY)
	    {
	      WP_Matrix3D y_rotation(Y_ROTATION_MATRIX, follow_angleY);
	      eye *= y_rotation;
	    }
	  
	  //rotate object back in original orientation
	  heading = WP_Matrix3D(Y_ROTATION_MATRIX, -fixed_object->heading);
	  pitch = WP_Matrix3D(X_ROTATION_MATRIX, fixed_object->pitch);
	  eye *= pitch;
	  eye *= heading;
	  
	  //translate back to original orientation
	  translation = WP_Matrix3D(TRANSLATION_MATRIX, center.data[0], center.data[1], center.data[2]);
	  eye *= translation;
	}
      
      //adjust camera to face object
      
      n = eye - center;
      u = fixed_object->up;
      n.normalize();
      
      if (!u.crossProduct(n))
	{
	  //crossproduct failed because n and u have an angle of 0 degrees between them
	  //in this case crossproduct of fixed_object->dir and fixed_object->up results in correct vector
	  WP_Vector3D temp = fixed_object->dir;
	  temp.crossProduct(fixed_object->up);
	  u.normalize();
	  
	  if (u.data[0] != n.data[0] || u.data[1] != n.data[1] || u.data[2] != n.data[2])
	    {
	      //vectors are pointing in opposite direction
	      u = -temp;
	    }
	  else
	    {
	      //vectors are pointing in same direction
	      u = temp;
	    }
	}
      
      u.normalize();
      
      v = n;
      v.crossProduct(u); //no need to check if crossproduct failed because we no for sure that n and u are not pointing in the same direction
      rotate(0, 0, follow_angleZ);

      computeFrustum();
    }
}

