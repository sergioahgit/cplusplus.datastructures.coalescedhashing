/*
* The MIT License(MIT)
*
* Copyright(c) 2019 - 2020 Sergio A. Hernandez, Jr.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files(the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions :
*
* The above copyright noticeand this permission notice shall be included in all copies or substantial
* portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name(s) of the above copyright holders shall not be used in
* advertising or otherwise to promote the sale, use or other dealings in this Software without prior
* written authorization.
*/

#include <math.h>
#include "coalescedhashing.h"
#include "hashingfunction.h"
#include "exceptions.h"

/**
 * A data structure which implements coalesced hashing as collision
 * resolution method for the hash table.
 *
 * => Colliding elements are stored in the same table.
 *    Eferences create chains which are subject to so called coalescence.
 *
 *    LISCH (late insert standard coalesced hashing)
 *    EISCH (early insert standard coalesced hashing)
 *
 *    - Using additional cellar space
 *
 *    LICH (late insert coalesced hashing)
 *    EICH (early insert coalesced hashing)
*/

/*--- Constructor. ---*/
template < class Object >
coalesced_hashing< Object >::coalesced_hashing( int size, bool eisch )
	: array( nextPrime( size ) ), eisch_algorithm( eisch ) {

	/*--- Default address factor. ---*/
	addressFactor = -1.0;

	/*--- Table Size - 1. ---*/
	unoccupiedPos = array.size() - 1;

	/*--- Clear array. ---*/
	clear( );
}

/*--- Constructor. ---*/
template < class Object >
coalesced_hashing< Object >::coalesced_hashing( int size, bool eich, const double & addressFactor)
	: array( nextPrime( size ) ), eisch_algorithm( eich ) {

	/*--- Set address factor. ---*/
	this->addressFactor = addressFactor;

	/*--- Default in case is less than zero or greater than 1. ---*/
	if( ( this->addressFactor < 0.0 ) || ( this->addressFactor > 1.0 ) )
		this->addressFactor = 0.86;

	/*--- Table Size - 1. ---*/
	unoccupiedPos = array.size( ) - 1;

	/*--- Clear array. ---*/
	clear( );
}

/*--- Insert into the table. ---*/
template < class Object >
void coalesced_hashing< Object >::insert( const Object& object ) {

	/*--- Get the position to insert the object. ---*/
	int pos = findPos( object );

	/* Search in the probe chain for the given object
	 * starting at the home address.
	*/
	SearchedResult result = findInProbeChain(object, pos);
	if( result.probes > 0 ) /*--- Not found. ---*/
		throw DuplicateItemException( );

	else { /*--- Try to insert the item. ---*/

		/*--- If there is nothing in the home address. ---*/
		if( result.pos == -1 ) {

			/*--- Insert item. ---*/
			array[ pos ] = CoalescedHashingEntry( object, -1, ACTIVE );

			/*--- Increase occupied variable. ---*/
			occupied++;
		}

		else { /*--- Lets go to the end of the probe chain. ---*/

			/* Find the bottommost empty location in the table.
			 * If none is found, terminate with a "full table"
			 * message.
			*/
			if( unoccupiedPos < 0 )
				throw IsFullException( );

			else {

				/*--- Insert item into the unoccupied location ---*/
				do {

					/*--- If the position is emtpy, break. ---*/
					if( array[ unoccupiedPos ].status != ACTIVE )
						break;

					/*--- Else, decrement the value. ---*/
					unoccupiedPos--;

					/*--- Do, while the unoccupied index is greater than zero. ---*/
				} while( unoccupiedPos > -1 );

				/*--- If the unoccupied index is -1, then the table is full. ---*/
				if( unoccupiedPos < 0 )
					throw IsFullException( );

				else {

					/*--- Else, insert the item. ---*/
					array[ unoccupiedPos ] = CoalescedHashingEntry( object, -1, ACTIVE );

					/* Set the link field of the record at the end of the
					 * chain to point to the location of the newly inserted record.
					*/
					if (!eisch_algorithm)
						array[ result.pos ].linkpos = unoccupiedPos;

					else {

						/* Create a temporary to store the link
						 * of where the home address is pointing to.
						*/
						size_t temppos = array[ pos ].linkpos;

						/*--- Assign the new link position. ---*/
						array[ pos ].linkpos = unoccupiedPos;

						/* Now from the inserted item, assign the
						 * link where the home address used to point
						 * before.
						*/
						array[ unoccupiedPos ].linkpos = temppos;
					}

					/*--- Increase occupied variable. ---*/
					occupied++;
				}
			}
		}
	}
}

/* Removes the item from the table.
 * This function is not currently implemented
 * at this time.
*/
template < class Object >
int coalesced_hashing< Object >::remove( const Object& object ) {

	return -1;
}

