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

#ifndef __COALESCED_HASHING_H_H__
#define __COALESCED_HASHING_H_H__

#include "const_ref.h"
#include <vector>
using std::vector;

/*--- Structure used during search. ---*/
struct SearchedResult {

	/* Stores the number of probes taken to find the element. ---*/
	int probes;

	/* Stores the position where the element is found.
	 * If the number of probes is zero, then
	 * this position is the position of the
	 * last element within the probe chain.
	*/
	size_t pos;
};

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

template < class Object >
class coalesced_hashing {

	public:

		/*--- Constructor. ---*/
		coalesced_hashing( int size, bool eisch );

		/*--- Constructor. ---*/
		coalesced_hashing( int size, bool eich, const double & addressFactor );

		/*--- Insert into the table. ---*/
		void insert( const Object & object );

		/* Removes the item from the table.
		 * This function is not currently implemented
		 * at this time.
		*/
		int remove( const Object& object );

		/*--- Find an item from the table. ---*/
		const_ref< Object > find( const Object & object ) const;

		/*--- Empty the table logically. ---*/
		void clear( );

		/*--- Empties the table physically. ---*/
		void empty( );

		/*--- Returns the number of items currently within the table. ---*/
		int elements( ) const;

		/*--- Returns the size of the table. ---*/
		size_t size( ) const;

	private: /*--- Private Functions. ---*/

		/*--- Returns the position for the given object. ---*/
		int findPos( const Object & obj ) const;

		/* Searches the given object starting at the given
		 * position till the end of probe chain.
		*/
		SearchedResult findInProbeChain( const Object & obj, size_t pos ) const;

		/*--- Returns true if the given number is a prime. ---*/
		bool isPrime( int n );

		/* Function to find the Next Prime.
		 * Assuming n > 0.
		*/
		int nextPrime( int n );

	private: /*--- Private attributes. ---*/

		enum EntryStatus { ACTIVE, REMOVED, EMPTY };

		/*--- To store the Coalesced Hashing Entry. ---*/
		struct CoalescedHashingEntry {

			/*--- Stores the entry. ---*/
			Object object;

			/*--- Link position within chain. ---*/
			size_t linkpos;

			/*--- Stores the entry status. ---*/
			EntryStatus status;

			/*--- Constructor. ---*/
			CoalescedHashingEntry( const Object & obj = Object( ),
				int pos = -1, EntryStatus s = EMPTY ) : object( obj ),
					linkpos( pos ), status( s ) { }

		};

		/*--- Stores the number of entries currently stored. ---*/
		int occupied;

		/*--- Algorith to use, default is late insertion. ---*/
		bool eisch_algorithm;

		/*--- Stores the ratio of the primary area to the total table size. ---*/
		double addressFactor;

		/*--- position to insert the incoming item during insertion. ---*/
		size_t unoccupiedPos;

		/*--- Array to store the Entries. ---*/
		vector< CoalescedHashingEntry > array;
};

#endif