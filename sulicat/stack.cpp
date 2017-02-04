// this is a file to create a stack using temlates

#include <iostream>
#include "stack.h"


/*
// stack constructor. init the variables
template < class T >
Stack< T >::Stack( ){
	array = new T[0];
	array_len = 0;
};


// Push a new element to the array
	// create temp array with 1 greater length
	// copy contents over.
	// delete old array
	// set old array to the temp array
	// increment length by 1
template < class T >
void Stack < T >::Push( T element ){
	T * temp = new T[ array_len + 1 ];
	std::copy( array, array + array_len, temp );
	delete array;
	array = temp;
	array_len += 1;

	array[ array_len - 1 ] = element;
};


// POP the last element added the array.
	// create a new temp array with a length of 1 less
	// copy contents before last elemtn over
	// delete the old array
	// set old equual to the temp
	// decrement length by 1
	// return the last element to the user
template < class T >
T Stack< T >::Pop( void ){
	T retval = array[ array_len - 1 ];

	T * temp = new T[ array_len - 1 ];
	std::copy( array, array + array_len - 1, temp );
	delete array;
	array = temp;
	array_len -= 1;

	return retval;
};



template < class T >
void Stack <T> :: print( void ){
	std::cout << "total elements:\t" << array_len << "\n";
};



*/	




