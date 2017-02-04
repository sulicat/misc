#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "../sulicat/stack.h"

#define PI 3.145211
#define INFINITE 999999999

void display_callback( void );
void render( int );
void init_glut( void );
void resize_callback( int, int );
void draw_circle( float, float, float, float );
void mouse_motion_callback( int x, int y );
void mouse_callback( int button, int state, int x, int y );

int window_width = 600;
int window_height = 400;
int frame_delay = 10;

class Node{
public:
	float x;
	float y;
	float weight;
	float radius;
	float r;
	float g;
	float b;
	int id;
	
	Node * linked_to;
	int linked_to_len;
	bool * draw_flag;
	int draw_flag_len;

	bool hover;
	bool is_start;
	bool is_end;

	bool visited;


	void init( void ){
		x = 0;
		y = 0;
		r = 1;
		g = 1;
		b = 1;
		radius = 10;
		weight = INFINITE;

		linked_to = new Node[ 0 ];
		linked_to_len = 0;
		draw_flag = new bool[ 0 ];
		draw_flag_len = 0;

		hover = false;
		is_end = false;
		is_start = false;
		visited = false;
	}


	void draw( void ){
					

			if( hover ){
				glColor3f( 1.0, 1.0, 0 );
				glLineWidth( 4 );
			}else if( is_start ){
				glColor3f( 0, 1.0, 0 );
				glLineWidth( 2);
			}else if( is_end ){
				glColor3f( 0, 1.0, 1.0 );
				glLineWidth( 2);
			}else{
				glColor3f( r, g, b );
				glLineWidth( 2 );
			}

			draw_circle( x, y, radius, 30);

			for( int i = 0; i < linked_to_len; i++ ){
				glColor3f( r, g, b );

				if( hover ){
					glColor3f( 1.0, 0, 1.0 );
					glBegin( GL_LINES );
						glVertex3f( x, y, 0.0 );
						glVertex3f( linked_to[i].x, linked_to[i].y, 0.0 );
					glEnd();
				}else if( draw_flag[ i ] == true ){
					glBegin( GL_LINES );
						glVertex3f( x, y, 0.0 );
						glVertex3f( linked_to[i].x, linked_to[i].y, 0.0 );
					glEnd();

				}

			}

	}

	void set_pos( float _x, float _y ){
		x = _x;
		y = _y;
	}

	void add_link( Node * n, bool should_draw ){

		Node * temp = new Node [ linked_to_len + 1 ];
		std::copy( linked_to, linked_to + linked_to_len, temp );
		delete linked_to;
		linked_to = temp;
		linked_to_len += 1;

		linked_to[ linked_to_len - 1 ] = *n;

		bool * temp_2 = new bool[ draw_flag_len + 1 ];
		std::copy( draw_flag, draw_flag + draw_flag_len, temp_2 );
		delete draw_flag;
		draw_flag = temp_2;
		draw_flag_len += 1;

		draw_flag[ draw_flag_len - 1 ] = should_draw;

	}

	void toggle_hover( bool h ){
		hover = h;
	}

	void toggle_hover( ){
		hover = !hover;
	}

};


////////////////////////////////////////////////////////
void add_node( Node * n, int * n_len, float x, float y );
Node * shortest_path( Node * n, int n_len, Node start, Node end );

Node * nodes = new Node[ 0 ];
int nodes_len = 0;
int button_press_count = 0;
Node start;
Node end;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

