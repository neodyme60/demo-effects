/* Copyright (C) 2002 W.P. van Paassen - peter@paassen.tmfweb.nl

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

/*note that the code has not been optimized*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "tdec.h"
#include "WPCG.h"

//redefine width and height
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

WP_GLState *state;
WP_Camera *cam;
WP_ObjectManager *manager;
WP_Model *model;
scalar heading = 0.0;

void quit( int code )
{      
  /*
   * Quit SDL so we can release the fullscreen
   * mode and restore the previous video settings,
   * etc.
   */
  
  SDL_Quit( );

  TDEC_print_fps();
  
  /* Exit program. */
  exit( code );
}

void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym )
      {
      case SDLK_ESCAPE:
	quit(1);
        break;
      default:
        break;
      }
}

void process_events( void )
{
  /* Our SDL event placeholder. */
  SDL_Event event;
  
  /* Grab all the events off the queue. */
  while( SDL_PollEvent( &event ) ) {
    
    switch( event.type ) {
    case SDL_KEYDOWN:
      /* Handle key presses. */
      handle_key_down( &event.key.keysym );
      break;
    case SDL_QUIT:
      /* Handle quit requests */    
      quit(1);
      break;
    }
  }
}

void draw_screen( void )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  WP_DynamicObject *demon = manager->getDynamicObject();
  demon->setNewRoll(heading); //FIXME the model's orientation should be changed in the MD2 file so the heading can be used instead of roll
  heading += 0.6;
  if (heading >= 360.0)
    heading -= 360.0;
  manager->drawObjects();

  SDL_GL_SwapBuffers( );
}

void init()
{
  Uint16 i;
  
  state = WP_GLState::getInstance();

  WP_TextureManager::getInstance()->mipmapping = true;

  glShadeModel(GL_SMOOTH);
  state->enableDepthTest();
  glCullFace(GL_BACK);
  state->enableCulling();
	
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_DECAL
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //GL_FASTEST
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	
  //state->enableNormalize();
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
  WP_Light light;
  light.setPosition(0.0, 3.0, 0.0, 1.0);
  light.color = WP_Color((float).75,(float).75,(float).75);
  glLightfv(GL_LIGHT0, GL_POSITION, light.getPointPosition()->data);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light.color.components);

  state->enableLighti(0);

  state->enableLighting();

  manager = WP_ObjectManager::getInstance();

  cam = WP_Camera::getInstance();
  
  WP_Point3D eye(0.0, 0.0, 7.0);
  WP_Point3D look(0.0, 0.0, 0.0);
  WP_Vector3D up(0.0, 1.0, 0.0);
  
  cam->setFrustumAndCamera(60.0, ((float)SCREEN_WIDTH) / ((float)SCREEN_HEIGHT), 0.1f, 100.0f, eye, look, up);
	
  cam->screen_width = SCREEN_WIDTH;
  cam->screen_height = SCREEN_HEIGHT;

  WP_Init init;
  init.vSetViewPort(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);	
	

  //set background color
  glClearColor(0.0, 0.0, 0.4, 0.0f);

  /*disable events */
  for (i = 0; i < SDL_NUMEVENTS; ++i) {
    if (i != SDL_KEYDOWN && i != SDL_QUIT) {
      SDL_EventState(i, SDL_IGNORE);
    }
  }
  
  SDL_ShowCursor(SDL_DISABLE);
}

int main( int argc, char* argv[] )
{
  if (argc > 1) {
    printf("Retro Quake2 Model - W.P. van Paassen - 2002\n");
    return -1;
  }
  
  TDEC_init_video();

  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  
  if (!TDEC_set_video_GL(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_DOUBLEBUF | SDL_HWACCEL | SDL_HWSURFACE | SDL_HWPALETTE  
			 /*| SDL_FULLSCREEN*/))
    quit(1);
  
  TDEC_init_timer();
  
  SDL_WM_SetCaption("Retro Quake2 Model effect ", "");
  
  init();

  // add quake2 demon model  
  manager->createDynamicObject(WP_Matrix3D(Y_ROTATION_MATRIX, 90.0) * WP_Matrix3D(X_ROTATION_MATRIX, -90.0), "Demon", "../MODELS/tris1.MD2",
WP_Vector3D(0.1,0.1,0.1)); 
  /* time based demo loop */
  while( 1 ) 
    {
      TDEC_new_time();
    
      process_events();

      if (TDEC_fps_ok()) 
	{
	  draw_screen();
	}
    }
  
  return 0; /* never reached */
}
