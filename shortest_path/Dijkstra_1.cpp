// first attemp at dijktras shortest path algorithm

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#define INFINITE 999999999999
#define PI 3.1452


void display_callback( void );
void render( int );
void init_glut( void );
void resize_callback( int, int );
void draw_circle( float, float, float, float );
void mouse_callback( int, int, int ,int );
void passive_mouse_motion_callback( int, int );


class Node{
public:
	int id;
	float x;
	float y;
	float r;
	std::string status;
	float distance_from_start;
	bool is_path;
	bool visited;
	bool is_start;
	bool is_end;
	bool hover;
	int * linked_to;
	int linked_to_len;


	void init( float _x, float _y ){
		id = 10;
		x = _x;
		y = _y;
		is_path = false;
		visited = false;
		distance_from_start = INFINITE;
		r = 5;
		linked_to = new int[ 0 ];
		linked_to_len = 0; 
		is_start = false;
		is_end = false;
		hover = false;
	}

	void set_id( int x ){
		id = x;
	}

	void set_distance( float _d ){
		distance_from_start = _d;
	}

	void to_string( void ){
		std::cout << " x: \t" << x << " \t\ty: \t" << y << "\n";
		std::cout << " dist: \t" << distance_from_start << " \t\tvisited ? : \t" << visited << "\n";
		std::cout << "\n";
	}

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
		}else{
			glColor3f( 1.0, 1.0, 1.0 );
			draw_circle( x, y, r, 30);
		}
	}

	void add_link( Node &n1 ){
		int * temp_arr = new int[ linked_to_len + 1 ];
		std::copy( linked_to, linked_to + linked_to_len, temp_arr );
		linked_to = temp_arr;
		linked_to_len += 1;

		linked_to[ linked_to_len - 1 ] = (long)&n1;
	}

};

class Link{
public:
	float length;
	Node n1;
	Node n2;

	void init( Node one, Node two ){
		n1 = one;
		n2 = two;
	}

	void draw( void ){

		glLineWidth( 3 );
		glColor3f( 0.2, 0.2, 0.2 );

		glBegin( GL_LINES );
			glVertex3f( n1.x, n1.y, 0.0 );
			glVertex3f( n2.x, n2.y, 0.0 );
		glEnd();
	}

};


void add_node( float, float );
void setup_nodes( void );
void add_link( Node, Node );
Node * return_connected_nodes( Node * n, Link * l, int l_len );
Node * dijktras_shortest_path( Node * n, int n_len, Link * l, int l_len );


// create the global vars
Node * nodes = new Node[ 0 ];
int nodes_len = 0;
Link * links = new Link[ 0 ];
int links_len = 0;

int window_width = 600;
int window_height = 400;
int frame_delay = 10;
int click = 0;

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

void add_node( float x, float y ){
	Node * temp_arr = new Node[ nodes_len + 1 ];
	std::copy( nodes, nodes + nodes_len, temp_arr );
	nodes = temp_arr;
	nodes_len += 1;

	nodes[ nodes_len - 1 ].init( x, y);
}





void setup_nodes(){
	nodes = new Node[ 0 ];
	nodes_len = 0;
	for( int i = 0; i < 20; i++ ){
		add_node( (int)(window_width * 0.1) + rand() % (int)(window_width*0.8), (int)(window_height * 0.1) + rand() % (int)(window_height*0.8) );
		nodes[ nodes_len - 1 ].set_id( i );
	}
}


void add_link( Node n1, Node n2 ){
	Link * temp_arr = new Link[ links_len + 1 ];
	std::copy( links, links + links_len, temp_arr );
	links = temp_arr;
	links_len += 1;

	links[ links_len - 1].init( n1, n2 );

}

int get_dist_sqr( float x1, float y1, float x2, float y2 ){
	return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

void setup_links( Node * nds, int nds_len, float min_dstance ){
	links = new Link[ 0 ];
	links_len = 0;
	for( int i = 0; i < nds_len; i++ ){
		for( int j = 0; j < nds_len; j++){
			bool add = true;
			if( i != j && get_dist_sqr( nds[i].x, nds[i].y, nds[j].x, nds[j].y) < min_dstance*min_dstance ){
				for( int x = 0; x < nds[j].linked_to_len; x++ ){
					if( nds[j].linked_to[x] == (long)&nds[i] ){
						add = false;
						//std::cout << "the same \n";
					}else{

						//std::cout << &nds[j].linked_to[x] << " " << &nds[i] << " \n";
					}
				}

				if( add ){
					add_link( nds[i], nds[j] );
					nds[i].add_link( nds[j] );
					nds[j].add_link( nds[i] );
				}
			}

		}
	}

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// render

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
	return_connected_nodes( &nodes[0], links, links_len );

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
	std::cout << button << " " << state << " " << x << " " << y << "\n";
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
				}
			}
		}
	}
}

void passive_mouse_motion_callback( int x, int y ){
	std::cout << x << " " << y << "\n";

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


Node * return_connected_nodes( Node * n, Link * l, int l_len ){
	Node * connections = new Node[ 0 ];
	int connections_len = 0;

	for( int i = 0; i < l_len; i++){
		if( l[i].n1.id == n->id || l[i].n2.id == n->id){
			n->is_end = true;
			l[i].n1.is_end = true;
			l[i].n2.is_end = true;
			std::cout << "yes \n";
		}
			//std::cout << l[i].n1.id << " " << l[i].n2.id << " " << n.id << "\n";
	}

}

Node * dijktras_shortest_path( Node * n, int n_len, Link * l, int l_len ){
	Node * path = new Node[0];
	int path_len = 0;

	return path;


}




