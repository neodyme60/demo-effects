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

#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <values.h>
#include <iostream.h>
#include <fstream.h>
#include <GL/gl.h>
//#include <GL/glut.h>
//#include "lib3ds/file.h"
#include "WP_TextureManager.h"
#include "WP_Camera.h"
#include "WP_Model.h"

/////////////////////////////WP_Model//////////////////////////////////

WP_Model::~WP_Model()
{
  if (meshes)
    {
      delete[] meshes;
    }
  
  if (blendedMeshes)
    {
      delete[] blendedMeshes;
    }

  if (scaling_matrix)
    {
      delete scaling_matrix;
    }

  WP_TextureManager::getInstance()->removeTextures(this);
}

bool WP_Model::init()
{
  if (!initModel())
    {
      return false;
    }
  
  return finalizeAll();
}

bool WP_Model::finalizeAll()
{
  try
    {
      //quicksort textures in normal mesh and blended mesh arrays
      //mesh array is sorted from no textures to textures and the blended mesh array from textures to no textures in this way the states 
      //can flow over in each other avoiding a texture2D state change
      
      qsort((void*)meshes, number_meshes, sizeof(WP_Mesh), &textureCompareBigToSmall);
      qsort((void*)blendedMeshes, number_blended_meshes, sizeof(WP_Mesh), &textureCompareSmallToBig);
      
      //set center and compute radius of sphere
      
      WP_Point3D _max = getMaxPoint();
      WP_Point3D _min = getMinPoint();
      
      scalar x_size = _max.data[0] - _min.data[0];
      scalar y_size = _max.data[1] - _min.data[1];
      scalar z_size = _max.data[2] - _min.data[2];
      
      if (x_size >= y_size && x_size >= z_size)
	{
	  radius = (x_size / 2.0) * 1.35;
	}
      else if (y_size >= x_size && y_size >= z_size)
	{
	  radius = (y_size / 2.0) * 1.35;
	}
      else
	{
	  radius =  (z_size / 2.0) * 1.35;
	}
      
      center.data[0] = _min.data[0] + ((_max.data[0] - _min.data[0]) / 2.0);
      center.data[1] = _min.data[1] + ((_max.data[1] - _min.data[1]) / 2.0);
      center.data[2] = _min.data[2] + ((_max.data[2] - _min.data[2]) / 2.0);
      return true;
    }
  catch(...)
    {
      return false;
    }
}

void WP_Model::drawOpenGL(const WP_Matrix3D& matrix) const
{
  glPushMatrix();
  glMultMatrixf(matrix.data); 
  
  int lastText = -1;
	
  //fixme add to WP_Model as static
  WP_GLState* state = WP_GLState::getInstance();
  WP_Camera* cam = WP_Camera::getInstance();

  WP_Point3D c;

  int i;
  for (i = 0; i < number_meshes; i++)
    {
      //multiply meshes[i].center point by matrix to get correct center of mesh
      c= meshes[i].center;
      c *= matrix;

      if (cam->inFrustum(c.data[0], c.data[1], c.data[2],
      			 meshes[i].radius))
      	{
	  //cam->meshes_in_frustum++;
	  
	  if (meshes[i].tex_id != lastText && meshes[i].tex_id != 0)
	    {
	      state->enableTexture2D();
	      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0); //priority of last texture set low because it will not be used anymore
	      glBindTexture(GL_TEXTURE_2D, meshes[i].tex_id);
	      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1); //priority of current texture set high
	      lastText = meshes[i].tex_id;
	    }
	  else if (meshes[i].tex_id == 0)
	    {
	      state->disableTexture2D();
	    }
	  meshes[i].drawOpenGL();
	}
    }
	
  state->enableBlending();
  glDepthMask(false);
  for (i = 0; i < number_blended_meshes; i++)
    {
     //multiply blendedMeshes[i].center point by matrix to get correct center of mesh
      c= blendedMeshes[i].center;
      c *= matrix;

      if (cam->inFrustum(c.data[0], c.data[1], c.data[2],
			 blendedMeshes[i].radius))
	{
	  //	  cam->meshes_in_frustum++;
	  
	  if (blendedMeshes[i].tex_id != lastText && blendedMeshes[i].tex_id != 0)
	    {
	      state->enableTexture2D();
	      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 0); //priority of last texture set low because it will not be used anymore
	      glBindTexture(GL_TEXTURE_2D, blendedMeshes[i].tex_id);
	      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1); //priority of current texture set high
	      lastText = blendedMeshes[i].tex_id;
	    }
	  else if (blendedMeshes[i].tex_id == 0)
	    {
	      state->disableTexture2D();
	    }
	  blendedMeshes[i].drawOpenGL();
	}
    }
  state->disableBlending();
  glDepthMask(true);
  state->disableTexture2D();
  
  glPopMatrix();
}

