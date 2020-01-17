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

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <string.h>
#include "coalescedhashing.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

#define TABLE_SIZE 13093
#define ADDRESS_FACTOR 0.86

/*--- Function to round of a number to the given decimal places. ---*/
double round_func( double number, int places ) {

	double multiplier = pow( 10.0, places );
	double roundValue = floor( number * multiplier + 0.5 );
	roundValue = roundValue / multiplier;
	return roundValue;
}

/*--- Function to insert the integers into the coalesced hashing table. ---*/
void insert( coalesced_hashing< int > & table, const vector< int > & list, int elements ) {

	/*--- For the number of elements. ---*/
	for( int i = 0; i < elements; i++ ) {

		/*--- Insert element. ---*/
		table.insert( list[ i ] );
	}
}

/*--- Save the results to file. ---*/
void saveResults( const coalesced_hashing< int > & table, const vector< int > & list, ofstream & outFile ) {

	/*--- Stores the combine probes for all the items searched for. ---*/
	double totalProbes = 0;

	/*--- Try to find all the items. ---*/
	for( int i = 0; i < table.elements( ); i++ ) {

		/*--- Find the value within the table. ---*/
		const_ref< int > element = table.find( list[ i ] );

		/*--- Check if the element was found. ---*/
		if( !element.isNULL( ) ) {

			/*--- Get the number of probes. ---*/
			totalProbes = totalProbes + element.getProbes( );
		}
	}

	/*--- Print out the mean number of probes. ---*/
	outFile << ( double )( totalProbes / ( double )table.elements( ) );
	outFile << "\t\t";
}

