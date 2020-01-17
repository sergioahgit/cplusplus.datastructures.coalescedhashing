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

#include "const_ref.h"
#include "exceptions.h"
#include <stdlib.h>

/**
 * Class that wraps a constant reference value.  In C++
 * a reference variable is different from pointer
 * since a pointer can point to either an object or NULL.
 * A reference variable can only reference to an object.
 * This implies that if a container is returning a reference
 * to an object we cannot know if the object is found.
 * For this reason, this wrapper is created.
*/

/*--- Default Constructor. ---*/
template< class Object >
const_ref< Object >::const_ref( )
	: object( NULL ), linkpos( 0 ), probes( 0 ) {
}

/*--- Constructor that takes in a reference to a constant object. ---*/
template< class Object >
const_ref< Object >::const_ref( const Object & obj, size_t linkpos, int probes ) {

	/*--- Set the object ---*/
	object = &obj;

	/*--- Set the link position. ---*/
	this->linkpos = linkpos;

	/*--- Set the number of probes. ---*/
	this->probes = probes;
}

/*--- Returns the Object. ---*/
template< class Object >
const Object& const_ref< Object >::getObject( ) const {

	/*--- Check if the Object is not null. ---*/
	if( !isNULL( ) )
		return *object;

	/*--- Throw a NullPointerException. ---*/
	throw NullPointerException( );
}

/*--- Returns the number of probes. ---*/
template< class Object >
int const_ref< Object >::getProbes( ) const {

	return probes;
}

/*--- Returns the link position. ---*/
template< class Object >
size_t const_ref< Object >::getLinkPos( ) const {

	return linkpos;
}

/*--- Returns true if the object points to NULL. ---*/
template< class Object >
bool const_ref< Object >::isNULL( ) const {

	return object == NULL;
}

/*--- Defined equal operator. ---*/
template< class Object >
const const_ref< Object > & const_ref< Object >::operator=( const const_ref< Object > & cref ) {

	/*--- If this is not the same memory location. ---*/
	if( this != &cref ) {

		/*--- Assign the object. ---*/
		object = &cref.getObject( );

		/*--- Set link position. ---*/
		linkpos = cref.getLinkPos( );

		/*--- Set number of probes. ---*/
		probes = cref.getProbes( );
	}

	return *this;
}