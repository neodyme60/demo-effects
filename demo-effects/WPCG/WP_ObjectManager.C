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

#include <iostream.h>
#include <stdio.h>
#include <values.h>
#include <math.h>
#include "WP_Model.h"
#include "WP_Camera.h"
#include "WP_ObjectManager.h"

WP_Math* WP_DynamicObject::math = WP_Math::getInstance();

WP_Object::WP_Object(const WP_Matrix3D& _matrix, const string& name):matrix(_matrix),object_name(name), heading(0), pitch(0), roll(0), inFrustum(true)
{
  //determine dir and up vectors, it is assumed that the object is orientated correctly thus facing north (0.0f, 0.0f, -1.0f) and up is (0.0f, 1.0f, 0.0f)
  
  dir = WP_Vector3D(0.0f, 0.0f, -1.0f);
  up = WP_Vector3D(0.0f, 1.0f, 0.0f);
};

void WP_Object::drawOpenGL() const 
{ 
    model->drawOpenGL(matrix);
}

void WP_Object::print() const
{
  cout << endl << "************************************" << endl;
  cout << "Object Name: " << object_name << endl;
  cout << "Model Name: " << model->model_name << endl;
  cout << "Position: x:" << matrix.data[12] << " y:" << matrix.data[13] << " z:" << matrix.data[14] << endl;
  cout << "Heading: " << heading << " degrees" << endl;
  cout << "Pitch: " << pitch << " degrees" << endl;
  cout << "Roll: " << roll << " degrees" << endl;
}

//********************************************* WP_ObjectManager *******************************************************

const unsigned char WP_ObjectManager::num_internal_models = 1;

WP_ObjectManager* WP_ObjectManager::om_instance = 0;

const string WP_ObjectManager::internal_models[] = {"WP_METABALL"};

WP_ObjectManager::WP_ObjectManager():cam(WP_Camera::getInstance()), unique(0){}

WP_ObjectManager::~WP_ObjectManager()
{
  removeAll();
  om_instance = 0;
}

WP_ObjectManager* WP_ObjectManager::getInstance()
{
  if (!om_instance)
    {
      om_instance = new WP_ObjectManager();
    }
  return om_instance;
}

//FIXME this function is also used in WP_Image.C, therefore better to create a WP_File class and park this function there
bool WP_ObjectManager::hasValidExtension(const char* file, const char* extension)
{
  //find the '.' separating the extension
  const char* copy = file;
  copy += strlen(file); //set pointer to last token
  while(*copy != '.' && copy != file)
    {
      copy--;
    }

  if (*copy != '.')
    {
      return false; //file has no extension
    }

  return strcasecmp(++copy, extension) == 0;
}

WP_Object* WP_ObjectManager::createStaticObject(const WP_Matrix3D& matrix, const string& object_name, 
							    const string& model_name, const WP_Vector3D& scaling)
{
  WP_StaticObject* sobject;
  WP_Model* model;
try
  {
    cout << "Creating static object " << object_name << " of model " << model_name << " at position: " << matrix.data[12] 
	 << " " << matrix.data[13] << " " << matrix.data[14] << " scaled by " << scaling.data[0] << " " << scaling.data[1] 
	 << " " << scaling.data[2] << endl;
  
    sobject = new WP_StaticObject(WP_Matrix3D(SCALING_MATRIX, scaling.data[0], scaling.data[1], scaling.data[2]) * matrix, object_name);
    if (!sobject)
      {
	return (WP_Object*)0;
      }

    //check if it's an internal object

    int i;
    bool internal = false;

    for (i = 0; i < num_internal_models; ++i)
      {
	if (object_name == internal_models[i])
	  {
	    internal = true;
	    sobject->name_id = unique++;
	    model = findInstance(model_name);
	    if (model)
	      {
		model->count++;
		sobject->model = model;
		static_objects.push_back(sobject);
	      }
	    else
	      {
		model = new WP_MetaBall(model_name, scaling);
		if (!model->init())
		  {
		    throw("");
		  }
		sobject->model = model;
		static_objects.push_back(sobject);
	      }
	    break;
	  }
      }
      
    if (!internal)
      {
	string m_name = "../MODELS/" + model_name;
	sobject->name_id = unique++;
	model = findInstance(m_name);
	if (model)
	  {
	    model->count++;
	    sobject->model = model;
	    static_objects.push_back(sobject);
	  }
	else
	  {
	    if (hasValidExtension(m_name.c_str(), "md2"))
	      {
		//quake2 md2 file
		model = new WP_Model_MD2(m_name, scaling);
		if (!model->init())
		  {
		    throw("");
		  }
		sobject->model = model;
		static_objects.push_back(sobject);
	      }
	    else
	      {
		//other formats like for example lightwave format
		cerr << "ERROR: Unable to load model " << m_name << "only quake2 md2 file format supported at the moment" << endl;
		throw("");
	      }
	  }
      }
  }
catch(...)
  {
    delete sobject;
    delete model;
    return (WP_Object*)0;
  }
 return sobject;
}

