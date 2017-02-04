#ifndef SULICAT_STACK_H
#define SULICAT_STACK_H


#define SULICAT_STACK_EMPTY_EXCEPTION 20

// the stack class
template < class T >
class Stack{
public:
	T * array;
	int array_len;

	// constructor
	// stack constructor. init the variables
	Stack(){
		array = new T[0];
		array_len = 0;
	}
	
	//methods
	

	// Push a new element to the array
		// create temp array with 1 greater length
		// copy contents over.
		// delete old array
		// set old array to the temp array
		// increment length by 1
	void push( T element ){
		T * temp = new T[ array_len + 1 ];
		std::copy( array, array + array_len, temp );
		delete array;
		array = temp;
		array_len += 1;

		array[ array_len - 1 ] = element;
	}


	// POP the last element added the array.
		// create a new temp array with a length of 1 less
		// copy contents before last elemtn over
		// delete the old array
		// set old equual to the temp
		// decrement length by 1
		// return the last element to the user
	T pop( void ){

		if( array_len == 0 ){
			throw SULICAT_STACK_EMPTY_EXCEPTION;
		}

		T retval = array[ array_len - 1 ];

		T * temp = new T[ array_len - 1 ];
		std::copy( array, array + array_len - 1, temp );
		delete array;
		array = temp;
		array_len -= 1;

		return retval;
	}


	T * give_contents(){
		return array;
	}

	// this will return an array with a copy of the stack contents
	T * give_contents_copy( void ){
		T * retval = new T[ array_len ];
		std::copy( array, array + array_len, retval );
		return retval;
	}

	T give_top( void ){
		return array[array_len - 1];
	}

	// this method will return the stack size
	int length( void ){
		return array_len;
	}


	// print number of elements in list	
	void print( void ){ 
		std::cout << "total elements:\t" << array_len << "\n"; 
	}

};

#endif