#include <stdio.h>
#include <iostream>
#include "../sulicat/stack.h"


class student{
public:
	int x;
	int y;

	student( ){
		x = 0;
		y = 1;
	}
};



int main( int argc, char * * argv ){
	
	Stack <student> s;
	student st;
	std::cout << "\n\n\n";
	std::cout << "initial memory " << &st << " \n";
	

	try{
		std::cout << s.pop().x << "\n";
	}catch( ... ){
		std::cout << "popped out of an empty array \n";
	}

	s.push( st );
	s.push( st );
	s.push( st );
	s.push( st );

	student * arr = s.give_contents_copy();
	int size = s.length();

	for( int i = 0; i < size; i++ ){
		std::cout << arr[i].x << " : " << &arr[i] << " \n";
	}

	std::cout << "\n";

	arr = s.give_contents();
	for( int i = 0; i < size; i++ ){
		std::cout << arr[i].x << " : " << &arr[i] << " \n";
	}

	s.print();

	std::cout << "\n\n\n";
}