WP_Object* WP_ObjectManager::createDynamicObject(const WP_Matrix3D& matrix, const string& object_name, 
							    const string& model_name, const WP_Vector3D& scaling, const WP_Vector3D& velocity)
{
  WP_DynamicObject* dobject;
  WP_Model* model;
try
  {

    cout << "Creating dynamic object " << object_name << " of model " << model_name << " at position: " << matrix.data[12] 
	 << " " << matrix.data[13] << " " << matrix.data[14] << " with vector " << velocity.data[0] << " " << velocity.data[1] << " " 
	 << velocity.data[2] << " scaled by " << scaling.data[0] << " " << scaling.data[1] 
	 << " " << scaling.data[2] << endl;

    dobject = new WP_DynamicObject(WP_Matrix3D(SCALING_MATRIX, scaling.data[0], scaling.data[1], scaling.data[2]) * matrix, object_name, velocity);
    if (!dobject)
      {
	return (WP_Object*)0;
      }

    //check if it's an internal object

    int i;
    bool internal = false;

    for (i = 0; i < num_internal_models; ++i)
      {
	if (model_name == internal_models[i])
	  {
	    internal = true;
	    dobject->name_id = unique++;
	    model = new WP_MetaBall(model_name, scaling);
	    if (!model->init())
	      {
		throw("");
	      }
	    dobject->model = model;
	    dynamic_objects.push_back(dobject);
	    break;
	  }
      }

    if (!internal)
      {
	string m_name = "../MODELS/" + model_name;
	dobject->name_id = unique++;

	/*model = findInstance(m_name);
  
	if (model) //FIXME add possibility to share animated models (current frame, interpolation etc) disabled for now 
	{
	model->count++;
	dobject->model = model;
	dynamic_objects.push_back(dobject);
	}
	else
	{
	*/
     
	if (hasValidExtension(m_name.c_str(), "md2"))
	  {
	    //quake2 MD2 file
	    model = new WP_Model_MD2(m_name, scaling);
	    if (!model->init())
	      {
		throw("");
	      }
	    dobject->model = model;
	    dynamic_objects.push_back(dobject);
	  }
	else
	  {
	    //other formats like for example lightwave format
	    cout << "ERROR: Unable to load model " << m_name << "\nonly quake2 md2 fileformat supported at the moment" << endl;
	    throw("");
	  }
      }
  }
catch(...)
  {
    delete dobject;
    delete model;
    return (WP_Object*)0;
  }
 return dobject;
}

WP_Object* WP_ObjectManager::getObject(const string& name) const
{
  WP_Object* object = getStaticObject(name);
  if (!object)
    {
      return getDynamicObject(name);
    }
  return object;
}

