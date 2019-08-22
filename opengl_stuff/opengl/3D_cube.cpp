#include <GL/glut.h>
#include <iostream>


double rot_x = 2;
double rot_y = 25;

void render( int a ){

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glRotatef( rot_y, 0, 1, 0 );
	glRotatef( rot_x, 1, 0, 0 );

glTranslatef( -0.5, -0.5, -0.5);
	
	glBegin( GL_QUADS );
		// z = 1
		glColor3f( 0.0, 0.0, 1.0 );
		glVertex3f( 0.0, 1.0, 1.0 );
		glVertex3f( 1.0, 1.0, 1.0 );
		glVertex3f( 1.0, 0.0, 1.0 );
		glVertex3f( 0.0, 0.0, 1.0 );
		// z = 0
		glColor3f( 0.0, 1.0, 1.0 );
		glVertex3f( 0.0, 1.0, 0.0 );
		glVertex3f( 1.0, 1.0, 0.0 );
		glVertex3f( 1.0, 0.0, 0.0 );
		glVertex3f( 0.0, 0.0, 0.0 );
		// x = 1
		glColor3f( 0.0, 1.0, 0.0 );
		glVertex3f( 1.0, 0.0, 0.0 );
		glVertex3f( 1.0, 1.0, 0.0 );
		glVertex3f( 1.0, 1.0, 1.0 );
		glVertex3f( 1.0, 0.0, 1.0 );
		// x = 0
		glColor3f( 1.0, 0.0, 1.0 );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( 0.0, 0.0, 1.0 );
		glVertex3f( 0.0, 1.0, 1.0 );
		glVertex3f( 0.0, 1.0, 0.0 );
		// y = 1
		glColor3f( 1.0, 0.0, 0.0 );
		glVertex3f( 0.0, 1.0, 0.0 );
		glVertex3f( 0.0, 1.0, 1.0 );
		glVertex3f( 1.0, 1.0, 1.0 );
		glVertex3f( 1.0, 1.0, 0.0 );
		// y = 0
		glColor3f( 1.0, 1.0, 0.0 );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( 0.0, 0.0, 1.0 );
		glVertex3f( 1.0, 0.0, 1.0 );
		glVertex3f( 1.0, 0.0, 0.0 );

	glEnd();

	glutSwapBuffers();
	

	rot_x += 1;
	//rot_y += 1;
	
	glutTimerFunc( 10, render, 0 );
}



void display_callback( void ){



	glClearColor( 0.0, 0.0, 0.0, 1);
	glMatrixMode( GL_MODELVIEW );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable(GL_DEPTH_TEST);

	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glTranslatef( -0.5, -0.5, -0.5);
	glRotatef( rot_y, 0, 1, 0 );
	glRotatef( rot_x, 1, 0, 0 );

	glBegin( GL_QUADS );
		// z = 1
		glColor3f( 0.0, 0.0, 1.0 );
		glVertex3f( 0.0, 1.0, 1.0 );
		glVertex3f( 1.0, 1.0, 1.0 );
		glVertex3f( 1.0, 0.0, 1.0 );
		glVertex3f( 0.0, 0.0, 1.0 );
		// z = 0
		glColor3f( 0.0, 1.0, 1.0 );
		glVertex3f( 0.0, 1.0, 0.0 );
		glVertex3f( 1.0, 1.0, 0.0 );
		glVertex3f( 1.0, 0.0, 0.0 );
		glVertex3f( 0.0, 0.0, 0.0 );
		// x = 1
		glColor3f( 0.0, 1.0, 0.0 );
		glVertex3f( 1.0, 0.0, 0.0 );
		glVertex3f( 1.0, 1.0, 0.0 );
		glVertex3f( 1.0, 1.0, 1.0 );
		glVertex3f( 1.0, 0.0, 1.0 );
		// x = 0
		glColor3f( 1.0, 0.0, 1.0 );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( 0.0, 0.0, 1.0 );
		glVertex3f( 0.0, 1.0, 1.0 );
		glVertex3f( 0.0, 1.0, 0.0 );
		// y = 1
		glColor3f( 1.0, 0.0, 0.0 );
		glVertex3f( 0.0, 1.0, 0.0 );
		glVertex3f( 0.0, 1.0, 1.0 );
		glVertex3f( 1.0, 1.0, 1.0 );
		glVertex3f( 1.0, 1.0, 0.0 );
		// y = 0
		glColor3f( 1.0, 1.0, 0.0 );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( 0.0, 0.0, 1.0 );
		glVertex3f( 1.0, 0.0, 1.0 );
		glVertex3f( 1.0, 0.0, 0.0 );

	glEnd();
	
	glutSwapBuffers();
	glutTimerFunc( 10, render, 0 );
}





void reshape_callback( int x, int y ){

	glMatrixMode( GL_MODELVIEW );
	
	glViewport( 0, 0, x, y );
	gluPerspective( 40.0, (double)x/(double)y, 1, 20 );
	//gluPerspective( 40.0, 1, 0, 1);
	


}


int main( int argc, char * * argv ){

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowPosition( 100, 100 );

	glutCreateWindow("test");
	glutDisplayFunc( display_callback );
	glutReshapeFunc( reshape_callback );



	glutMainLoop();

 	return 0;
}
