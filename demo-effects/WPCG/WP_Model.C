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
#include <list>
#include <math.h>
#include <GL/gl.h>
#include "WP_TextureManager.h"
#include "WP_GLState.h"
#include "WP_Camera.h"
#include "WP_Model.h"

/////////////////////////////WP_Model//////////////////////////////////

WP_Model::~WP_Model()
{
  delete scaling_matrix;

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
      /*      //set center and compute radius of sphere
      
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
      */
      return true;
      
    }
  catch(...)
    {
      return false;
    }
}

////////////////////////////////// WP_AnimatedModel ////////////////////////////////////

WP_AnimatedModel::~WP_AnimatedModel()
{
  delete frames;
}



WP_Point3D 
WP_AnimatedModel::WP_Frame::getMaxPoint() const
{
  WP_Point3D _max(MINFLOAT, MINFLOAT, MINFLOAT);
  /*  WP_Point3D p;

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
  */
  return _max;
}

WP_Point3D 
WP_AnimatedModel::WP_Frame::getMinPoint() const
{
  WP_Point3D _min(MAXFLOAT, MAXFLOAT, MAXFLOAT);
  WP_Point3D p;

  /*  int j = 0;
  
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
  */  
  return _min;
}

void WP_AnimatedModel::WP_Frame::computeBoundingSphere(WP_Matrix3D* scaling_matrix)
{
  /* //set center and determine sphere radius
  
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
  */
}

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

WP_Model_MD2::WP_Model_MD2(const string& name, const WP_Vector3D& scaling): WP_AnimatedModel(name, scaling){}

bool WP_Model_MD2::initModel()
{
  int i = 0;
  int j = 0;
  int k = 0;

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
      tex_id = tex_manager->getTexture(sname.c_str(), this);

      numberFrames = header.numFrames;
      frames = new WP_Frame[numberFrames];
      
      p = buffer + header.offsetFrames;

      for (k = 0; k < header.numFrames; ++k)
	{
	  (frames + k)->vertices = new WP_Vertex[header.numVertices];
      
	  float x,y,z;
	  endian->getTypedData(&x, p, sizeof(float));
	  endian->getTypedData(&y, p, sizeof(float));
	  endian->getTypedData(&z, p, sizeof(float));
      
	  WP_Matrix3D scale(SCALING_MATRIX, x, y, z);
      
	  endian->getTypedData(&x, p, sizeof(float));
	  endian->getTypedData(&y, p, sizeof(float));
	  endian->getTypedData(&z, p, sizeof(float));
      
	  WP_Matrix3D translate(TRANSLATION_MATRIX, x, y, z);
      
	  for (i = 0; i < 16; i++)
	      (frames + k)->name[i] = *p++;

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
	  
	      (frames + k)->vertices[j] = WP_Vertex(point, quake2_normals[x]);
	    }
	}  

      p = buffer + header.offsetGlCommands;
      
      int command;
      int vertex_index;
      float s,t;

      while(1)
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
	    WP_TriangleFan* fan = new WP_TriangleFan();
	    fan->numberIndices = command;
	    fan->indices = new unsigned int[command];
	    fan->texCoords = new float[command << 1];

	    unsigned int i;
	    for (i = 0; i < command; i++)
	      {
		endian->getTypedData(&s, p, sizeof(float));
		endian->getTypedData(&t, p, sizeof(float));
		endian->getTypedData(&vertex_index, p, sizeof(int));
		
		*(fan->indices + i) = vertex_index;
		*(fan->texCoords + (i << 1)) = s;
		*(fan->texCoords + (i << 1) + 1) = t;
	      }
 
	    triangle_groups.push_back(fan);
	  }
	else
	  {
	    //triangle strip
	    WP_TriangleStrip* strip = new WP_TriangleStrip();
	    strip->numberIndices = command;
	    strip->indices = new unsigned int[command];
	    strip->texCoords = new float[command << 1];

	    unsigned int i;
	    for (i = 0; i < command; i++)
	      {
		endian->getTypedData(&s, p, sizeof(float));
		endian->getTypedData(&t, p, sizeof(float));
		endian->getTypedData(&vertex_index, p, sizeof(int));

		*(strip->indices + i) = vertex_index;
		*(strip->texCoords + (i << 1)) = s;
		*(strip->texCoords + (i << 1) + 1) = t;
	      }
 
	    triangle_groups.push_back(strip);
	  }
      }
      
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

void 
WP_Model_MD2::drawOpenGL(const WP_Matrix3D& matrix) 
{
  glPushMatrix();
  glMultMatrixf(matrix.data); 
  
  glCullFace(GL_FRONT); 

  //animation
  if (interpolate >= 1.0)
    {
      currentFrame++;
      currentFrame %= numberFrames;
      interpolate = 0.0;
    }
  interpolate += 0.1;  

  list<WP_TriangleGroup*>::iterator i = triangle_groups.begin();
  glBindTexture(GL_TEXTURE_2D, tex_id);
  WP_GLState::getInstance()->enableTexture2D();

  while (i != triangle_groups.end())
    {
      if (currentFrame == numberFrames - 1 || interpolate == 0.0)
	(*i)->drawOpenGL((frames + currentFrame)->vertices, 0, interpolate);
      else
	(*i)->drawOpenGL((frames + currentFrame)->vertices, (frames + currentFrame + 1)->vertices, interpolate);
      i++;
    }

  WP_GLState::getInstance()->disableTexture2D();
  glPopMatrix();
  glCullFace(GL_BACK);
}


////////////////////////// WP_MetaBall ////////////////////

WP_MetaBall::WP_MetaBall(const string& name, const WP_Vector3D& scaling):WP_Model(name, scaling)
{
  
}

void 
WP_MetaBall::drawOpenGL(const WP_Matrix3D& matrix)
{
  glPushMatrix();
  glMultMatrixf(matrix.data); 

  //fixme t be done
  
  glEnd();
  glPopMatrix();
}

/**
 * this function is used for reading the model file and initializing the model by filling the variables of the base class with the appropriate read values. This function is automaticly called by the base class WP_Model by a call to its <i>init</i> function
 */
bool 
WP_MetaBall::initModel()
{
  return true;
}