//if last == 0 then the first element of the list is given
//0 is returned when list is empty
WP_StaticObject* WP_ObjectManager::getNextStaticObject(const WP_StaticObject* last) const
{
  list<WP_StaticObject*>::const_iterator j;
  if (last)
    {
      list<WP_StaticObject*>::const_iterator i = static_objects.begin();
      while (i != static_objects.end())
	{	
	  if ((*i) == last)
	    {
	      if (++i != static_objects.end())
		{
		  return (*i);
		}
	      break;
	    }
	  i++;
	}
    }
  j = static_objects.begin();
  if (j != static_objects.end())
    {
      return (*j);
    }
  return 0; //empty list
}

//if last == 0 then the first element of the list is given
//0 is returned when list is empty
WP_DynamicObject* WP_ObjectManager::getNextDynamicObject(const WP_DynamicObject* last) const
{
  list<WP_DynamicObject*>::const_iterator j;
  if (last)
    {
      list<WP_DynamicObject*>::const_iterator i = dynamic_objects.begin();
      while (i != dynamic_objects.end())
	{	
	  if ((*i) == last)
	    {
	      if (++i != dynamic_objects.end())
		{
		  return (*i);
		}
	      break;
	    }
	  i++;
	}
    }
  j = dynamic_objects.begin();
  if (j != dynamic_objects.end())
    {
      return (*j);
    }
  return 0; //empty list
}

WP_StaticObject* WP_ObjectManager::getStaticObject(const string& name) const
{
  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
     if ((*i)->object_name == name)
	{
	  return (*i);
	}
      i++;
    }
  return 0; //instance not found
}

WP_StaticObject* WP_ObjectManager::getStaticObject() const
{
  if (static_objects.begin() != static_objects.end())
    {
      list<WP_StaticObject*>::const_iterator i = static_objects.begin();
      return (*i);
    }
  return 0; //no instance available
}

WP_DynamicObject* WP_ObjectManager::getDynamicObject(const string& name) const
{
  list<WP_DynamicObject*>::const_iterator i = dynamic_objects.begin();
  while (i != dynamic_objects.end())
    {	
     if ((*i)->object_name == name)
	{
	  return (*i);
	}
      i++;
    }
  return 0; //instance not found
}

WP_DynamicObject* WP_ObjectManager::getDynamicObject() const
{
  if (dynamic_objects.begin() != dynamic_objects.end())
    {
      list<WP_DynamicObject*>::const_iterator i = dynamic_objects.begin();
      return (*i);
    }
  return 0; //no instance available
}

void WP_ObjectManager::drawObjects() 
{
//  cam->meshes_in_frustum = 0;

  cam->followObject();

  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
      if (cam->inFrustum((*i)->matrix.data[12], (*i)->matrix.data[13],
			 (*i)->matrix.data[14], (*i)->model->radius))
	{
	  (*i)->drawOpenGL();
	  (*i)->inFrustum = true;
	}
      else
	{
	  (*i)->inFrustum = false;
	}
      i++;
    }

  list<WP_DynamicObject*>::const_iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
     if (cam->inFrustum((*j)->matrix.data[12], (*j)->matrix.data[13],
			(*j)->matrix.data[14], (*j)->model->radius))
	{
	  (*j)->drawOpenGL();
	  (*j)->inFrustum = true;
	}
     else
       {
	 (*j)->inFrustum = false;
       }
      j++;
    }
}

void WP_ObjectManager::drawObjectsSelection() 
{
  glInitNames(); //init the name stack

  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
      if (cam->inFrustum((*i)->matrix.data[12], (*i)->matrix.data[13],
			 (*i)->matrix.data[14], (*i)->model->radius))
	{
	  glPushName((*i)->name_id);
	  (*i)->drawOpenGL();
	  glPopName();
	}
      i++;
    }

  list<WP_DynamicObject*>::const_iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
      if (cam->inFrustum((*j)->matrix.data[12], (*j)->matrix.data[13],
			 (*j)->matrix.data[14], (*j)->model->radius))
	{
	  glPushName((*j)->name_id);
	  (*j)->drawOpenGL();
	  glPopName();
	}
      j++;
    }
}

