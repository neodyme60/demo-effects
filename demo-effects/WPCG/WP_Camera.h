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

#ifndef WP_CAMERA_H
#define WP_CAMERA_H

#include "WPCG.h"

//forward declaration
class WP_Object;

/**
 * this singleton class represents a camera used for viewing a 3D scene. The camera has its own coordinate system and three vectors represent the axis of this coordinate system. u = x-axis, v = y-axis and n = z-axis. The camera is looking along the negative n-axis. See F.S. Hill, JR, Computer Graphics using OpenGL, second edition, chapter 7\n
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
class WP_Camera
{
public:
	~WP_Camera(){};

	/**
	 * this function is used for moving the camera along one of its own axes in the <b>u</b>, <b>v</b> or <b>n</b> direction. Movement along <b>n</b> is <i>forward</i> or <i>backward</i>. Movement along <b>u</b> is <i>left</i> or <i>right</i>. Movement along <b>v</b> is <i>up</i> or <i>down</i>
	 * @param deltaU the amount of movement along <b>u</b>
	 * @param deltaV the amount of movement along <b>v</b>
	 * @param deltaN the amount of movement along <b>n</b>
	 */
	void slide(scalar deltaU, scalar deltaV, scalar deltaN);

	/**
	 * this function rotates the camera first around <b>u</b>, then around <b>v</b> and finally around <b>n</b>
	 * @param angleU the angle in degrees of rotation around <b>u</b>
	 * @param angleV the angle in degrees of rotation around <b>v</b>
	 * @param angleN the angle in degrees of rotation around <b>n</b>
	 */
	void rotate(int angleU, int angleV, int angleN);

	/**
	 * this function <i>pitches</i> the object. Pitch is an aviation term and the pitch of an airplane is the angle that its longitudinal axis (running from tail to nose and having direction <b>-n</b> makes with the horizontal plane
	 * @param the angle in degrees
	 */
	void pitch(int angle);

	/**
	 * this function <i>rolls</i> the object. Roll is an aviation term and an airplane rolls by rotating about its longitudinal axis (<b>n</b>. The roll is the amount of rotation relative to the horizontal plane.
	 * @param the angle in degrees
	 */
	void roll(int angle);

	/**
	 * this function <i>yaws</i> the object. Yaw is an aviation term and it means changing the heading of the plane by rotating about <b>v</b>
	 * @param the angle in degrees
	 */
	void yaw(int angle);

	/**
	 * this function is used to obtain a pointer to the only instance of this class (singleton)
	 * @return a pointer to the only instance of this class
	 */
	static WP_Camera* getInstance()
	{ 
		if (!_instance)
		{
			_instance = new WP_Camera();
		}
		return _instance;
	};

	/**
	 * this function moves the camera to follow the attached <i>fixed_object</i>. It keeps a certain distance from the object defined by <i>looking_distanceX</i>, <i>looking_distanceY</i> and <i>looking_distanceZ</i>
	 */
	void followObject();

	/**
	 * this function sets the view volume and the camera in OpenGL. See F.S. Hill,JR Computer Graphics using OpenGL, page 360
	 * @param v the viewangle in degrees, this sets the angle between the top and bottom walls of the view volume (frustum)
	 * @param a the aspect ratio, this sets the aspect ratio of any window parallel to the xy-plane
	 * @param n the near plane, this sets the distance from the eye to the near plane 
	 * @param f the far plane, this sets the distance from the eye to the far plane
	 * @param _eye a WP_point3D object representing the eye and therefore the position of the camera
	 * @param _look a WP_Point3D object representing the point at which the camera is looking
	 * @param _up a WP_Vector3D object representing the upward direction of the camera
	 */
	void setFrustumAndCamera(scalar _viewAngle, scalar _aspectRatio, scalar _nearPlane, scalar _farPlane, 
				 const WP_Point3D& _eye, const WP_Point3D& _look, const WP_Vector3D& _up);

	/**
	 * this function sets a viewing volume with the according perspective projection which is used for object picking. See OpenGL's red book about object picking
	 * @param width the width of the view volume in pixels
	 * @param height the height of the view volume in pixels
	 * @param x the x position of the picking device (e.g. mouse)
	 * @param y the y position of the picking device. Note that this y position is in normal window coordinates, so y = 0 is in the top area of the application window
	 */
	void setPickingVolume(int width, int height, int x, int y);
	
	/**
	 * this function sets the framebuffer render volume but it only has to be called when object picking is needed and you want to switch back from the selection buffer viewing volume to the normal framebuffer rendering view volume. This call has therefor only effect when currently the picking view volume is defined.
	 */
	void setRenderVolume();
	/**
	 * this function creates a WP_Ray3D object which represents a 3D ray starting at the eye of the camera's and passing through pixel x y on the nearplane N. See F.S. Hill, Computer Graphics using OpenGL, page 743/735
	 * @param x the x position of the pixel on the nearplane through which the ray passes
	 * @param y the y position of the pixel on the nearplane through which the ray passes
	 * @return a WP_Ray3D object representing the ray which starts at the eye of the camera and passes through pixels x y on the nearplane N
	 */
	WP_Ray3D createRayForTracing(int x, int y) const; 

	/**
	 * a WP_Point3D object representing the eye (position) of the camera
	 */
	WP_Point3D eye;

	/**
	 * a WP_Point3D object representing the point at which the camera is looking
	 */
	WP_Point3D look;

	/**
	 * a WP_Vector3D object representing the upward direction of the camera
	 */
	WP_Vector3D up;

	/**
	 * a WP_Matrix3D object representing the camera's camera matrix which will be loaded to OpenGL to transform objects from world space to camera space
	 */
	WP_Matrix3D matrix;

	/**
	 * a pointer to a WP_Object, if this object points to WP_Object, the camera is attached to this object and will follow it
	 */
	const WP_Object* fixed_object;

	/**
	 * if the camera is attached to an object, this represents the distance the camera keeps between itself and the object
	 */
	scalar follow_distance;

	/**
	 * if the camera is attached to an object, this represents the rotation in degrees about the object's x-axis
	 */
	int follow_angleX;

	/**
	 * if the camera is attached to an object, this represents the rotation in degrees about the object's y-axis
	 */
	int follow_angleY;

	/**
	 * if the camera is attached to an object, this represents the rotation in degrees about the object's z-axis
	 */
	int follow_angleZ;

	/**
	 * this variable keeps track of the object meshes currently in the camera's viewing volume (frustum)
	 */
	int meshes_in_frustum;

	/**
	 * this variable describes the screen width and thus the width of the viewport
	 */
	int screen_width;

	/**
	 * this variable describes the screen height and thus the height of the viewport
	 */
	int screen_height;

	/**
	 * this boolean is used for telling if the current viewing volume is defined for selection or framebuffer rendering
	 */
	bool normal_viewing_volume;

	Plane* getFrustum() 
	  {
	    return frustum;
	  }