int main( int argc, char * * argv ){
	
	glutInit( &argc, argv );
	glutInitWindowSize( window_width, window_height );
	glutInitWindowPosition( 0, 0 );
	glutInitDisplayMode( GLUT_DOUBLE );
	glutCreateWindow( "shortest path" );

	srand( 2  );

	init_glut( );	
	glutDisplayFunc( display_callback );
	glutReshapeFunc( resize_callback );	
	glutPassiveMotionFunc( mouse_motion_callback );
	glutMouseFunc( mouse_callback );

	glutMainLoop();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

















///////////////////////////////////////////////////////////////////////////////////////////////////////////
// node methods


int return_dist_sqr( float x1, float y1, float x2, float y2 ){

	return ((x1-x2)*(x1-x2)) + ((y1-y2)*(y1-y2));

}

void add_node( Node * n, int * n_len, float x, float y, int ID){
	
	Node * temp = new Node[ *n_len + 1 ];
	std::copy( n, n + *n_len, temp );
	nodes = temp;
	*n_len += 1;

	nodes[ *n_len - 1 ].init();
	nodes[ *n_len - 1 ].set_pos( x, y );
	nodes[ *n_len - 1 ].id = ID;

	std::cout << "adding node: " << *n_len  << " at x: " << x << "  Y: " << y << "ID: " << nodes[ *n_len - 1 ].id << "\n";
}



void add_node_standard( Node * n, int * n_len, Node * input ){
	
	Node * temp = new Node[ *n_len + 1 ];
	std::copy( n, n + *n_len, temp );
	nodes = temp;
	*n_len += 1;

	nodes[ *n_len - 1 ].init();

	std::cout << "adding node: to array" << *n_len  << "\n";
}



// creates nodes at random spots using add node method
void create_random_nodes( Node * * n, int * n_len, int num ){
	for( int i = 0; i < num; i++ ){
		add_node( *n, n_len, (int)(window_width * 0.1) + rand() % (int)(window_width*0.8), (int)(window_height * 0.1) + rand() % (int)(window_height*0.8), i );
	}

}



// cycle through array of nodes to add links to the ones within the given range
void create_links( Node * n, int n_len, float range ){
	for( int i = 0; i < n_len; i++ ){
		for( int x = i; x < n_len; x++ ){
			if( x != i && return_dist_sqr( n[i].x, n[i].y, n[x].x, n[x].y ) < range*range ){
				//std::cout << "coonnect \n";
				n[i].add_link( &n[x], true );
				n[x].add_link( &n[i], false );				
			}
		}
	}


}




bool is_node_equal(Node n1, Node n2){

	if( n1.id == n2.id ){
		return true;
	}else{
		return false;
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////
// settings

void init_glut( void ){
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glViewport( 0.0, 0.0, window_width, window_height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();	
	
	glOrtho( 0.0, window_width, window_height, 0.0, 1.0, -1.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}


void resize_callback( int x, int y ){
	window_width = x;
	window_height = y;
	init_glut();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// render

void display_callback( void ){
	delete nodes;
	nodes = new Node[ 0 ];
	nodes_len = 0;

	create_random_nodes( &nodes, &nodes_len, 100 );
	create_links( nodes, nodes_len, 100 );

	glClearColor( 1.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );

	

	std::cout << "actual len: " << nodes_len << "\n\n\n\n\n";


	glutSwapBuffers();	
	glutTimerFunc( frame_delay, render, 0  );
}

void render( int a ){

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );

	for( int i = 0; i < nodes_len; i++ ){
		nodes[i].draw();
		//std::cout << " node " << i << " x: " << nodes[i].x << "\n";

	}



	glutSwapBuffers();
	glutTimerFunc( frame_delay, render, 0  );

}










///////////////////////////////////////////////////////////////////////////////////////////////////////////
// input
void mouse_motion_callback( int x, int y ){
	for( int i = 0; i < nodes_len; i++ ){
		if( return_dist_sqr( x, y, nodes[i].x, nodes[i].y ) <= nodes[i].radius*nodes[i].radius ){
			nodes[i].hover = true; 
		}else{
			nodes[i].hover = false;
		}

	}
}


void mouse_callback( int button, int state, int x, int y ){

	if( state == 0 ){
		for( int i = 0; i < nodes_len; i++ ){
			if( return_dist_sqr( x, y, nodes[i].x, nodes[i].y ) <= nodes[i].radius*nodes[i].radius ){
				
				if( button_press_count == 0 ){
					nodes[i].is_start = true;
					button_press_count += 1;
					start = nodes[i];
				}else if( button_press_count == 1 ){
					nodes[i].is_end = true; 
					button_press_count += 1;
					end = nodes[i];
					shortest_path( nodes, nodes_len, start, end );
				}else if( button_press_count == 2 ){
					button_press_count = 0;
					for( int x = 0; x < nodes_len; x++ ){
						nodes[x].is_end = false;
						nodes[x].is_start = false;
					}
				}
			

			}

	}

	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
// drawing funcs

void draw_rect( float x, float y, float width, float height ){
	glBegin( GL_POLYGON );
		glVertex3f( x, y, 0.0 );
		glVertex3f( x, y+height, 0.0 );
		glVertex3f( x+width, y+height, 0.0 );
		glVertex3f( x+width, y, 0.0 );
	glEnd();	
}


void draw_rect_outline( float x, float y, float width, float height, float thickness ){
	glBegin( GL_POLYGON );
		draw_rect( x,y,width, thickness );
		draw_rect( x,y,thickness, height );		
		draw_rect( x+width,y,thickness, height );
		draw_rect( x,y+width,width, thickness );		
	glEnd();	
}


void draw_circle( float x, float y, float r, float angle_interval ){
	glBegin( GL_POLYGON );
		for( int i = 0; i < 360; i += angle_interval ){
			glVertex3f( x + r*cos(i*(PI/180)), y + r*sin(i*(PI/180)), 0.0 );
		}	
	glEnd();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////



Node * shortest_path( Node * n, int n_len, Node current_node, Node end_node ){

	Node * path = new Node[ 0 ];
	int path_len = 0;

	Stack <Node> visited;
	Stack <Node> un_visited;

	current_node.visited = true;
	visited.push(current_node);

	if( is_node_equal(visited.give_top(), current_node) ){
		std::cout << "hello cat\n";
	}

	for( int i = 0; i < n_len; i++ ){
		un_visited.push( n[i] );
		
	}



	return path;


}



