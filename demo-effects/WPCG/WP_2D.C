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

#include <fstream.h>
#include <math.h>
#include <GL/glut.h>
#include "WP_2D.h"
#include "WP_Point2D.h"

//////////////////// WP_Draw_2D ////////////////////////////////

WP_Draw_2D::WP_Draw_2D(){}
WP_Draw_2D::~WP_Draw_2D(){}

void WP_Draw_2D::vDrawPoint(GLint x, GLint y) const
{
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();
}

void WP_Draw_2D::vDrawPoint(const WP_Point2D* point) const
{
	glBegin(GL_POINTS);
		glVertex2f(point->x, point->y);
	glEnd();
}

void WP_Draw_2D::vDrawLine(GLint startx, GLint starty, GLint endx, GLint endy) const
{
	glBegin(GL_LINES);
		glVertex2i(startx, starty);
		glVertex2i(endx, endy);
	glEnd();
}

void WP_Draw_2D::vDrawLine(const WP_Point2D* startpoint, const WP_Point2D* endpoint) const
{
	glBegin(GL_LINES);
		glVertex2f(startpoint->x, startpoint->y);
		glVertex2f(endpoint->x, endpoint->y);
	glEnd();
}

void WP_Draw_2D::vDrawPolyLine(WP_Point2D* points, GLint number, bool closed) const
{
	glBegin(closed ? GL_LINE_LOOP : GL_LINE_STRIP);
	for (GLint i = 0; i < number; i++)
		glVertex2f(points[i].x, points[i].y);	
	glEnd();
	glFlush();
}

