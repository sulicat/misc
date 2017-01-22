// first attemp at dijktras shortest path algorithm

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <unistd.h>

#define INFINITE 999999999999
#define PI 3.1452


// these are some of the function prototypes
// the rest are below the classes due to program read order

void display_callback( void );
void render( int );
void init_glut( void );
void resize_callback( int, int );
void draw_circle( float, float, float, float );
void mouse_callback( int, int, int ,int );
void passive_mouse_motion_callback( int, int );

// a MAJOR class, nodes are the circles on the screen.

class Node{
public:
	int id;
	float x;
	float y;
	float r;
	std::string status;
	float distance_from_start;
	float distance_from_start_sqr;
	bool is_path;
	bool visited;
	bool is_start;
	bool is_end;
	bool hover;
	Node * linked_to;
	int linked_to_len;


	// to create a node, all it needs is an x and y
	void init( float _x, float _y ){
		id = 10;
		x = _x;
		y = _y;
		is_path = false;
		visited = false;
		distance_from_start = INFINITE;
		distance_from_start_sqr = INFINITE;
		r = 5;
		linked_to = new Node[ 0 ];
		linked_to_len = 0; 
		is_start = false;
		is_end = false;
		hover = false;
	}

	// must give unique id, makes future programming easier
	void set_id( int x ){
		id = x;
	}

	void set_distance( float _d ){
		distance_from_start = _d;
	}
	void set_distance_sqr( float y){
		distance_from_start_sqr = x;	
	}

	void to_string( void ){
		std::cout << " x: \t" << x << " \t\ty: \t" << y << "\n";
		std::cout << " dist: \t" << distance_from_start << " \t\tvisited ? : \t" << visited << "\n";
		std::cout << "\n";
	}

	// draw depending on state
	void draw( void ){
		if( is_start ){
			glColor3f( 0.0, 1.0, 0.0 );
			draw_circle( x, y, r*2, 30);
		}else if(is_end){
			glColor3f( 1.0, 1.0, 0.0 );
			draw_circle( x, y, r*2, 30);
		}else if( hover ){
			glColor3f( 0.0, 1.0, 1.0 );
			draw_circle( x, y, r*3, 30);
		}else if( visited ){
			glColor3f( 1.0, .0, 1.0 );
			draw_circle( x, y, r, 30);
		}else{
			glColor3f( 1.0, 1.0, 1.0 );
			draw_circle( x, y, r, 30);
		}
	}

	void add_link( Node &n1 ){
		Node * temp_arr = new Node[ linked_to_len + 1 ];
		std::copy( linked_to, linked_to + linked_to_len, temp_arr );
		linked_to = temp_arr;
		linked_to_len += 1;

		linked_to[ linked_to_len - 1 ] = n1;
	}

	void set_end( bool a ){
		is_end = a;
	}


};

// the other MAJOR class..... contains two nodes, represents a line
class Link{
public:
	float length;
	Node * n1;
	Node * n2;

	void init( Node * one, Node * two ){
		n1 = one;
		n2 = two;
	}

	void draw( void ){

		glLineWidth( 3 );
		glColor3f( 0.2, 0.2, 0.2 );

		glBegin( GL_LINES );
			glVertex3f( n1->x, n1->y, 0.0 );
			glVertex3f( n2->x, n2->y, 0.0 );
		glEnd();
	}

};

// the rest of the function prototypes.....
// they require the classes so they are here
void add_node( float, float );
void setup_nodes( void );
void add_link( Node *, Node * );
Node * return_connected_nodes( Node * n, Link * l, int l_len );
Node * dijktras_shortest_path( Node * n, int n_len, Link * l, int l_len );
int get_dist_sqr( float x1, float y1, float x2, float y2 );

// create the global vars
Node * nodes = new Node[ 0 ];
int nodes_len = 0;
Link * links = new Link[ 0 ];
int links_len = 0;

int window_width = 600;
int window_height = 400;
int frame_delay = 10;
int click = 0;