int WP_Model::textureCompareSmallToBig(const void* mesh1, const void* mesh2)
{
	if (((WP_Mesh*)mesh1)->tex_id < ((WP_Mesh*)mesh2)->tex_id)
		return -1;
	else if (((WP_Mesh*)mesh1)->tex_id > ((WP_Mesh*)mesh2)->tex_id)
		return 1;
	return 0; 
}

int WP_Model::textureCompareBigToSmall(const void* mesh1, const void* mesh2)
{
	if (((WP_Mesh*)mesh1)->tex_id > ((WP_Mesh*)mesh2)->tex_id)
		return -1;
	else if (((WP_Mesh*)mesh1)->tex_id < ((WP_Mesh*)mesh2)->tex_id)
		return 1;
	return 0; 
}

WP_Point3D WP_Model::getMaxPoint() const
{
  WP_Point3D _max(MINFLOAT, MINFLOAT, MINFLOAT);
  WP_Point3D p;

  int j = 0;
  
  for (; j < number_blended_meshes; j++)
    {
      p = blendedMeshes[j].getMaxPoint(scaling_matrix);

      if (p.data[0] > _max.data[0])
	{
	  _max.data[0] = p.data[0];
	}
			      
      if (p.data[1] > _max.data[1])
	{
	  _max.data[1] = p.data[1];
	} 
      
      if (p.data[2] > _max.data[2])
	{
	  _max.data[2] = p.data[2];
	}
    }

  j = 0;
  
  for(; j < number_meshes; j++)
    {
      p = meshes[j].getMaxPoint(scaling_matrix);

      if (p.data[0] > _max.data[0])
	{
	  _max.data[0] = p.data[0];
	}
			      
      if (p.data[1] > _max.data[1])
	{
	  _max.data[1] = p.data[1];
	} 
			      
      if (p.data[2] > _max.data[2])
	{
	  _max.data[2] = p.data[2];
	}
    }

  return _max;
}

WP_Point3D WP_Model::getMinPoint() const
{
  WP_Point3D _min(MAXFLOAT, MAXFLOAT, MAXFLOAT);
  WP_Point3D p;

  int j = 0;
  
  for (; j < number_blended_meshes; j++)
    {
      p = blendedMeshes[j].getMinPoint(scaling_matrix);

      if (p.data[0] < _min.data[0])
	{
	  _min.data[0] = p.data[0];
	}
      
      if (p.data[1] < _min.data[1])
	{
	  _min.data[1] = p.data[1];
	} 
			      
      if (p.data[2] < _min.data[2])
	{
	  _min.data[2] = p.data[2];
	}
    }

  j = 0;
  
  for(; j < number_meshes; j++)
    {
      p = meshes[j].getMinPoint(scaling_matrix);

      if (p.data[0] < _min.data[0])
	{
	  _min.data[0] = p.data[0];
	}
			      
      if (p.data[1] < _min.data[1])
	{
	  _min.data[1] = p.data[1];
	} 

      if (p.data[2] < _min.data[2])
	{
	  _min.data[2] = p.data[2];
	}
    }
  
  return _min;
}

///////////////////////////////////// WP_Mesh ////////////////////////

WP_Model::WP_Mesh::WP_Mesh():tex_id(0), numberVertices(0), numberFaces(0), vertices(0),faces(0), displayID(0){}

WP_Model::WP_Mesh::~WP_Mesh()
{
  if (vertices)
    {
      delete[] vertices;
    }
  if (faces)
    {
      delete[] faces;
    }
  
  glDeleteLists(displayID, 1);
}

void WP_Model::WP_Mesh::computeVertexNormals()
{
  //compute vertex normals
  int i,j;
  
  for (i = 0; i < numberVertices; i++)
    {
      WP_Vector3D normal_sum;
      for(j = 0; j < numberFaces; j++)
	{
	  WP_Vertex** v = faces[j].vertices;
	  if (v[0] == vertices + i || v[1] == vertices + i || v[2] == vertices + i)
	    {
	      normal_sum += faces[j].normal;
	    }
	}
      normal_sum.normalize();
      vertices[i].normal = normal_sum;
    }
}

void WP_Model::WP_Mesh::computeBoundingSphere(WP_Matrix3D* scaling_matrix)
{
  //set center and determine sphere radius
  
  WP_Point3D _max = getMaxPoint(scaling_matrix);
  WP_Point3D _min = getMinPoint(scaling_matrix);
  
  center.data[0] = _min.data[0] + ((_max.data[0] - _min.data[0]) / 2.0);
  center.data[1] = _min.data[1] + ((_max.data[1] - _min.data[1]) / 2.0);
  center.data[2] = _min.data[2] + ((_max.data[2] - _min.data[2]) / 2.0);
  
  scalar x_size = _max.data[0] - _min.data[0];
  scalar y_size = _max.data[1] - _min.data[1];
  scalar z_size = _max.data[2] - _min.data[2];
  
  if (x_size >= y_size && x_size >= z_size)
    {
      radius = (x_size / 2.0) * 1.35;
    }
  else if (y_size >= x_size && y_size >= z_size)
    {
      radius = (y_size / 2.0) * 1.35;
    }
  else
    {
      radius = (z_size / 2.0) * 1.35;
    }
}