void WP_Draw_2D::vDrawString(void* font, const char* text,
			  GLint xpos, GLint ypos) const
{
  for (; *text != '\0'; text++)
    {
      glRasterPos2i(xpos, ypos);
      glutBitmapCharacter(font, *text);
      xpos += glutBitmapWidth(font,*text);
    }
}
/*
//////////////////// WP_PolyFileReader ////////////////////////////////

WP_PolyFileReader::WP_PolyFileReader():number(0), lines(NULL){}

WP_PolyFileReader::~WP_PolyFileReader()
{
	if (lines)
		vFreeAll();
}

GLint WP_PolyFileReader::vReadFile(const char* file)
{
	//check extension

	GLint i = 0;
	while (file[i++] != '.');
	if (file[i] == 'p' && file[i + 1] == 'o' && file[i + 2] == 'l')
		return iLoadNormalFile(file);
	else if (file[i] == 'd' && file[i + 1] == 'p' && file[i + 2] == 'o' && file[i + 3] == 'l')
		return iLoadDifferentialFile(file);
	else
		return -1; //failure
}

void WP_PolyFileReader::vDrawPolyLines() const
{
	if (!lines)
		return;

	for (GLint i = 0; i < number; i++)
	{
		lines[i]->vDraw();
	}
}

GLint WP_PolyFileReader::iLoadNormalFile(const char* file)
{
	fstream input(file, ios::in | ios::nocreate);
	if (input.fail())
		return -1; //failure

	input >> number;
	if (number < 0)
		return -1; //failure

	if (lines)
	{
		vFreeAll();
	}

	lines = new WP_PolyLine* [number];

	GLint x, y, count;
	GLfloat red, green, blue;

	for (GLint i = 0; i < number; i++)
	{
		input >> count;
		input >> red >> green >> blue;

		WP_PolyLine* poly;
		if (count < 0)
		{
			poly = new WP_PolyLine(red, green , blue, 1);
			count = -count;
		}
		else
			poly = new WP_PolyLine(red, green , blue, 0);
		
		for (GLint j = 0; j < count; j++)
		{
			input >> x >> y;
			WP_Point2D* p = new WP_Point2D(x, y);
			poly->vAddPoint(p);
		}
		lines[i] = poly;
	}
	return 1; //success
}

GLint WP_PolyFileReader::iLoadDifferentialFile(const char* file)
{
	fstream input(file, ios::in | ios::nocreate);
	if (input.fail())
		return -1; //failure

	input >> number;
	if (number < 0)
		return -1; //failure

	if (lines)
	{
		vFreeAll();
	}

	lines = new WP_PolyLine* [number];

	GLint x, y, oldx, oldy, count;
	GLfloat red, green, blue;

	for (GLint i = 0; i < number; i++)
	{
		oldx = 0;
		oldy = 0;
		
		input >> count;
		input >> red >> green >> blue;

		WP_PolyLine* poly;
		if (count < 0)
		{
			poly = new WP_PolyLine(red, green , blue,1);
			count = -count;
		}
		else
			poly = new WP_PolyLine(red, green , blue,0);

		for (GLint j = 0; j < count; j++)
		{
			input >> x >> y;
			WP_Point2D* p = new WP_Point2D(oldx + x, oldy + y);
			poly->vAddPoint(p);
			oldx = oldx + x;
			oldy = oldy + y;
		}
		lines[i] = poly;
	}
	return 1; //success
}


void WP_PolyFileReader::vFreeAll()
{
	if (lines)
	{
		for (GLint i = 0; i < number; i++)
		{
			delete lines[i];
		}
		delete[] lines;
		lines = NULL;
	}
}

//////////////////// WP_PolyLine ////////////////////////////////

		
WP_PolyLine::WP_PolyLine():red(1.0f), green(1.0f), blue(1.0f), closed(0){}

WP_PolyLine::WP_PolyLine(GLfloat _red, GLfloat _green, GLfloat _blue,
												 bool _closed): red(_red), green(_green), blue(_blue), closed(_closed){}

WP_PolyLine::~WP_PolyLine()
{
		vFreeAll();
}

void WP_PolyLine::vAddPoint(WP_Point2D* point)
{
	points.push_back(point);
}

void WP_PolyLine::vRemoveAllPoints()
{
	vFreeAll();
}

void WP_PolyLine::vSetColor(GLfloat _red, GLfloat _green, GLfloat _blue)
{
	red		= _red;
	green = _green;
	blue	= _blue;
}

void WP_PolyLine::vGetColor(GLfloat* _red, GLfloat* _green, GLfloat* _blue) const
{
	*_red		= red;
	*_green = green;
	*_blue	= blue;
}

void WP_PolyLine::vSetClosed(bool _closed)
{
	closed = _closed;
}

bool WP_PolyLine::iGetClosed()
{
	return closed;
}

void WP_PolyLine::vDraw() const
{
	glBegin(closed ? GL_LINE_LOOP : GL_LINE_STRIP);
	std::list<WP_Point2D*>::const_iterator index = points.begin();

	glColor3f(red, green, blue);
	
	while (index != points.end())
	{
			glVertex2f((*index)->getX(), (*index)->getY());				
			index++;
	}		
	glEnd();
	glFlush();
}

void WP_PolyLine::vFreeAll()
{
	std::list<WP_Point2D*>::iterator index = points.begin();
	while (index != points.end())
	{
		delete (*index);
		index++;
	}
	points.clear();
}

//////////////////// WP_TROCHOIDS ////////////////////////////////

WP_Trochoids::WP_Trochoids(): radius1(10.0), radius2(5.0), rod(2.0), mode(0){}

WP_Trochoids::WP_Trochoids(GLfloat _radius1, GLfloat _radius2, GLfloat _rod):
		radius1(_radius1), radius2(_radius2), rod(_rod), mode(0){}

WP_Trochoids::~WP_Trochoids(){}


void WP_Trochoids::vDraw(GLfloat x, GLfloat y)
{
	if (!mode)
		vDrawEpiTrochoid(x, y);
	else
		vDrawHypoTrochoid(x, y);
}

void WP_Trochoids::vDrawEpiTrochoid(GLfloat centerx, GLfloat centery) const
{
	glBegin(GL_LINE_STRIP);
	
	for (GLfloat t = 0.0; t <= 1.0; t += 0.01)
	{
		GLfloat x = ((radius1 + radius2) * cos(DOUBLE_PI * t)) - 
			(rod * cos(DOUBLE_PI * (((radius1 + radius2) * t) / radius2)));
	
		GLfloat y = ((radius1 + radius2) * sin(DOUBLE_PI * t)) - 
			(rod * sin(DOUBLE_PI * (((radius1 + radius2) * t) / radius2)));
	
		glVertex2f(centerx + x, centery + y);
	}

	glEnd();

	glFlush();
}

void WP_Trochoids::vDrawHypoTrochoid(GLfloat centerx, GLfloat centery) const
{
	glBegin(GL_LINE_STRIP);
	
	for (GLfloat t = 0.0; t <= 1.0; t += 0.01)
	{
		GLfloat x = ((radius1 - radius2) * cos(DOUBLE_PI * t)) +
			(rod * cos(DOUBLE_PI * (((radius1 - radius2) * t) / radius2)));
	
		GLfloat y = ((radius1 - radius2) * sin(DOUBLE_PI * t)) - 
			(rod * sin(DOUBLE_PI * (((radius1 - radius2) * t) / radius2)));
	
		glVertex2f(centerx + x, centery + y);
	}

	glEnd();
	glFlush();
}

void WP_Trochoids::vIncreaseRadius1(GLfloat add)
{
	radius1 += add;
}

void WP_Trochoids::vIncreaseRadius2(GLfloat add)
{
	radius2 += add;
}

void WP_Trochoids::vIncreaseRod(GLfloat add)
{
	rod += add;
}

void WP_Trochoids::vDecreaseRadius1(GLfloat sub)
{
		radius1 -= sub;
}

void WP_Trochoids::vDecreaseRadius2(GLfloat sub)
{
		radius2 -= sub;
}

void WP_Trochoids::vDecreaseRod(GLfloat sub)
{
		rod -= sub;
}

void WP_Trochoids::vSetMode(bool _mode)
{
	mode = _mode;
}

GLfloat WP_Trochoids::fGetRadius1() const
{
	return radius1;
}

GLfloat WP_Trochoids::fGetRadius2() const
{
	return radius2;
}

GLfloat WP_Trochoids::fGetRod() const
{
	return rod;
}

bool WP_Trochoids::epitrochoid() const
{
	return !mode;
}
*/
