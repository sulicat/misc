#include <stdio.h>
#include <iostream>


class test_object{
public:
	int id;
};

//void add_object_standard( test_object * obj, int * obj_len, test_object * add );

test_object * objects = new test_object[ 10 ];
int objects_len = 10;

void add_object_standard( test_object * obj, int * obj_len, test_object * add ){

	//std::cout << " setting " << &obj[ *obj_len - 1 ] << " = " << add << "\n";

	//std::cout << " mem adress of obj to add: " << &add << "\n";
	
	test_object * temp = new test_object[ *obj_len + 1 ];
	std::copy( obj, obj + *obj_len, temp );
	obj = temp;
	*obj_len += 1;
	 = *add;



}


int main( int argc, char * * argv ){
	for( int i = 0; i < objects_len; i++ ){
		objects[ i ].id = i;
	}

	std::cout << "\n\n\n";

	test_object add;
	add.id = 3;
	std::cout << " mem adress of obj to add: " << &add << "\n";


	test_object * temp = new test_object[ objects_len + 1 ];
	std::copy( objects, objects + objects_len, temp );
	objects = temp;
	objects_len += 1;
	(objects[ objects_len - 1 ]) = add;

	//add_object_standard( objects, &objects_len, &(add) );

	
	std::cout << "\n" << &add.id << "\t@\t" << (&objects[ objects_len - 1 ].id) << "\n\n";


	for( int i = 0; i < objects_len; i++ ){
		//objects[ i ].id = i;
		std::cout << objects[ i ].id << "\t@\t" << (&objects[ i ]) << "\n";
	}

}



