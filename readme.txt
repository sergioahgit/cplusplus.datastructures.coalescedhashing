		Run Instructions
		--------------------------------

To run the "app = datastructures.coalescedhashing.exe" executable

	-->> At the shell prompt type: ./app list.txt
	     where list.txt is the file that contains the numbers to insert into the coalesced hashing tables.
		 The file 'list.txt' is located in the "files" folder within the solution.

	     The "app" executable takes in a parameter, a file containing numbers.

	NOTE: After "app" has finished executing, it will create a XXXX.log result log file,
	      where XXXX is the name of the file given.

Sample of expected output in the XXXX.log file:
----------------------------------------------

Table 3.1 MEAN NUMBER OF PROBES FOR SUCCESSFUL LOOKUP( TABLE SIZE = 13093 ) FOR
 VARIANTS OF COALESCED HASHING
  &		0.2			0.4			0.6			0.8			0.9			0.95		0.99
Method
------------------------------------------------------------------------------------------------------------------------
EISCH	1.06147		1.14245		1.22798		1.33531		1.39308		1.42965		1.45834		
LISCH	1.06186		1.14417		1.23065		1.34476		1.40479		1.4434		1.47585		
EICH	1.08477		1.15524		1.23498		1.32471		1.3879		1.42563		1.45664		
LICH	1.08477		1.15524		1.23498		1.32251		1.38408		1.42129		1.45163		
