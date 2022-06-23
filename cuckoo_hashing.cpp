#include <iostream>
#include <iomanip>
#include <cmath>

	// Zählt wie oft durch das Hashen eines Elements, ein anderes verschoben wird.
	// Wenn der Zähler MODULOR erreicht, ist es ein endlosloop und man muss Rehashen
int cycle_counter = 0;

int MODULOR = 2;					// MODULOR fängt bei 2 an und wird automatisch erhöht, 
									// wenn es eine Loop gibt. So wird das Ergebnis effizient
									// man kann hier aber auch direkt MODULOR = 11 angeben und spart sich
									// einige Rehashes, aber dann hat man wahrscheinlich nicht 
									// die kleinsten Hashtablegrößen					

int amount_of_rehashes_needed = 0;	// Zählt wie oft aufgrund von Loops rehashed werden musste

  	// Wenn eine Loop entdeckt wurde, wird rehashed. Modulor wird um 1 erhöht, damit man andere
	// Hashingalgorithmen hat und es wird sich gemerkt, dass rehashed wurde 
	// (um die Anzahl der Rehashes später ausgeben zu können)
bool rehash () {

	if (cycle_counter >= MODULOR*2) {

		MODULOR++;
		amount_of_rehashes_needed++;
		return true;
	}

	else {

		return false;
	}
}


void printblank (int blanks) {

	for (int i = 0; i < blanks; i++) {

		std::cout << " ";
	}
}

	// Nur zur schönen Ausgabe der Tabellen da
int digit_count (int number) {

	if (number == 0) {

		return 1;
	}
	else {

		return int(log10(number) + 1);
	}	
}



int hash1 (int key) {
	
	return key % MODULOR;
}


int hash2 (int key) {

	return (key / MODULOR) % MODULOR;
}

void initialize_hashtables (int* hashtable1, int* hashtable2) {


		for (int i = 0; i < MODULOR; i++) {

			hashtable1 [i] = 0;
		}

		for (int i = 0; i < MODULOR; i++) {

			hashtable2 [i] = 0;
		}
}


void trytoplace (int, int*, int*);

void cuckoofrom1to2 (int, int*, int*);

void cuckoofrom2to1 (int, int*, int*);

void printhashes (int*, int);

void printhashtables (int*, int*);



int main (void) {
	bool rehash_needed_bool = true;
	
	while (rehash_needed_bool == true) { // START REHASH TRUE

		rehash_needed_bool = false;

		/* Loop-freies Array Mod 11 */
//		int array [] = {20, 50, 53, 75, 100, 67, 105, 3, 36, 39};

		/* Loop-haltiges Array Mod 11, wegen der 6 */
		int array [] = {20, 50, 53, 75, 100, 67, 105, 3, 36, 39, 6};

		int array_size = sizeof(array) / sizeof (int);



		printhashes (array, array_size);

		std::cout << std::endl << "Array Size: " << array_size << std::endl;

		
		int hashtable1 [MODULOR];

		int hashtable2 [MODULOR];


		// Hashtables werden auf 0 gesetzt. 0 bedeutet hier ein freier Platz. 0 Kann dementsprechend
		// kein Key sein. (Suboptimal)

		initialize_hashtables (hashtable1, hashtable2);


		// Versucht die Keys in Hashtable1 zu packen, wenn nicht dann wird cuckoo'd (rekursiv)
		for (int i = 0; i < array_size; i++) {

			trytoplace (array [i], hashtable1, hashtable2);
		}

		// Es wird geguckt ob es eine Loop gab oder nicht. Wenn es eine gab, 
		// wird rehashed werden müssen

		rehash_needed_bool = rehash ();
		
		if (rehash_needed_bool == false) {

			printhashtables (hashtable1, hashtable2);


			int userinput;

			std::cout << std::endl << "Zahl eingeben: ";
			std::cin >> userinput;


    		std::cout << std::endl << "Es soll überprüft werden ob die Zahl " << userinput 
    		<< " in dem Array enthalten ist" << std::endl << std::endl;


    		if (userinput == hashtable1 [ hash1 (userinput) ] ) {

    			std::cout << "Der Wert " << userinput << " ist in Hashtable 1 am Index " << hash1 (userinput) << std::endl;
    		}

    		else if (userinput == hashtable2 [ hash2 (userinput) ] ) {

    			std::cout << "Der Wert " << userinput << " ist in Hashtable 2 am Index " << hash2 (userinput) << std::endl;
    		}

    			else {

    				std::cout << "Der Wert " << userinput << " ist nicht im Array enthalten" << std::endl;
    		}

		
		std::cout << std::endl << "Aufgrund von Loops mussten die Keys " << amount_of_rehashes_needed <<
		" mal rehashed werden" << std::endl;

		}	


		else {
		
			std::cout << std::endl << std::endl << std::endl 
    		<< "Aufgrund der Loop(s) wird Rehashed..." << std::endl << std::endl << std::endl;

    		std::cout << "Der neue MODULOR ist: " << MODULOR << std::endl;
		}


	

    } // END REHASH TRUE LOOP

	return 0;
} ////////////////////////// MAIN END //////////////////////////








