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

#ifndef WP_MODEL_H
#define WP_MODEL_H

#include <string>
using namespace std;
#include <list>

#include "WPCG.h"

/**
 * this abstract class represents a 3D model which is used in WP_Object to composite a 3D entity. The model is only the 3D shape, its textures, its bounding sphere for frustum culling and in the future its bounding hull for collision detection. A WP_Object contains far more, it can contain for instance among others mass, heading, velocity etc etc\n
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
class WP_Model
{
 public:
  WP_Model(){};
  
  /**
   * @param the name of the to be loaded model
   * @param scaling a WP_Vector3D object holding values by which the model is scaled
   */
  WP_Model(const string& name, const WP_Vector3D& scaling): model_name(name), numberFrames(0), currentFrame(0), interpolate(0.0)
    {
      scaling_matrix = new WP_Matrix3D(SCALING_MATRIX, scaling.data[0], scaling.data[1], scaling.data[2]);
    };
  
  virtual ~WP_Model();
  
  /**
   * this function draws the model according to its world matrix into the 3D scene but only if its bounding sphere is in the viewing frustum
   * @param matrix a WP_Matrix3D object representing the world matrix of the model indicating where and how the model is rendered into the scene
   */
  void drawOpenGL(const WP_Matrix3D& matrix);
  
  /**
   * this virtual function should be overriden by child objects. In this function the model must be read and the model's internals must be filled (like for example the meshes, the material etc). Every 3d file format stores this information in a different way there this function can be used to substract the necessary data from it.
   */
  virtual bool initModel() = 0;
  
  /**
   * this function must be called to init the model. This is based on the Template design pattern.
   */
  bool init();

  /**
   * a WP_Matrix3D object representing the scaling matrix by which the model is scaled (used for correct scaling of bounding sphere and collision hull)
   */
  WP_Matrix3D* scaling_matrix;

  /**
   * a WP_Point3D object holding the position of the center of the model, this is used for the definition of a bounding sphere to determine if the model is in the viewing frustum or not
   */
  WP_Point3D center;
  
  /**
   * the radius of the bounding sphere of the model
   */
  scalar radius;

  /**
   * the name of the model
   */
  string model_name;
  
  /**
   * the number of objects using this model, this is used by the WP_ObjectManager object. The model is only discarted by the object manager if this value is zero. This is a so called reference count
   */
  int count;

  /**
   * the OpenGL texture ID of the texture belonging to this model
   */
  GLint tex_id;

 protected:

  /**
   * this nested class represents a frame. 
   * @author Copyright (C) 2002 W.P. van Paassen   peter@paassen.tmfweb.nl
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
  class WP_Frame
    {
    public:
      WP_Frame():numberVertices(0), vertices(0){};
      virtual ~WP_Frame()
	{
	  delete[] vertices;
	}

      /**
       * the number of vertices in the frame
       */
      int numberVertices;

      /**
       * an array of <i>numberVertices</i> WP_Vertex objects representing the vertices of the frame
       */
      WP_Vertex* vertices;

      /**
       * a WP_Material object holding the OpenGL material of the frame
       */
      WP_Material material;

      /**
       * the name of the frame
       */
      char name[64];

      /**
       * this function computes the maximum values of the vertices in the frame. This to determine the size of the model its bounding sphere
       * @return a WP_Point3D object holding the maximum values of the model
       */
      WP_Point3D getMaxPoint() const;
  
      /**
       * this function computes the maximum values of the vertices in the frame. This to determine the size of the model its bounding sphere
       * @return a WP_Point3D object holding the minimum values of the model
       */
      WP_Point3D getMinPoint() const;

      /**
       * this function computes the bounding sphere of the frame for a quick checking if the frame is in the viewing frustum
       * @param scaling_matrix a pointer to a WP_Vector3D object representing the frame its scaling matrix (used for scaling of bounding sphere and in the future for the collision hull)
       */
      void computeBoundingSphere(WP_Matrix3D* scaling_matrix);

      /**
       * a WP_Point3D object holding the position of the center of the frame, this is used for the definition of a bounding sphere to determine if the frame is in the viewing frustum or not
       */
      WP_Point3D center;

      /**
       * the radius of the bounding sphere of the frame
       */
      scalar radius;

    };

  /**
   * this function finalizes all after the model was read and everything was initialized. The textures are sorted and the model bounding sphere is created
   */
  bool finalizeAll();

  scalar interpolate;
  list<WP_TriangleGroup*> triangle_groups;
  WP_Frame *frames;
  unsigned int numberFrames;
  unsigned int currentFrame;
};

////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * this class is used for the creation of models in ID's MD2 file format, as used in the computer game Quake2\n
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
class WP_Model_MD2: public WP_Model
{
 public:
  WP_Model_MD2(){}; 

  /**
   * @param name the name of the file containing the .md2 model
   * @param scaling a WP_Vector3D object holding values by which the model is scaled
   */
  WP_Model_MD2(const string& name, const WP_Vector3D& scaling);

  virtual ~WP_Model_MD2(){};
  
  /**
   * this function is used for reading the model file and initializing the model by filling the variables of the base class with the appropriate read values. This function is automaticly called by the base class WP_Model by a call to its <i>init</i> function
   */
  bool initModel();

 protected:

  // nested class WP_MD2_HEADER 
  class WP_MD2_HEADER
  {
  public:
    int magic; 
    int version; 
    int skinWidth; 
    int skinHeight; 
    int frameSize; 
    int numSkins; 
    int numVertices; 
    int numTexCoords; 
    int numTriangles; 
    int numGlCommands; 
    int numFrames; 
    int offsetSkins; 
    int offsetTexCoords; 
    int offsetTriangles; 
    int offsetFrames; 
    int offsetGlCommands; 
    int offsetEnd; 
  }; 
};

#endif