/*===============================================================================================*/
// MAIN
/*===============================================================================================*/
int main( int argc, char * * argv ){
	
	glutInit( &argc, argv );
	glutInitWindowSize( window_width, window_height );
	glutInitWindowPosition( 0, 0 );
	glutInitDisplayMode( GLUT_DOUBLE );
	glutCreateWindow( " shortest path 1 " );

	srand( 2 );

	init_glut( );	
	glutDisplayFunc( display_callback );
	glutReshapeFunc( resize_callback );	
	glutMouseFunc( mouse_callback );
	glutPassiveMotionFunc( passive_mouse_motion_callback );

	glutMainLoop();

	return 0;
}
/*===============================================================================================*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// settings

// glut setup
void init_glut( void ){
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glViewport( 0.0, 0.0, window_width, window_height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();	
	
	glOrtho( 0.0, window_width, window_height, 0.0, 1.0, -1.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}


// when we resize window, setup again
void resize_callback( int x, int y ){
	window_width = x;
	window_height = y;
	init_glut();
}


// add a node to the global nodes array.
// this function = BAD programming..... not general enough use :/ sorry guys
void add_node( float x, float y ){
	Node * temp_arr = new Node[ nodes_len + 1 ];
	std::copy( nodes, nodes + nodes_len, temp_arr );
	nodes = temp_arr;
	nodes_len += 1;

	nodes[ nodes_len - 1 ].init( x, y);
}


// setup the nodes array
void setup_nodes(){
	nodes = new Node[ 0 ];
	nodes_len = 0;
	for( int i = 0; i < 20; i++ ){
		add_node( (int)(window_width * 0.1) + rand() % (int)(window_width*0.8), (int)(window_height * 0.1) + rand() % (int)(window_height*0.8) );
		nodes[ nodes_len - 1 ].set_id( i );
	}
}


// create a new link using two nodes
// again ..... sorry for the bad code :'( making general reusable functions would have been better. FIX LATER
void add_link( Node * n1, Node * n2 ){
	Link * temp_arr = new Link[ links_len + 1 ];
	std::copy( links, links + links_len, temp_arr );
	links = temp_arr;
	links_len += 1;

	links[ links_len - 1].init( n1, n2 );
	links[ links_len - 1].length = get_dist_sqr( n1->x, n1->y, n2->x, n2->y );
}

// I like this one :)
int get_dist_sqr( float x1, float y1, float x2, float y2 ){
	return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

// this one is a good one too ! maybe could be optimized a bit
// this function will take in the nodes array and decide where to create links
void setup_links( Node * nds, int nds_len, float min_dstance ){
	links = new Link[ 0 ];
	links_len = 0;
	for( int i = 0; i < nds_len; i++ ){
		for( int j = 0; j < nds_len; j++){
			bool add = true;
			if( i != j && get_dist_sqr( nds[i].x, nds[i].y, nds[j].x, nds[j].y) < min_dstance*min_dstance ){
				for( int x = 0; x < nds[j].linked_to_len; x++ ){
					if( nds[j].linked_to[x] == nds[i] ){
						add = false;
						//std::cout << "the same \n";
					}else{

						//std::cout << &nds[j].linked_to[x] << " " << &nds[i] << " \n";
					}
				}

				if( add ){
					add_link( &(nds[i]), &(nds[j]) );
					nds[i].add_link( nds[j] );
					nds[j].add_link( nds[i] );
				}
			}

		}
	}

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// render

// here is where we initialize our arrays.
// not in main because we need to have the window size first 
void display_callback( void ){
	
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );

	setup_nodes();
	setup_links( nodes, nodes_len, 200);

	for( int i = 0; i < links_len; i++ ){
		links[ i ].draw();
	}

	for( int i = 0; i < nodes_len; i++ ){
		nodes[ i ].draw();
	}
	
	//return_connected_nodes( &nodes[1], links, links_len );
	//dijktras_shortest_path( nodes, nodes_len, links, links_len );
	
	glutSwapBuffers();	
	glutTimerFunc( frame_delay, render, 0  );
}

void render( int a ){
	glClear( GL_COLOR_BUFFER_BIT );


	for( int i = 0; i < links_len; i++ ){
		links[ i ].draw();
	}

	for( int i = 0; i < nodes_len; i++ ){
		nodes[ i ].draw();
	}

	glutSwapBuffers();
	glutTimerFunc( frame_delay, render, 0  );

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// input


void mouse_callback( int button, int state, int x, int y ){
	//std::cout << button << " " << state << " " << x << " " << y << "\n";
	if( state == 0 ){
		if( click == 2 ){
			for( int i = 0; i < nodes_len; i++ ){ 
				nodes[i].is_start = false;
				nodes[i].is_end = false; 
				click = 0;
			}
		}

		for( int i = 0; i < nodes_len; i++ ){
			if( (x - nodes[i].x)*(x - nodes[i].x) + (y - nodes[i].y)*(y - nodes[i].y) < nodes[i].r * nodes[i].r ){
				if( click == 0 ){
					nodes[i].is_start = true;
					click = 1;
				}else if( click == 1 ){
					nodes[i].is_end = true;
					click = 2;
					dijktras_shortest_path( nodes, nodes_len, links, links_len );
				}
			}
		}
	}
}

void passive_mouse_motion_callback( int x, int y ){
	//std::cout << x << " " << y << "\n";

	for( int i = 0; i < nodes_len; i++ ){
		nodes[i].hover = false;
		if( (x - nodes[i].x)*(x - nodes[i].x) + (y - nodes[i].y)*(y - nodes[i].y) < nodes[i].r * nodes[i].r ){
			nodes[i].hover = true;
		}else{ nodes[i].hover = false; }

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
// logic part

// return an array of adjacent nodes
// WIP
Node * return_connected_nodes( Node * n, Link * l, int l_len ){
	Node * connections = new Node[ 0 ];
	int connections_len = 0;

	for( int i = 0; i < l_len; i++){
		if(l[i].n1->id == n->id){
			//l[i].n2->is_start = true;
			
		}else if( l[i].n2->id == n->id ){
			//l[i].n1->is_start = true;
		}
	}
}

void add_to_array(){


}


Node * dijktras_shortest_path( Node * n, int n_len, Link * l, int l_len ){
	Node * path = new Node[0];
	int path_len = 0;
/*

	float shortest_distance = 0;
	Node * n_start;
	Node * n_end;
	Node * current_node;*/

	// first we must find the starting and ending point
/*	for( int i = 0; i < n_len; i++){
		if( n[i].is_end == true ){	
			n_end = &(n[i]);
		}else if( n[i].is_start == true ){
			n_start = &(n[i]);
			current_node = &(n[i]);
		}
	}
*/
	/* algorithm
	- set start to visited
	- look for closest node
	*/
/*
	current_node->visited = true;
	current_node->distance_from_start = 0;

	for( int i = 0; i < current_node->linked_to_len; i++ ){
		if( current_node->linked_to[i].visited == false ){
			linked_to[i].visited = true;
			usleep(500000);			
		}
	}
*/
/*
	usleep(500000);
	n[1].visited = true;
	usleep(500000);
	n[2].visited = true;
*/
	
	return path;


}