void trytoplace (int key, int* hashtable1, int* hashtable2) {

	int hash_value_1 = hash1 (key);


	if (hashtable1 [hash_value_1] == 0) {

		hashtable1 [hash_value_1] = key;
	}

	else {

		int opfer = hashtable1 [hash_value_1];
		hashtable1 [hash_value_1] = key;

		cuckoofrom1to2 (opfer, hashtable1, hashtable2);
	}

}





void cuckoofrom1to2 (int key, int* hashtable1, int* hashtable2) {


	int hash_value_2 = hash2 (key);

	if (cycle_counter >= (MODULOR*2) ) {

		std::cout << std::endl << "Loop Detected. Bitte Rehashen" << std::endl;
		return;
	}

	if (hashtable2 [hash_value_2] == 0) {

		hashtable2 [hash_value_2] = key;
		cycle_counter = 0;	// cycle counter reset
	}

	else {

		int opfer = hashtable2 [hash_value_2];
		hashtable2 [hash_value_2] = key;
		cycle_counter++;	// cycle counter incrementieren

		cuckoofrom2to1 (opfer, hashtable1, hashtable2);
	}
}

void cuckoofrom2to1 (int key, int* hashtable1, int* hashtable2) {


	int hash_value_1 = hash1 (key);

	if (cycle_counter >= (MODULOR*2) ) {

		std::cout << std::endl << "Loop Detected. Bitte Rehashen" << std::endl;
		return;
	}

	if (hashtable2 [hash_value_1] == 0) {

		hashtable2 [hash_value_1] = key;
		cycle_counter = 0;	// cycle counter reset
	}

	else {

		int opfer = hashtable1 [hash_value_1];
		hashtable1 [hash_value_1] = key;
		cycle_counter++;	// cycle counter incrementieren

		cuckoofrom1to2 (opfer, hashtable1, hashtable2);
	}
}



void printhashes (int* array, int array_size) {

	std::cout << "Keys mit dazugehörigen Hashes Hash 1 und Hash 2" << std::endl;

	for (int i = 0; i < array_size; i++) {


    		std::cout <<  "Key "; 
    		std::cout << array[i] << ":";
    		printblank (10-digit_count (array [i]) );


    		std::cout << hash1 (array [i]);

    		printblank (10-digit_count (hash1 (array [i])) );
    		std::cout << hash2 (array [i]) << std::endl;
    	
	}

}

void printhashtables (int* hashtable1, int* hashtable2) {

	std::cout << std::endl << "Hashtables Hashtable 1 und Hashtable 2" << std::endl;

    for (int i = 0; i < MODULOR; i++) {

    		std::cout <<  "Index "; 
    		std::cout << i << ":";
    		printblank (10-digit_count (i) );


    		std::cout << hashtable1 [i];
    		printblank ( 10-digit_count (hashtable1 [i]) );

    		std::cout << hashtable2 [i] << std::endl;

  	}

}  		