WP_Model* WP_ObjectManager::findInstance(const string& model_name) 
{
  list<WP_StaticObject*>::iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
       if ((*i)->model->model_name == model_name)
	{
	  return (*i)->model;
	}
      i++;
    }

  list<WP_DynamicObject*>::iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
       if ((*j)->model->model_name == model_name)
	{
	  return (*j)->model;
	}
      j++;
    }
  return 0; //instance not found
}
  
bool WP_ObjectManager::removeStaticObject(const string& name)
{
  list<WP_StaticObject*>::iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
      if ((*i)->object_name == name)
	{
	  (*i)->model->count--;
	  if ((*i)->model->count == 0)
	    {
	      delete (*i)->model;
	    }
	  
	  delete (*i);
	  static_objects.erase(i);

	  return true; //object removed
	}
      i++;
    }
  return false; //object not found
}

bool WP_ObjectManager::removeDynamicObject(const string& name)
{
  list<WP_DynamicObject*>::iterator i = dynamic_objects.begin();
  while (i != dynamic_objects.end())
    {	
      if ((*i)->object_name == name)
	{
	  (*i)->model->count--;
	  if ((*i)->model->count == 0)
	    {
	      delete (*i)->model;
	    }
	  
	  delete (*i);
	  dynamic_objects.erase(i);

	  return true; //object removed
	}
      i++;
    }
  return false; //object not found
}

bool WP_ObjectManager::removeObject(const string& name)
{
  if (!removeStaticObject(name))
    return removeDynamicObject(name);
  return false;
}

bool WP_ObjectManager::removeSameObjects(const string& name)
{
  int static_counter = countStaticObjects(name);
  int j = 0;
  int static_removed = 0;

  for (; j < static_counter; j++)
    {
       list<WP_StaticObject*>::iterator i = static_objects.begin();
       while (i != static_objects.end())
	 {	
	   if ((*i)->object_name == name)
	     {
	       (*i)->model->count--;
	       if ((*i)->model->count == 0)
		 {
		   delete (*i)->model;
		 }

	       delete (*i);
	       static_objects.erase(i);
	       static_removed++;
	       break;
	     }
	   i++;
	 }
    }
  
  int dynamic_counter = countDynamicObjects(name);
  j = 0;
  int dynamic_removed = 0;

  for (; j < dynamic_counter; j++)
    {
       list<WP_DynamicObject*>::iterator i = dynamic_objects.begin();
       while (i != dynamic_objects.end())
	 {	
	   if ((*i)->object_name == name)
	     {
	       (*i)->model->count--;
	       if ((*i)->model->count == 0)
		 {
		   delete (*i)->model;
		 }
	       
	       delete (*i);
	       dynamic_objects.erase(i);
	       dynamic_removed++;
	       break;
	     }
	   i++;
	 }
    }
  return static_removed == static_counter && dynamic_removed == dynamic_counter;
}

int WP_ObjectManager::countStaticObjects(const string& name) const
{
  int counter = 0;
  list<WP_StaticObject*>::const_iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
      if ((*i)->object_name == name)
	{
	  counter++;
	}
      i++;
    }
  return counter;
}

int WP_ObjectManager::countDynamicObjects(const string& name) const
{
  int counter = 0;
  list<WP_DynamicObject*>::const_iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
      if ((*j)->object_name == name)
	{
	  counter++;
	}
      j++;
    }
  return counter;
}

bool WP_ObjectManager::removeAll()
{
  int count = static_objects.size() + dynamic_objects.size();
  int removed = 0;

  list<WP_StaticObject*>::iterator i = static_objects.begin();
  while (i != static_objects.end())
    {	
      (*i)->model->count--;
      if ((*i)->model->count == 0)
	{
	  delete (*i)->model;
	}
      
      delete (*i);
      removed++;
      i++;
    }
  static_objects.clear();

 list<WP_DynamicObject*>::iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
      (*j)->model->count--;
      if ((*j)->model->count == 0)
	{
	  delete (*j)->model;
	}
      
      delete (*j);
      removed++;
      j++;
    }
  dynamic_objects.clear();

  return removed == count;
}

