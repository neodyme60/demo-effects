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

#include <GL/gl.h>
//#include "lib3ds/mesh.h"
//#include "lib3ds/material.h"
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
  WP_Model(const string& name, const WP_Vector3D& scaling): model_name(name), number_meshes(0), meshes(0), number_blended_meshes(0), blendedMeshes(0)
    {
      scaling_matrix = new WP_Matrix3D(SCALING_MATRIX, scaling.data[0], scaling.data[1], scaling.data[2]);
    };
  
  virtual ~WP_Model();
  
  /**
   * this function draws the model according to its world matrix into the 3D scene but only if its bounding sphere is in the viewing frustum
   * @param matrix a WP_Matrix3D object representing the world matrix of the model indicating where and how the model is rendered into the scene
   */
  void drawOpenGL(const WP_Matrix3D& matrix) const;
  
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
   * the number of objects using this model, this is used by the WP_ObjectManager object. The model is only discarted by the object manager if this value is zero
   */
  int count;

 protected:
  /**
   * this nested class represents a 3D mesh. Currently the meshes are stored in OpenGL display lists\n
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
  class WP_Mesh
    {
    public:
      WP_Mesh();
      virtual ~WP_Mesh();
      
      /**
       * this function inits and creates the mesh
       * @param _name the name of the mesh
       * @param _numberVertices the number of vertices in the mesh
       * @param _vertices a pointer to an array of <i>_numberVertices</i> WP_Vertex objects representing the mesh its vertices
       * @param _numberFaces the number of faces (triangles) in the mesh
       * @param _faces a pointer to an array of <i>_numberFaces</i> WP_Triangle objects representing the faces (triangles) in the mesh
       * @param _tex_id the OpenGL texture ID of the texture belonging to this mesh
       * @param _material a WP_Material object holding the OpenGL material values of this mesh
       * @param scaling_matrix a pointer to a WP_Vector3D object representing the mesh its scaling matrix (used for scaling of bounding sphere and in the future its collision hull)
       * @param compute_vertex_normals a boolean indicating if this function should compute the vertex normals or not. Default is true. 
       * @param create_display_list a boolean indicating if this function should create a display list or not. Default is true. 
       */
      void init_mesh(const char* _name, int _numberVertices, WP_Vertex* _vertices, int _numberFaces, WP_Triangle* _faces,
		     int _tex_id, const WP_Material& _material, WP_Matrix3D* scaling_matrix, bool compute_vertex_normals = true, 
		     bool create_display_list = true);

      /**
       * this function computes the per vertex normal for all vertices
       */
      void computeVertexNormals();

      /**
       * this function computes the bounding sphere of the mesh for a quick checking if the mesh is in the viewing frustum
       * @param scaling_matrix a pointer to a WP_Vector3D object representing the mesh its scaling matrix (used for scaling of bounding sphere and collision hull)
       */
      void computeBoundingSphere(WP_Matrix3D* scaling_matrix);

      /**
       * this function creates a display list of the vertices and faces in the mesh which is used for rendering with OpenGL
       */
      void createDisplayList();

      /**
       * this function draws the mesh in OpenGL by calling its display list
       */
      void drawOpenGL();
      
      /**
       * the OpenGL texture ID of the texture belonging to this mesh
       */
      GLint tex_id;

      /**
       * a WP_Point3D object holding the position of the center of the mesh, this is used for the definition of a bounding sphere to determine if the mesh is in the viewing frustum or not
       */
      WP_Point3D center;

      /**
       * the radius of the bounding sphere of the mesh
       */
      scalar radius;
      
      /**
       * this function computes the maximum values of the vertices in the mesh. This to determine the size of the mesh its bounding sphere
       * @param scaling_matrix a pointer to a WP_Vector3D object representing the mesh its scaling matrix (used for scaling of bounding sphere and in the future its collision hull)
       * @return a WP_Point3D object holding the maximum values of the mesh
       */
      WP_Point3D getMaxPoint(WP_Matrix3D* scaling_matrix) const;

      /**
       * this function computes the minimum values of the vertices in the mesh. This to determine the size of the mesh its bounding sphere
       * @param scaling_matrix a pointer to a WP_Vector3D object representing the mesh its scaling matrix (used for scaling of bounding sphere and in the future its collision hull)
       * @return a WP_Point3D object holding the minimum values of the mesh
       */
      WP_Point3D getMinPoint(WP_Matrix3D* scaling_matrix) const;
      
      /**
       * the number of vertices in the mesh
       */
      int numberVertices;

      /**
       * the number of faces (triangles) in the mesh
       */
      int numberFaces;
      
      /**
       * an array of <i>numberVertices</i> WP_Vertex objects representing the vertices of the mesh
       */
      WP_Vertex* vertices;

      /**
       * an array of <i>numberFaces</i> WP_Triangle objects representing the faces of the mesh
       */
      WP_Triangle* faces;

      /**
       * the OpenGL display list ID of the mesh
       */
      GLuint displayID;

      /**
       * a WP_Material object holding the OpenGL material of the mesh
       */
      WP_Material material;

      /**
       * the name of the mesh
       */
      char name[64];
    };
  
  /**
   * this function compares the texture ID of two textures. This is used to sort the meshes and blendedMeshes arrays
   * @param mesh1 a void pointer to the first mesh
   * @param mesh2 a void pointer to the second mesh
   * @return -1 if the texture ID of mesh1 < mesh2, 1 if the texture ID of mesh1 > mesh2, 0 otherwise
   */
  static int textureCompareSmallToBig(const void* mesh1, const void* mesh2);

  /**
   * this function compares the texture ID of two textures. This is used to sort the meshes and blendedMeshes arrays
   * @param mesh1 a void pointer to the first mesh
   * @param mesh2 a void pointer to the second mesh
   * @return -1 if the texture ID of mesh1 > mesh2, 1 if the texture ID of mesh1 < mesh2, 0 otherwise
   */
  static int textureCompareBigToSmall(const void* mesh1, const void* mesh2);
  
  /**
   * this function finalizes all after the model was read and everything was initialized. The textures are sorted and the model bounding sphere is created
   */
  bool finalizeAll();

  /**
   * the number of meshes in the model
   */
  int number_meshes;

  /**
   * a pointer to an array of <i>number_meshes</i> WP_Mesh objects holding the mesh data
   */
  WP_Mesh* meshes;

  /**
   * the number of blended meshes in the model
   */
  int number_blended_meshes;

  /**
   * a pointer to an array of <i>number_blended_meshes</i> WP_Mesh objects holding the blended mesh data
   */
  WP_Mesh* blendedMeshes;

  /**
   * this function computes the maximum values of the vertices in the mesh. This to determine the size of the model its bounding sphere
   * @return a WP_Point3D object holding the maximum values of the model
   */
  WP_Point3D getMaxPoint() const;
  
  /**
   * this function computes the maximum values of the vertices in the mesh. This to determine the size of the model its bounding sphere
   * @return a WP_Point3D object holding the minimum values of the model
   */
  WP_Point3D getMinPoint() const;
};

///////////////////////////////////////////////////////////////////////////////////////////

/**
 * this class is used for the creation of models stored in Autodesk's .3DS file format\n
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

//class WP_Model_3DS: public WP_Model
//{
// public:
//  WP_Model_3DS(){}; 

//  /**
// * @param name the name of the file containing the .3ds model
// * @param scaling a WP_Vector3D object holding values by which the model is scaled
// */
//WP_Model_3DS(const string& name, const WP_Vector3D& scaling);
//
//virtual ~WP_Model_3DS(){};
//
///**
// * this function is used for reading the model file and initializing the model by filling the variables of the base class with the appropriate read values. This function is automaticly called by the base class WP_Model by a call to its <i>init</i> function
// */
//bool initModel();
//};


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

