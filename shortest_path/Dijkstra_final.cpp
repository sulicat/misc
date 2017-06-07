#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "GL/glut.h"


int WINDOW_WIDTH;
int WINDOW_HEIGHT;


class TILE{
public:
	float x;
	float y;
	float width;
	float height;
	bool visited = false;

	TILE(){
		x = 0;
		y = 0;
		width = 0.1;
		height = 0.1;
	}

	TILE( int _x, int _y, int _w, int _h ){
		x = ((float)_x/(float)WINDOW_WIDTH) - 1;
		y = ((float)_y/(float)WINDOW_HEIGHT) - 1;
		width = ((float)_w / (float)WINDOW_WIDTH);
		height = ((float)_h / (float)WINDOW_HEIGHT);
	}


	void render(){

		if(  ){

		}


		glBegin( GL_QUADS );
			glVertex3f( x, 			y 			,0	);
			glVertex3f( x + width, 	y 			,0 	);
			glVertex3f( x + width,	y + height 	,0 	);
			glVertex3f( x, 			y + height 	,0	);
		glEnd();
		

		glLineWidth( 2.5 );
		glBegin( GL_LINES );
			glVertex3f( x, 			y 			,0	);
			glVertex3f( x + width, 	y 			,0 	);

			glVertex3f( x + width, 	y 			,0 	);
			glVertex3f( x + width,	y + height 	,0 	);
			
			glVertex3f( x + width,	y + height 	,0 	);
			glVertex3f( x, 			y + height 	,0	);
			
			glVertex3f( x, 			y + height 	,0	);
			glVertex3f( x, 			y 			,0	);
		
		glEnd();
	}



};






TILE * tiles = new TILE[ 0 ];
int tiles_len = 0;

void add_tile( int x, int y, int width, int height ){
	TILE * temp = new TILE[ tiles_len + 1 ];
	std::copy( tiles, tiles + tiles_len, temp );
	tiles = temp;
	tiles[ tiles_len ] = TILE( x, y, width, height );

	tiles_len += 1;
}











void render( int a ){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	

	
	for( int i = 0; i < tiles_len; i++ ){
		tiles[i].render();
	}


	glutSwapBuffers();

	glutTimerFunc( 10, render, 0 );
}



void callback_display( void ){

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	


	glBegin( GL_QUADS );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( 0.0, 0.5, 0.0 );
		glVertex3f( 0.5, 0.5, 0.0 );
		glVertex3f( 0.5, 0.0, 0.0 );
	glEnd();

	glutSwapBuffers();

	glutTimerFunc( 10, render, 0 );

}




int main( int argc, char * * argv ){


	WINDOW_WIDTH = 1200;
	WINDOW_HEIGHT = 1200;

	glutInit( &argc, argv);

	glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
	glutInitWindowPosition( 2000, 0 );
	glutCreateWindow( "shortest path" );

	glutInitDisplayMode( GL_RGBA | GL_DEPTH | GL_DOUBLE );
	glEnable( GL_DEPTH_TEST );
	glClearColor( 0.0, 0.0, 0.0, 1 );



	int number_tiles_x = 10;
	int number_tiles_y = 10;
	int tiles_width = (WINDOW_WIDTH / number_tiles_x) * 2; 
	int tiles_height = (WINDOW_HEIGHT / number_tiles_y) * 2; 

	//  create the tiles to fit the screen
	for( int r = 0; r < number_tiles_y; r++ ){
		for( int c = 0; c < number_tiles_x; c++ ){
			add_tile( c * tiles_width, r * tiles_height, tiles_width, tiles_height );
		}
	}



	

	glutDisplayFunc( callback_display );


	glutMainLoop();

	return 0;
}