void WP_ObjectManager::moveObjects() 
{
  list<WP_DynamicObject*>::iterator j = dynamic_objects.begin();
  while (j != dynamic_objects.end())
    {	
      (*j)->move();
      j++;
    }
}

WP_Object* WP_ObjectManager::pickObject(int x, int y)
{
  //FIXME there are some possibilities for performance improvement here
  //first render only the objects which are in the new selection buffer viewing volume, so again frustum 
  //culling
  //Secondly, use object addresses for names, in this case it is not necessary to walk through the object
  //lists to match the objects' name_id and the found name in the selection buffer

  GLuint selectBuf[512];
  WP_GLState* state = WP_GLState::getInstance();
  GLuint hits = 0;

  glSelectBuffer(512, selectBuf);

  state->selection_mode();

  cam->setPickingVolume(5, 5, x, y);

  drawObjectsSelection();

  cam->setRenderVolume();
  hits = state->render_mode();

  //now all objects are rendered to the selection buffer and a hitlist was created and stored in selectBuf. The picked object name is stored in this list (if hits > 0). Now get the name of the closest picked object (depth information is smallest, see OpenGL's red book

  if (hits > 0)
    {
      unsigned int k;
      GLuint names;
      GLuint* ptr;
      GLuint minZ;
      GLuint closest_name_id = 0;
      
      ptr = selectBuf;
      minZ = 0xffffffff;
      for (k = 0; k < hits; k++) 
	{ 
	  names = *ptr;
	  ptr++;
	  if (*ptr < minZ) 
	    {
	      minZ = *ptr;
	      closest_name_id = *(ptr + 2);
	    }
	  ptr += 2 + names;
	}

      //the name of the closest name is now known and stored in closest_name_id
      //so find and return it
      
      list<WP_StaticObject*>::const_iterator i = static_objects.begin();
      while (i != static_objects.end())
	{	
	  if ((*i)->name_id == closest_name_id)
	    {
	      return (*i);
	    }
	  i++;
	}
  
      list<WP_DynamicObject*>::const_iterator j = dynamic_objects.begin();
      while (j != dynamic_objects.end())
	{	
	  if ((*j)->name_id == closest_name_id)
	    {
	      return (*j);
	    }
	  j++;
	}
    }

  return (WP_Object*)0;
}

//**************************************************************************************

WP_DynamicObject::WP_DynamicObject(const WP_Matrix3D& _matrix, const string& name, const WP_Vector3D& _velocity):
  WP_Object(_matrix, name), velocity(_velocity)
{
  //orientate object according to velocity vector, object should be orientated according to 0 degrees on a compass 

  heading = computeHeading();
  WP_Matrix3D yrot(Y_ROTATION_MATRIX, -heading); //negative heading because the rotation in WP_Matrix3D is Counter Clockwise (CCW) and it should be CW
  matrix *= yrot;

  //set speed, speed is length of the velocity vector
  speed = sqrt((velocity.data[0] * velocity.data[0]) + (velocity.data[1] * velocity.data[1]) + (velocity.data[2] * velocity.data[2]));

  //now calculate the rotation on the x-axis, see page 831 of HILL

  pitch = computePitch();
  if (pitch != 0.0f)
    {
      WP_Matrix3D xrot(X_ROTATION_MATRIX, pitch);
   
      matrix *= xrot;
      
      dir *= xrot;
      up *= xrot;
    }

  dir *= yrot;
  up *= yrot;

  dir.normalize();
  up.normalize();
}