/*--- Find an item from the table. ---*/
template < class Object >
const_ref< Object > coalesced_hashing< Object >::find( const Object & object ) const {

	/*--- Get the position to insert the object. ---*/
	int pos = findPos( object );

	/* Search in the probe chain for the given object
	 * starting at the home address.
	*/
	SearchedResult result = findInProbeChain( object, pos );
	if( result.probes == 0 ) /*--- Not found. ---*/
		return const_ref< Object >( );

	else /*--- return the object. ---*/
		return const_ref< Object >( array[ result.pos ].object, array[ result.pos ].linkpos, result.probes );
}

/*--- Empty the table logically. ---*/
template < class Object >
void coalesced_hashing< Object >::clear( ) {

	occupied = 0;

	/*--- Clear the array. ---*/
	for( int i = 0; i < array.size( ); i++ ) {

		/*--- Make the positions logically empty. ---*/
		array[ i ].status = EMPTY;

		/*--- Set link position. ---*/
		array[ i ].linkpos = -1;
	}
}

/*--- Empties the table physically. ---*/
template < class Object >
void coalesced_hashing< Object >::empty( ) {

	occupied = 0;

	/*--- Remove everything. ---*/
	static_cast< void >( array.empty( ) );

	/*--- Resize array. ---*/
	array.resize( 0 );
}

/*--- Returns the number of items currently within the table. ---*/
template < class Object >
int coalesced_hashing< Object >::elements( ) const {

	return occupied;
}

/*--- Returns the size of the table. ---*/
template < class Object >
size_t coalesced_hashing< Object >::size( ) const {

	return array.size( );
}

/* If the given object is an not an integer,
 * this function will get called, else, the function
 * declared at the hashinghunction.h
 * Returns the hashing value associated with the given object.
*/
template < class Object >
int hash( const Object & obj ) {

	return reinterpret_cast< int >( &obj );
}

/*--- Returns the position for the given object. ---*/
template < class Object >
int coalesced_hashing< Object >::findPos( const Object& obj ) const {

	/*--- Get position. ---*/
	int pos = 0;
	if( addressFactor != -1.0 )
		pos = hash( obj ) % ( int )( addressFactor * array.size( ) );

	else
		pos = hash( obj ) % array.size( );

	/*--- Return the position. ---*/
	return pos;
}

/* Searches the given object starting at the given
 * position till the end of probe chain.
*/
template < class Object >
SearchedResult coalesced_hashing< Object >::findInProbeChain( const Object & obj, size_t pos ) const {

	/* Stores the prev item before the item to be remove
	 * within the probe chain.
	*/
	size_t prevlink = -1;

	/*--- To Store the Search Result. ---*/
	SearchedResult result;

	/* To store the number of probes taken
	 * to find the item.
	*/
	result.probes = 0;
	result.pos = 0;

	/*--- Flag is set to true, when the item is found. ---*/
	bool itemFound = false;
	do {

		/*--- Increment number of probes. ---*/
		result.probes++;

		/*--- Check if this position contains the given item. ---*/
		if( ( array[ pos ].status == ACTIVE ) && ( obj == array[ pos ].object ) ) {

			/*--- Item was found. ---*/
			itemFound = true;
			break;
		}

		/*--- Set Previous link position. ---*/
		prevlink = pos;

		/*--- find the next link position. ---*/
		pos = array[ pos ].linkpos;

		/*--- If position is -1, then we reached end of probe chain. ---*/
	}while ( pos > -1 );

	/*--- If the item was not found, then throw an exception. ---*/
	if( !itemFound ) {

		/*--- Check number of probes. ---*/
		if( result.probes == 1 ) { /*--- This means we only went to the home address. ---*/

			/*--- Check if there is something in there. ---*/
			if( array[ prevlink ].status == ACTIVE )
				result.pos = prevlink;

			else /*--- This will implied that the home address is available. ---*/
				result.pos = -1;
		}

		else {

			/*--- Set the last item in the probe chain if it is late insertion. ---*/
			result.pos = prevlink;
		}

		/*--- Set the search result. ---*/
		result.probes = 0;
	}

	else {

		/*--- Set the position where the item is stored. ---*/
		result.pos = pos;
	}

	/*--- Return the search result. ---*/
	return result;
}

/*--- Returns true if the given number is a prime. ---*/
template < class Object >
bool coalesced_hashing< Object >::isPrime( int n ) {

	if( n == 2 )
		return true;

	if( n == 1 || n % 2 == 0 )
		return false;

	/*--- Get the Square Root of n and add 1. ---*/
	int sqrtOf_n = ( int )sqrt( ( double ) n ) + 1;

	/*--- Now, try to check if n is a prime. ---*/
	for( int i = 3; i < sqrtOf_n; i += 2 )
		if( n % i == 0 )
			return false;

	return true;
}

/* Function to find the Next Prime.
 * Assuming n > 0.
*/
template < class Object >
int coalesced_hashing< Object >::nextPrime( int n ) {

	/*--- If it is not even, increment to make it odd. ---*/
	if( n % 2 == 0 )
		n++;

	/*--- Now find the next prime number. ---*/
	for( ; !isPrime( n ); n += 2 )
		;

	return n;
}