void WP_Model::WP_Mesh::createDisplayList()
{
  //create display list
  int i;

  displayID = glGenLists(1);
  glNewList(displayID, GL_COMPILE);
  glPushMatrix();

  glCullFace(GL_BACK);
  material.initMaterialGL();
  glBegin(GL_TRIANGLES);
  for (i = 0; i < numberFaces; i++)
    {
      faces[i].drawOpenGL();
    }
  glEnd();
  glPopMatrix();
  glEndList();
}

void WP_Model::WP_Mesh::init_mesh(const char* _name, int _numberVertices, WP_Vertex* _vertices, int _numberFaces, WP_Triangle* _faces, int _tex_id, 
				  const WP_Material& _material, WP_Matrix3D* scaling_matrix, bool compute_vertex_normals = true, 
				  bool create_display_list = true)
{
  sprintf(name, "%s", _name);
  numberVertices = _numberVertices;
  vertices = _vertices;
  numberFaces = _numberFaces;
  faces = _faces;
  tex_id = _tex_id;
  material = _material;
  
  if (compute_vertex_normals)
    {
      computeVertexNormals();
    }

  computeBoundingSphere(scaling_matrix);

  if (create_display_list)
    {
      createDisplayList();
    }
}

void WP_Model::WP_Mesh::drawOpenGL()
{
  //glTranslatef(center.data[0], center.data[1],center.data[2]);
  //glutWireSphere(radius, 15, 15);
  //glTranslatef(-center.data[0], -center.data[1], -center.data[2]);
  glCallList(displayID);
}

WP_Point3D WP_Model::WP_Mesh::getMaxPoint(WP_Matrix3D* scaling_matrix) const
{
  WP_Point3D _max(MINFLOAT, MINFLOAT, MINFLOAT);
  WP_Point3D p;
  int i = 0;
  
  if (!scaling_matrix->isIdentity())
    {
      //determine max point
      for (; i < numberVertices; i++)
	{
	  p = vertices[i].point;
	  p *= *scaling_matrix;
	  
	  if (p.data[0] > _max.data[0])
	    {
	      _max.data[0] = p.data[0];
	    }
	  if (p.data[1] > _max.data[1])
	    {
	      _max.data[1] = p.data[1];
	    }
	  if (p.data[2] > _max.data[2])
	    {
	      _max.data[2] = p.data[2];
	    }	
	}
    }
  else
    {
      //determine max point
      for (; i < numberVertices; i++)
	{
	  p = vertices[i].point;
	  
	  if (p.data[0] > _max.data[0])
	    {
	      _max.data[0] = p.data[0];
	    }
	  if (p.data[1] > _max.data[1])
	    {
	      _max.data[1] = p.data[1];
	    }
	  if (p.data[2] > _max.data[2])
	    {
	      _max.data[2] = p.data[2];
	    }	
	}
    }
  
  return _max;
}

WP_Point3D WP_Model::WP_Mesh::getMinPoint(WP_Matrix3D* scaling_matrix) const
{
  WP_Point3D _min(MAXFLOAT, MAXFLOAT, MAXFLOAT);
  WP_Point3D p;
  int i = 0;

  if (!scaling_matrix->isIdentity())
    {
      //determine min point
      for (; i < numberVertices; i++)
	{
	  p = vertices[i].point;
	  p *= *scaling_matrix;
	  
	  if (p.data[0] < _min.data[0])
	    {
	      _min.data[0] = p.data[0];
	    }
	  if (p.data[1] < _min.data[1])
	    {
	      _min.data[1] = p.data[1];
	    }
	  if (p.data[2] < _min.data[2])
	    {
	      _min.data[2] = p.data[2];
	    }
	}
    }
  else
    {
      //determine min point
      for (; i < numberVertices; i++)
	{
	  p = vertices[i].point;
	  
	  if (p.data[0] < _min.data[0])
	    {
	      _min.data[0] = p.data[0];
	    }
	  if (p.data[1] < _min.data[1])
	    {
	      _min.data[1] = p.data[1];
	    }
	  if (p.data[2] < _min.data[2])
	    {
	      _min.data[2] = p.data[2];
	    }
	}
    }

  return _min;
}

/////////////////////////// WP_Model_3DS //////////////////////////////////////////
// Autodesk 3DS file format loader