WP_DynamicObject::WP_DynamicObject(const WP_Matrix3D& _matrix, const string& name, scalar _heading, scalar _speed, 
				   scalar _pitch):WP_Object(_matrix, name), speed(_speed)
{
  heading = _heading;
  pitch = _pitch;

  velocity = computeVelocityVector();
  WP_Matrix3D yrot(Y_ROTATION_MATRIX, -heading); //negative heading because the rotation in WP_Matrix3D is Counter Clockwise (CCW) and it should be CW
  matrix *= yrot;
  
  //now calculate the rotation on the x-axis, see page 831 of HILL

  if (pitch != 0.0f)
    {
      WP_Matrix3D xrot(X_ROTATION_MATRIX, pitch);
      matrix *= xrot;
    }

  dir.normalize();
  up.normalize();
}

scalar WP_DynamicObject::computeHeading() const
{
  //first calculate rotation on y-axis, see page 831 of Hill, Computer Graphics using OpenGL
  
  if (velocity.data[0] == 0.0f && velocity.data[2] == 0.0f)
    {
      return 0.0f;
    }
  else
    {
      //first the outcome of the atan2 function was compared to the actual orientation on the unitcircle
      //the following table was made
      //
      //outcome          unitcircle
      //
      // 1/2PI               -1/2PI
      // -1/2PI               1/2PI
      // 0                    0
      // PI                   PI
      // 1/4PI                -1/4PI
      // etc                  etc
      
      //this table shows that when the z (velocity.data[2]) != 0.0, the outcome has to be inverted to match the correct
      // unitcircle position. When the z == 0.0 the position on the unit circle is correct.

      scalar course = atan2(velocity.data[2] , velocity.data[0]);

      if (velocity.data[2] != 0.0f)
      	{
	  course = -course;
	}

      //now that the course matches the correct position on the unitcircle it has got to be corrected to match a compass orientation
      //compared to the unitcircle a compass starts at 1/2PI with 0 degrees (north), 0 is 90 degrees (east), -1/2PI = 180 degrees(south),
      //PI is 270 degrees(west). The compass its orientation is clockwise!.
      //The above mentioned values show that when the course is corrected by course = HALF_PI - course, the values are correctly matched on
      //the compass, so the formula is 1/2PI - course.

      course = HALF_PI - course;

      return math->fRadToDegree(course);
      
    }
}

//this function computes the velocity vector according to the known heading, speed and pitch
WP_Vector3D WP_DynamicObject::computeVelocityVector() 
{
  //check for zero speed
  if (speed == 0.0f)
    {
      return WP_Vector3D(0.0f, 0.0f, 0.0f);
    }

  //first convert compass heading to unitcircle heading
  scalar course = math->fDegreeToRad(heading);
  course = HALF_PI - course;

  //determine x and z component of resulting vector

  WP_Vector3D vec(cos(course), 0.0f, -sin(course));

  //rotate vector according to its pitch
  //first rotate it pointing to 0 degrees (north);

    if (pitch != 0.0f)
    {
      scalar difference = 360.0f - heading; 
      if (difference !=  0)
	{
	  WP_Matrix3D yrot(Y_ROTATION_MATRIX, -difference); 
	  vec *= yrot;
	}
  
      //now rotate around x-axis pitch degrees
      WP_Matrix3D xrot(X_ROTATION_MATRIX, pitch);

      vec *= xrot;

      dir *= xrot;
      up *= xrot;

      //rotate vector back to heading
 
      if (difference !=  0)
	{
	  WP_Matrix3D yrot(Y_ROTATION_MATRIX, difference); 
	  WP_Matrix3D yrot2(Y_ROTATION_MATRIX, difference - 360.0f);
	  vec *= yrot;

	  dir *= yrot2;
	  up *= yrot2;
	}
    }

    vec.data[0] *= speed;
    vec.data[1] *= speed;
    vec.data[2] *= speed;

    return vec;
}

scalar WP_DynamicObject::computePitch() const
{
  //now calculate the rotation on the x-axis, see page 831 of HILL

  if (speed != 0.0f)
    {
      return math->fRadToDegree(asin(velocity.data[1] / speed));
    }
  return 0;
}