private:
	WP_Camera();

	/**
	 * a WP_Vector3D object representing the direction of the x-axis of the camera's coordinate system
	 */
	WP_Vector3D u;

	/**
	 * a WP_Vector3D object representing the direction of the y-axis of the camera's coordinate system
	 */
	WP_Vector3D v;

	/**
	 * a WP_Vector3D object representing the direction of the z-axis of the camera's coordinate system
	 */
	WP_Vector3D n;
	
	/**
	 * the angle between the top and bottom walls of the view volume (frustum)
	 */
	scalar viewAngle;
	
	/**
	 * the aspect ratio of any window parallel to the xy-plane
	 */
	scalar aspectRatio;

	/**
	 * the distance from the eye to the near plane 
	 */
	scalar nearPlane;

	/**
	 * the distance from the eye to the far plane 
	 */
	scalar farPlane;

	/**
	 * a 2D array of 6 rows of 4 scalars holding the a, b, c and d components of the 6 frustum's plane, where a, b, and c define the plane's normal and d the distance of the plane from the origin 
	 */
	Plane frustum[6];

	/**
	 * a pointer to the only instance of this object
	 */
	static WP_Camera* _instance;
	
	/**
	 * this function loads the camera's modelview matrix into OpenGL
	 */
	void setModelViewMatrixGL();	

	/**
	 * this function computes the six frustum planes after each change in orientation of the camera's view volume (frustum)
	 */
	void computeFrustum();

	/**
	 * a pointer to a WP_Math object used to obtain fast sin and cos values
	 */
	WP_Math* math;

	/**
	 * a pointer to a WP_GLState object used to change OpenGL's internal state machine
	 */
	WP_GLState* state;
};
#endif