/*WP_Model_3DS::WP_Model_3DS(const string& name, const WP_Vector3D& scaling): WP_Model(name, scaling){}

bool WP_Model_3DS::initModel()
{
  number_meshes = 0;
  number_blended_meshes = 0;
  meshes = 0;
  blendedMeshes = 0;
  
  Lib3dsFile *f = 0;
  
  try
    {
      f=lib3ds_file_load(model_name.c_str());
      if (!f)
	{
	  char buf[256];
	  sprintf(buf, "WP_MODEL_3DS::initModel - ERROR: initing model %s failed", model_name.c_str());
	  throw(buf);
	}
      
      //  model_name = name;
      count = 1;
      Lib3dsMesh* m;
      Lib3dsMaterial* material;
      
      //read meshes and determine number of meshes
      //determine number of blended and normal meshes
      
      for (m=f->meshes; m; m=m->next)
	{
	  number_meshes++;
	  
	  material = f->materials;
	  
	  //find correct material to identify blended or normal mesh
	  for (; material; material = material->next)
	    {
	      if (m->faceL)
		{
		  if (strcmp(material->name, m->faceL->material) == 0)
		    {
		      if (material->transparency > 0.0)
			{
			  number_blended_meshes++;
			  break;
			}
		    }
		}
	    }
	}

      //create #number_meshes - #number_blended_meshes WP_Mesh_3DS objects
      number_meshes -= number_blended_meshes;
      if (number_meshes > 0)
	meshes = new WP_Mesh[number_meshes];
      if (number_blended_meshes > 0)
	blendedMeshes = new WP_Mesh[number_blended_meshes];
      
      int i = 0;
      int j = 0;
      
      //init meshes
      
      i = 0;
      j = 0;
      
      for (m=f->meshes; m; m=m->next)
	{
	  material = f->materials;
	  //find correct material to identify blended or normal mesh
	    for (; material; material = material->next)
	      {
		if (m->faceL)
		  {
		    if (strcmp(material->name, m->faceL->material) == 0)
		      {
			unsigned int k;
			int tex_id = 0;
			
			Lib3dsMaterial* mat;
			WP_Material _material;
			
			//vertices
			WP_Vertex* vertices = new WP_Vertex[m->points];
			
			//set vertices and determine min/max point
			
			for (k = 0; k < m->points; k++)
			  {
			    vertices[k].point = WP_Point3D(m->pointL[k].pos[0], m->pointL[k].pos[1], m->pointL[k].pos[2]);
			  }
			
			//set texels
			for (k = 0; k < m->texels; k++)
			  {
			    vertices[k].texCoords[0] = m->texelL[k][0];
			    vertices[k].texCoords[1] = m->texelL[k][1];
			  }
			
			//faces (WP_Triangle)
			WP_Triangle* faces = new WP_Triangle[m->faces];
			
			//set material
			//find material in material array of .3ds file
			WP_TextureManager* tex_man = WP_TextureManager::getInstance();
			
			for ( mat = material; mat; mat =mat->next)
			  {
			    //here the assumption was made that every mesh has only one texture and therefore all the faces in the mesh are of the same material
			    //therefore m->faceL[0].material is checked, only the material of the first face is hence checked
			    if (m->faceL)
			      {
				if (strcmp(mat->name, m->faceL[0].material) == 0)
				  {
				    //set texture_id if texture exists
				    if (m->texels > 0 && strcmp(mat->texture1_map.name, "") != 0)
				      {
					tex_id = tex_man->getTexture(mat->texture1_map.name, this);
				      }
				    
				    //set material
				    float* rgba = mat->ambient;
				    _material.ambient = WP_Color(rgba[0], rgba[1], rgba[2], rgba[3]);
				    rgba = mat->diffuse;
				    _material.diffuse = WP_Color(rgba[0], rgba[1], rgba[2], rgba[3]);
				    rgba = mat->specular;
				    _material.specular = WP_Color(1.0 - rgba[0], 1.0 - rgba[1], 1.0 - rgba[2], rgba[3]);
				    _material.specularExponent = mat->shininess;
				    _material.setTransparency(mat->transparency);
				    break;
				  }
			      }
			  }
			
			for (k = 0; k < m->faces; k++)
			  {
			    //set vertices
			    faces[k].vertices[0] = &vertices[m->faceL[k].points[0]];
			    faces[k].vertices[1] = &vertices[m->faceL[k].points[1]];
			    faces[k].vertices[2] = &vertices[m->faceL[k].points[2]];
			    
			    //set normal
			    faces[k].normal = WP_Vector3D(m->faceL[k].normal[0],m->faceL[k].normal[1], m->faceL[k].normal[2]);
			  }
			if (material->transparency > 0.0)
			  {
			    blendedMeshes[j].init_mesh(m->name, (int)m->points, vertices, (int)m->faces, faces, tex_id, _material, scaling_matrix);
			    j++;
			  }
			else
			  {
			    meshes[i].init_mesh(m->name, (int)m->points, vertices, (int)m->faces, faces, tex_id, _material, scaling_matrix);
			    i++;
			  }
		      }
		  }
		else
		  {
		    char buf[256];
		    if (f)
		      {
			lib3ds_file_free(f);
		      }
		    sprintf(buf, "WP_MODEL_3DS::initModel - ERROR: initing model %s failed", model_name.c_str());
		    throw(buf);
		  }
	      }
	}
	lib3ds_file_free(f);
	return true;
    }
  catch(char* s)
    {
      cerr << s << endl;
      return false;
    }
  catch(...)
    {
      return false;
    }
}
*/
/////////////////////////// WP_Model_MD2 //////////////////////////////////////////
// Quake 2 file format model loader

    WP_Vector3D quake2_normals[] = 
    {
      WP_Vector3D(-0.525731, 0.000000, 0.850651), 
      WP_Vector3D(-0.442863, 0.238856, 0.864188),
      WP_Vector3D(-0.295242, 0.000000, 0.955423), 
      WP_Vector3D(-0.309017, 0.500000, 0.809017), 
      WP_Vector3D(-0.162460, 0.262866, 0.951056), 
      WP_Vector3D(0.000000, 0.000000, 1.000000), 
      WP_Vector3D(0.000000, 0.850651, 0.525731), 
      WP_Vector3D(-0.147621, 0.716567, 0.681718), 
      WP_Vector3D(0.147621, 0.716567, 0.681718), 
      WP_Vector3D(0.000000, 0.525731, 0.850651), 
      WP_Vector3D(0.309017, 0.500000, 0.809017), 
      WP_Vector3D(0.525731, 0.000000, 0.850651), 
      WP_Vector3D(0.295242, 0.000000, 0.955423), 
      WP_Vector3D(0.442863, 0.238856, 0.864188), 
      WP_Vector3D(0.162460, 0.262866, 0.951056), 
      WP_Vector3D(-0.681718, 0.147621, 0.716567), 
      WP_Vector3D(-0.809017, 0.309017, 0.500000), 
      WP_Vector3D(-0.587785, 0.425325, 0.688191), 
      WP_Vector3D(-0.850651, 0.525731, 0.000000), 
      WP_Vector3D(-0.864188, 0.442863, 0.238856), 
      WP_Vector3D(-0.716567, 0.681718, 0.147621), 
      WP_Vector3D(-0.688191, 0.587785, 0.425325), 
      WP_Vector3D(-0.500000, 0.809017, 0.309017), 
      WP_Vector3D(-0.238856, 0.864188, 0.442863), 
      WP_Vector3D(-0.425325, 0.688191, 0.587785), 
      WP_Vector3D(-0.716567, 0.681718, -0.147621), 
      WP_Vector3D(-0.500000, 0.809017, -0.309017), 
      WP_Vector3D(-0.525731, 0.850651, 0.000000), 
      WP_Vector3D(0.000000, 0.850651, -0.525731), 
      WP_Vector3D(-0.238856, 0.864188, -0.442863), 
      WP_Vector3D(0.000000, 0.955423, -0.295242), 
      WP_Vector3D(-0.262866, 0.951056, -0.162460), 
      WP_Vector3D(0.000000, 1.000000, 0.000000), 
      WP_Vector3D(0.000000, 0.955423, 0.295242), 
      WP_Vector3D(-0.262866, 0.951056, 0.162460), 
      WP_Vector3D(0.238856, 0.864188, 0.442863), 
      WP_Vector3D(0.262866, 0.951056, 0.162460), 
      WP_Vector3D(0.500000, 0.809017, 0.309017), 
      WP_Vector3D(0.238856, 0.864188, -0.442863), 
      WP_Vector3D(0.262866, 0.951056, -0.162460), 
      WP_Vector3D(0.500000, 0.809017, -0.309017), 
      WP_Vector3D(0.850651, 0.525731, 0.000000), 
      WP_Vector3D(0.716567, 0.681718, 0.147621), 
      WP_Vector3D(0.716567, 0.681718, -0.147621), 
      WP_Vector3D(0.525731, 0.850651, 0.000000), 
      WP_Vector3D(0.425325, 0.688191, 0.587785), 
      WP_Vector3D(0.864188, 0.442863, 0.238856), 
      WP_Vector3D(0.688191, 0.587785, 0.425325), 
      WP_Vector3D(0.809017, 0.309017, 0.500000), 
      WP_Vector3D(0.681718, 0.147621, 0.716567), 
      WP_Vector3D(0.587785, 0.425325, 0.688191), 
      WP_Vector3D(0.955423, 0.295242, 0.000000), 
      WP_Vector3D(1.000000, 0.000000, 0.000000), 
      WP_Vector3D(0.951056, 0.162460, 0.262866), 
      WP_Vector3D(0.850651, -0.525731, 0.000000), 
      WP_Vector3D(0.955423, -0.295242, 0.000000), 
      WP_Vector3D(0.864188, -0.442863, 0.238856), 
      WP_Vector3D(0.951056, -0.162460, 0.262866), 
      WP_Vector3D(0.809017, -0.309017, 0.500000), 
      WP_Vector3D(0.681718, -0.147621, 0.716567), 
      WP_Vector3D(0.850651, 0.000000, 0.525731), 
      WP_Vector3D(0.864188, 0.442863, -0.238856), 
      WP_Vector3D(0.809017, 0.309017, -0.500000), 
      WP_Vector3D(0.951056, 0.162460, -0.262866), 
      WP_Vector3D(0.525731, 0.000000, -0.850651), 
      WP_Vector3D(0.681718, 0.147621, -0.716567), 
      WP_Vector3D(0.681718, -0.147621, -0.716567), 
      WP_Vector3D(0.850651, 0.000000, -0.525731), 
      WP_Vector3D(0.809017, -0.309017, -0.500000), 
      WP_Vector3D(0.864188, -0.442863, -0.238856), 
      WP_Vector3D(0.951056, -0.162460, -0.262866), 
      WP_Vector3D(0.147621, 0.716567, -0.681718), 
      WP_Vector3D(0.309017, 0.500000, -0.809017), 
      WP_Vector3D(0.425325, 0.688191, -0.587785), 
      WP_Vector3D(0.442863, 0.238856, -0.864188), 
      WP_Vector3D(0.587785, 0.425325, -0.688191), 
      WP_Vector3D(0.688191, 0.587785, -0.425325), 
      WP_Vector3D(-0.147621, 0.716567, -0.681718), 
      WP_Vector3D(-0.309017, 0.500000, -0.809017), 
      WP_Vector3D(0.000000, 0.525731, -0.850651), 
      WP_Vector3D(-0.525731, 0.000000, -0.850651), 
      WP_Vector3D(-0.442863, 0.238856, -0.864188), 
      WP_Vector3D(-0.295242, 0.000000, -0.955423), 
      WP_Vector3D(-0.162460, 0.262866, -0.951056), 
      WP_Vector3D(0.000000, 0.000000, -1.000000), 
      WP_Vector3D(0.295242, 0.000000, -0.955423), 
      WP_Vector3D(0.162460, 0.262866, -0.951056), 
      WP_Vector3D(-0.442863, -0.238856, -0.864188), 
      WP_Vector3D(-0.309017, -0.500000, -0.809017), 
      WP_Vector3D(-0.162460, -0.262866, -0.951056), 
      WP_Vector3D(0.000000, -0.850651, -0.525731), 
      WP_Vector3D(-0.147621, -0.716567, -0.681718), 
      WP_Vector3D(0.147621, -0.716567, -0.681718), 
      WP_Vector3D(0.000000, -0.525731, -0.850651), 
      WP_Vector3D(0.309017, -0.500000, -0.809017), 
      WP_Vector3D(0.442863, -0.238856, -0.864188), 
      WP_Vector3D(0.162460, -0.262866, -0.951056), 
      WP_Vector3D(0.238856, -0.864188, -0.442863), 
      WP_Vector3D(0.500000, -0.809017, -0.309017), 
      WP_Vector3D(0.425325, -0.688191, -0.587785), 
      WP_Vector3D(0.716567, -0.681718, -0.147621), 
      WP_Vector3D(0.688191, -0.587785, -0.425325), 
      WP_Vector3D(0.587785, -0.425325, -0.688191), 
      WP_Vector3D(0.000000, -0.955423, -0.295242), 
      WP_Vector3D(0.000000, -1.000000, 0.000000), 
      WP_Vector3D(0.262866, -0.951056, -0.162460), 
      WP_Vector3D(0.000000, -0.850651, 0.525731), 
      WP_Vector3D(0.000000, -0.955423, 0.295242), 
      WP_Vector3D(0.238856, -0.864188, 0.442863), 
      WP_Vector3D(0.262866, -0.951056, 0.162460), 
      WP_Vector3D(0.500000, -0.809017, 0.309017), 
      WP_Vector3D(0.716567, -0.681718, 0.147621), 
      WP_Vector3D(0.525731, -0.850651, 0.000000), 
      WP_Vector3D(-0.238856, -0.864188, -0.442863), 
      WP_Vector3D(-0.500000, -0.809017, -0.309017), 
      WP_Vector3D(-0.262866, -0.951056, -0.162460), 
      WP_Vector3D(-0.850651, -0.525731, 0.000000), 
      WP_Vector3D(-0.716567, -0.681718, -0.147621), 
      WP_Vector3D(-0.716567, -0.681718, 0.147621), 
      WP_Vector3D(-0.525731, -0.850651, 0.000000), 
      WP_Vector3D(-0.500000, -0.809017, 0.309017), 
      WP_Vector3D(-0.238856, -0.864188, 0.442863), 
      WP_Vector3D(-0.262866, -0.951056, 0.162460), 
      WP_Vector3D(-0.864188, -0.442863, 0.238856), 
      WP_Vector3D(-0.809017, -0.309017, 0.500000), 
      WP_Vector3D(-0.688191, -0.587785, 0.425325), 
      WP_Vector3D(-0.681718, -0.147621, 0.716567), 
      WP_Vector3D(-0.442863, -0.238856, 0.864188), 
      WP_Vector3D(-0.587785, -0.425325, 0.688191), 
      WP_Vector3D(-0.309017, -0.500000, 0.809017), 
      WP_Vector3D(-0.147621, -0.716567, 0.681718), 
      WP_Vector3D(-0.425325, -0.688191, 0.587785), 
      WP_Vector3D(-0.162460, -0.262866, 0.951056), 
      WP_Vector3D(0.442863, -0.238856, 0.864188), 
      WP_Vector3D(0.162460, -0.262866, 0.951056), 
      WP_Vector3D(0.309017, -0.500000, 0.809017), 
      WP_Vector3D(0.147621, -0.716567, 0.681718), 
      WP_Vector3D(0.000000, -0.525731, 0.850651), 
      WP_Vector3D(0.425325, -0.688191, 0.587785), 
      WP_Vector3D(0.587785, -0.425325, 0.688191), 
      WP_Vector3D(0.688191, -0.587785, 0.425325), 
      WP_Vector3D(-0.955423, 0.295242, 0.000000), 
      WP_Vector3D(-0.951056, 0.162460, 0.262866), 
      WP_Vector3D(-1.000000, 0.000000, 0.000000), 
      WP_Vector3D(-0.850651, 0.000000, 0.525731), 
      WP_Vector3D(-0.955423, -0.295242, 0.000000), 
      WP_Vector3D(-0.951056, -0.162460, 0.262866), 
      WP_Vector3D(-0.864188, 0.442863, -0.238856), 
      WP_Vector3D(-0.951056, 0.162460, -0.262866), 
      WP_Vector3D(-0.809017, 0.309017, -0.500000), 
      WP_Vector3D(-0.864188, -0.442863, -0.238856), 
      WP_Vector3D(-0.951056, -0.162460, -0.262866), 
      WP_Vector3D(-0.809017, -0.309017, -0.500000), 
      WP_Vector3D(-0.681718, 0.147621, -0.716567), 
      WP_Vector3D(-0.681718, -0.147621, -0.716567), 
      WP_Vector3D(-0.850651, 0.000000, -0.525731), 
      WP_Vector3D(-0.688191, 0.587785, -0.425325), 
      WP_Vector3D(-0.587785, 0.425325, -0.688191), 
      WP_Vector3D(-0.425325, 0.688191, -0.587785), 
      WP_Vector3D(-0.425325, -0.688191, -0.587785), 
      WP_Vector3D(-0.587785, -0.425325, -0.688191), 
      WP_Vector3D(-0.688191, -0.587785, -0.425325), 
    };