void WP_DynamicObject::setVelocityVector(const WP_Vector3D& vector)
{
  velocity = vector;
  scalar new_heading = computeHeading();
  setNewHeading(new_heading);
  speed = sqrt((velocity.data[0] * velocity.data[0]) + (velocity.data[1] * velocity.data[1]) + (velocity.data[2] * velocity.data[2]));
  scalar new_pitch = computePitch();
  setNewPitch(new_pitch);
}

//adds degrees to the current heading
void WP_DynamicObject::changeHeading(scalar delta_degrees)
{
  setNewHeading(heading + delta_degrees);
}

void WP_DynamicObject::changePitch(scalar delta_degrees)
{
  setNewPitch(pitch + delta_degrees);
}

void WP_DynamicObject::changeRoll(scalar delta_degrees)
{
  setNewRoll(roll + delta_degrees);
}

//orientates object according to the new heading
//rotation is CW
void WP_DynamicObject::setNewHeading(scalar new_heading)
{
  if (heading != new_heading)
    {
      if (new_heading >= 360.0f)
	{
	  new_heading -= 360.0f;
	}
      else if (new_heading < 0.0f)
	{
	  new_heading += 360.0f;
	}

      scalar difference = heading - new_heading;
      if (difference !=  0.0f)
	{
	  WP_Matrix3D yrot(Y_ROTATION_MATRIX, difference); 
	  matrix *= yrot;
	  heading = new_heading;

	  //adjust dir and up

	  dir *= yrot;
	  up *= yrot;

	  //change velocity vector too

	  velocity *= yrot;
	}
    }
}

//orientates object according to the new pitch
//90 degrees is up, 270 degrees is down
void WP_DynamicObject::setNewPitch(scalar new_pitch)
{
  if (pitch != new_pitch)
    {
      if (new_pitch >= 360.0f)
	{
	  new_pitch -= 360.0f;
	}
      else if (new_pitch < 0.0f)
	{
	  new_pitch += 360.0f;
	}

      scalar difference = new_pitch - pitch;
      if (difference != 0.0f)
	{
	  WP_Matrix3D xrot(X_ROTATION_MATRIX, difference);
	  matrix *= xrot;
	  pitch = new_pitch;

	  //change velocity vector too
	  WP_Matrix3D xrot2(X_ROTATION_MATRIX, -difference);
	  velocity *= xrot2;

	  //adjust dir and up

	  dir *= xrot2; //FIXME IS THIS CORRECT?
	  up *= xrot2;
	}
    }
}

//orientates object according to the new pitch
//roll is in CW order
void WP_DynamicObject::setNewRoll(scalar new_roll)
{
  if (roll != new_roll)
    {
      if (new_roll >= 360.0f)
	{
	  new_roll -= 360.0f;
	}
      else if (new_roll < 0.0f)
	{
	  new_roll += 360.0f;
	}

      scalar difference = new_roll - roll;
      if (difference != 0.0f)
	{
	  WP_Matrix3D zrot(Z_ROTATION_MATRIX, difference);
	  matrix *= zrot;
	  roll = new_roll;

	  //adjust up

	  up *= zrot;
	}
    }
}

void WP_DynamicObject::changeSpeed(scalar delta_speed)
{
  setNewSpeed(speed + delta_speed);
}

void WP_DynamicObject::setNewSpeed(scalar new_speed)
{
  if (new_speed != speed)
    {
      scalar multiply = 0.0f;
 
      if (speed == 0.0f)
	{
	  //compute new velocity vector, because object starts moving again
	  speed = new_speed;
	  velocity = computeVelocityVector();
	}
      else
	{
	  multiply = new_speed / speed;
	  speed = new_speed;
	  velocity *= multiply;
	}
   }
}

void WP_DynamicObject::move()
{
  matrix.data[12] += velocity.data[0];
  matrix.data[13] += velocity.data[1];
  matrix.data[14] += velocity.data[2];
}

void WP_DynamicObject::print() const
{
  WP_Object::print();
  cout << "Speed: " << speed << endl;
  cout << "Velocity vector: x:" << velocity.data[0] << " y:" << velocity.data[1] << " z:" << velocity.data[2] << endl;
}