int main( int argc, char* argv[ ] ) {

	/*--- Check the number of arguments. ---*/
	if( argc != 2 ) {

		cout << "Expecting a file list with integers." << endl;
		cout << "In order to run this program, you must supply" << endl;
		cout << "a file name as the first parameter." << endl;
		return 0;
	}

	/*--- Stores the current number being read from the given file. ---*/
	int currentNumber = 0;

	/*--- Stores the given numbers in a list. ---*/
	vector< int > list;

	/*--- Lets Open up the file. ---*/
	ifstream openFile( argv[ 1 ] );
	if( !openFile ) {

		cout << "-->> Cannot open file" << argv[ 1 ] << endl;
		return 0;
	}

	cout << "Reading File, please wait..." << endl;

	/*--- While not end of file.  Read in each number at a time. ---*/
	while( !openFile.eof( ) ) {

		/*--- Read in the numbers from the file. ---*/
		openFile >> currentNumber;

		/*--- Insert it into the list. ---*/
		list.push_back( currentNumber );
	}

	cout << "Finished reading the file." << endl << endl;

	/*--- Close file. ---*/
	openFile.close( );

	/*--- Allocate memory to store the logfile name. ---*/
	size_t logFileSize = strlen(argv[1]) + 4/*.LOG*/ + 1;
	char * logfile = new char[ logFileSize ];

	/*--- Copy the name. ---*/
	strcpy_s( logfile, logFileSize, argv[ 1 ] );
	strcat_s( logfile, logFileSize, ".log" );

	/*--- Open file for writing. ---*/
	ofstream saveFile( logfile );

	/*--- Delete memory. ---*/
	delete[ ] logfile;

	/*---- Create Table. ---*/
	/*---------------------------------------------------------------------------------*/
	saveFile << "--------------------------------------------------------------------";
	saveFile << "----------------------------------------------------" << endl;
	saveFile << "Table 3.1 MEAN NUMBER OF PROBES FOR SUCCESSFUL LOOKUP( TABLE SIZE = ";
	saveFile << TABLE_SIZE << " ) FOR\n VARIANTS OF COALESCED HASHING" << endl;

	saveFile << "  &\t0.2\t\t0.4\t\t0.6\t\t0.8\t\t0.9\t\t0.95\t\t0.99" << endl;
	saveFile << "Method" << endl;
	saveFile << "--------------------------------------------------------------------";
	saveFile << "----------------------------------------------------" << endl;
	/*---------------------------------------------------------------------------------*/

	/*--- For the number of algorithms. ---*/
	int elements = 0;
	for( int algorithm = 0; algorithm < 4; algorithm++ ) {

		if( algorithm == 0 ) {

			cout << "------------------------------------------------" << endl;
			cout << "Executing EISCH Algorithm method, please wait..." << endl;
			saveFile << "EISCH\t";
		}

		else if( algorithm == 1 ) {

			cout << "------------------------------------------------" << endl;
			cout << "Executing LISCH Algorithm method, please wait..." << endl;
			saveFile << "LISCH\t";
		}

		else if( algorithm == 2 ) {

			cout << "------------------------------------------------" << endl;
			cout << "Executing EICH Algorithm method, please wait..." << endl;
			saveFile << "EICH\t";
		}

		else if( algorithm == 3 ) {

			cout << "------------------------------------------------" << endl;
			cout << "Executing LICH Algorithm method, please wait..." << endl;
			saveFile << "LICH\t";
		}

		/*--- Lets create the table 3.1 that is on page 55. ---*/
		for( double packingFactor = 0.2; packingFactor < 1.0; ) {

			/*--- Lets get the number of elements to read in. ---*/
			elements = ( int )round_func( TABLE_SIZE * packingFactor, 0 );

			/*--- Set the algorithm case. ---*/
			switch( algorithm ) {

				case 0: { /*--- This is for EISCH algorithm. ---*/

					/*--- Create the Coalesced Hashing algorithm to use. ---*/
					coalesced_hashing< int > EISCH( TABLE_SIZE, true/*--- For EISCH ---*/ );

					cout << "-->> Insert elements into the EISCH table with packing factor: " << packingFactor << endl;

					/*--- Insert integers from the list into the Coalesced Hashing table. ---*/
					insert( EISCH, list, elements );

					cout << "Done, inserting elements into the EISCH table." << endl << endl;
					cout << "-->> Saving results for the EISCH Algorithm Method." << endl;

					/*--- Saving results. ---*/
					saveResults( EISCH, list, saveFile );

					cout << "Done saving results for the EISCH Algorith Method." << endl;
					cout << "--------------------------------------------------" << endl << endl;

					break;
				}

				case 1: { /*--- This is for LISCH algorithm. ---*/

					/*--- Create the Coalesced Hashing algorithm to use. ---*/
					coalesced_hashing< int > LISCH( TABLE_SIZE, false/*--- For LISCH ---*/ );

					cout << "-->> Insert elements into the LISCH table with packing factor: " << packingFactor << endl;
				
					/*--- Insert integers from the list into the Coalesced Hashing table. ---*/
					insert( LISCH, list, elements );

					cout << "Done, inserting elements into the LISCH table." << endl << endl;
					cout << "-->> Saving results for the LISCH Algorithm Method." << endl;

					saveResults( LISCH, list, saveFile );

					cout << "Done saving results for the LISCH Algorith Method." << endl;
					cout << "--------------------------------------------------" << endl << endl;
					break;
				}

				case 2: { /*--- This is for EICH algorithm. ---*/

					/*--- Create the Coalesced Hashing algorithm to use. ---*/
					coalesced_hashing< int > EICH( TABLE_SIZE, true/*--- For EICH ---*/, ADDRESS_FACTOR );

					cout << "-->> Insert elements into the EICH table with packing factor: " << packingFactor << endl;

					/*--- Insert integers from the list into the Coalesced Hashing table. ---*/
					insert( EICH, list, elements );

					cout << "Done, inserting elements into the EICH table." << endl << endl;
					cout << "-->> Saving results for the EICH Algorithm Method." << endl;

					saveResults( EICH, list, saveFile );

					cout << "Done saving results for the EICH Algorith Method." << endl;
					cout << "--------------------------------------------------" << endl << endl;
					break;
				}

				case 3: { /*--- This is for LICH algorithm. ---*/

					/*--- Create the Coalesced Hashing algorithm to use. ---*/
					coalesced_hashing< int > LICH( TABLE_SIZE, false/*--- For EICH ---*/, ADDRESS_FACTOR);

					cout << "-->> Insert elements into the LICH table with packing factor: " << packingFactor << endl;

					/*--- Insert integers from the list into the Coalesced Hashing table. ---*/
					insert( LICH, list, elements );

					cout << "Done, inserting elements into the LICH table." << endl << endl;
					cout << "-->> Saving results for the LICH Algorithm Method." << endl;

					saveResults( LICH, list, saveFile );

					cout << "Done saving results for the LICH Algorith Method." << endl;
					cout << "--------------------------------------------------" << endl << endl;
					break;
				}
			}

			/*--- If the Packing Factor is 0.8. ---*/
			if( packingFactor == 0.8 ) {

				/*--- Set the packing factor. ---*/
				packingFactor = 0.9;
				continue;
			}

			/*--- If the Packing Factor is 0.9. ---*/
			else if( packingFactor == 0.9 ) {

				/*--- Set the packing factor. ---*/
				packingFactor = 0.95;
				continue;
			}

			/*--- If the Packing Factor is 0.9. ---*/
			else if( packingFactor == 0.95 ) {

				/*--- Set the packing factor. ---*/
				packingFactor = 0.99;
				continue;
			}

			/*--- If the Packing Factor is 0.99. ---*/
			else if( packingFactor == 0.99 )
				break; /*--- Lets get out from this loop. ---*/

			else
				packingFactor += 0.2;
		}

		/*--- Go to the next line. ---*/
		saveFile << endl;
	}

	/*--- Close the file. ---*/
	saveFile.close( );

	return 0;
}