WP_Model_MD2::WP_Model_MD2(const string& name, const WP_Vector3D& scaling): WP_Model(name, scaling){}

bool WP_Model_MD2::initModel()
{
  int i = 0;
  int j = 0;

  WP_Endian* endian = WP_Endian::getInstance();

  try
    {
      ifstream input(model_name.c_str(), ios::in | ios::binary);

      if (!input)
	{
	  char buf[256];
	  sprintf(buf, "WP_Model_MD2::initModel - error while initing model %s", model_name.c_str());
	  throw(buf);
	}

      //read file to memory

      int size = 0;
      byte c;
      while(input.get(c))
	{
	  size++;
	}
      
      byte* buffer = new byte[size];
      if (!buffer)
	{
	  input.close();
	  throw("WP_Model_MD2::initModel: Error while allocating memory");
	}

      input.close();
      input.open(model_name.c_str(), ios::binary | ios::in);
      if(input.fail())
	{ 
	  char buf[256];
	  sprintf(buf, "WP_Model_MD2::initModel - error while initing model %s", model_name.c_str());
	  throw(buf);
	}

      byte* p = buffer;

      while(input.get(c))
	{
	  *p = c;
	  p++;
	}

      input.close();
      p = buffer;  

      WP_MD2_HEADER header;

      endian->getTypedData(&header.magic, p, sizeof(int));
      endian->getTypedData(&header.version, p, sizeof(int));
      endian->getTypedData(&header.skinWidth, p, sizeof(int));
      endian->getTypedData(&header.skinHeight, p, sizeof(int));
      endian->getTypedData(&header.frameSize, p, sizeof(int));
      endian->getTypedData(&header.numSkins, p, sizeof(int));
      endian->getTypedData(&header.numVertices, p, sizeof(int));
      endian->getTypedData(&header.numTexCoords, p, sizeof(int));
      endian->getTypedData(&header.numTriangles, p, sizeof(int));
      endian->getTypedData(&header.numGlCommands, p, sizeof(int));
      endian->getTypedData(&header.numFrames, p, sizeof(int));
      endian->getTypedData(&header.offsetSkins, p, sizeof(int));
      endian->getTypedData(&header.offsetTexCoords, p, sizeof(int));
      endian->getTypedData(&header.offsetTriangles, p, sizeof(int));
      endian->getTypedData(&header.offsetFrames, p, sizeof(int));
      endian->getTypedData(&header.offsetGlCommands, p, sizeof(int));
      endian->getTypedData(&header.offsetEnd, p, sizeof(int));
      
      //get skin
      
      p = buffer + header.offsetSkins;

      char name[64];
      for (i = 0; i < 64; i++)
	{
	  name[i] = *p;
	  p++;
	}

      string sname(name);
  
      for (i = 0; i < 64; i++)
	{
	  name[i] = *p;
	  p++;
	}
  
      int last = sname.find_last_of('/');
      if (last != -1)
	{
	  sname = sname.substr(last + 1, sname.length());
	}
      
      WP_TextureManager* tex_manager = WP_TextureManager::getInstance();
      int tex_id = tex_manager->getTexture(sname.c_str(), this);

      p = buffer + header.offsetFrames;
      
      //FIXME at this point only one frame allowed, so no animation available. Lineair interpolation functions are available in WPCG/WP_Vertex
      
      //create one mesh
      
      number_meshes = 1;
      meshes = new WP_Mesh[number_meshes];
      WP_Vertex* vertices = new WP_Vertex[header.numVertices];
      
      float x,y,z;
      endian->getTypedData(&x, p, sizeof(float));
      endian->getTypedData(&y, p, sizeof(float));
      endian->getTypedData(&z, p, sizeof(float));
      
      WP_Matrix3D scale(SCALING_MATRIX, x, y, z);
      
      endian->getTypedData(&x, p, sizeof(float));
      endian->getTypedData(&y, p, sizeof(float));
      endian->getTypedData(&z, p, sizeof(float));
      
      WP_Matrix3D translate(TRANSLATION_MATRIX, x, y, z);
      
      char mesh_name[16];
      for (i = 0; i < 16; i++)
	{
	  mesh_name[i] = *p;
	  p++;
	}
      
      for (j = 0; j < header.numVertices; j++)
	{
	  byte x,y,z;
	  endian->getTypedData(&x, p, sizeof(byte));
	  endian->getTypedData(&y, p, sizeof(byte));
	  endian->getTypedData(&z, p, sizeof(byte));

	  WP_Point3D point(x, y, z);
	  point *= scale;
	  point *= translate;
	  
	  endian->getTypedData(&x, p, sizeof(byte));
	  
	  WP_Vector3D normal = quake2_normals[x];
	  
	  WP_Vertex vertex(point, normal);
	  vertices[j] = vertex;
	}
      
      WP_Triangle* faces = new WP_Triangle[header.numTriangles];
      
      short index;
      
      p = buffer + header.offsetTriangles;
      
      for (i = 0; i < header.numTriangles; i++)
	{
	  //read vertex indices
	  
	  endian->getTypedData(&index, p, sizeof(short));
	  faces[i].vertices[0] = vertices + index;
	  endian->getTypedData(&index, p, sizeof(short));
	  faces[i].vertices[1] = vertices + index ; 
	  endian->getTypedData(&index, p, sizeof(short));
	  faces[i].vertices[2] = vertices + index ;
	  
	  //set triangle normal, this is stored in all the 3 vertices for now, later on each vertex normal will be calculated (see init_mesh)
	  
	  faces[i].normal = faces[i].vertices[0]->normal;
	  
	  //read tex_coords indices
	  p += 3 * sizeof(short);
	}
      
      p = buffer + header.offsetGlCommands;
      
      int command;
      int vertex_index;
      float s,t;
      
      //create display list of triangle fans and triangle strips
      
      meshes[0].displayID = glGenLists(1);
      glNewList(meshes[0].displayID, GL_COMPILE);
      glPushMatrix();
      
      glCullFace(GL_FRONT);
      WP_Material material;
      material.initMaterialGL();
      
      while(true)
	{
	  endian->getTypedData(&command, p, sizeof(int));
	  
	  if (command == 0)
	    {
	      break; //end of list
	    }
	  
	  if (command < 0)
	    {
	      //triangle fan
	      command = -command;
	      
	      glBegin(GL_TRIANGLE_FAN);
	      
	      for (i = 0; i < command; i++)
		{
		  endian->getTypedData(&s, p, sizeof(float));
		  endian->getTypedData(&t, p, sizeof(float));
		  endian->getTypedData(&vertex_index, p, sizeof(int));
		
		  glNormal3fv(vertices[vertex_index].normal.data);
		  glTexCoord2f(s, t);	
		  glVertex3fv(vertices[vertex_index].point.data);
		}
	      glEnd();
	    }
	  else
	    {
	      //triangle strip
	      
	      glBegin(GL_TRIANGLE_STRIP);
	      
	      for (i = 0; i < command; i++)
		{
		  endian->getTypedData(&s, p, sizeof(float));
		  endian->getTypedData(&t, p, sizeof(float));
		  endian->getTypedData(&vertex_index, p, sizeof(int));
		
		  glNormal3fv(vertices[vertex_index].normal.data);
		  glTexCoord2f(s, t);	
		  glVertex3fv(vertices[vertex_index].point.data);
		}
	      glEnd();
	    }
	}
      
      glPopMatrix();
      glEndList();
      
      //init mesh
      meshes[0].init_mesh("Quake2 mesh", header.numVertices, vertices, header.numTriangles, faces, tex_id, material, scaling_matrix, false, false);
      
      if (buffer)
	{
	  delete[] buffer;
	}

      return true;
    }
  catch(char* s)
    {
      cerr << s << endl;
      return false;
    }
  catch(...)
    {
      return false;
    }
}



