#include <iostream>
#include <iomanip>
#include <cmath>
#include <string.h>

#define MAXLEN 64

	// Zählt wie oft durch das Hashen eines Elements, ein anderes verschoben wird.
	// Wenn der Zähler MODULOR erreicht, ist es ein endlosloop und man muss Rehashen
int cycle_counter = 0;

int MODULOR = 8;					// MODULOR fängt bei 8 an (2*8 >= 16)
									// und wird automatisch erhöht, 
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



int hash1 (char* key) {
	
	int end = strlen (key); 

	int gesamt = 0;

	for (int i = 0; i < end; i++) {
		
		gesamt+= key [i];
	}


	return gesamt % MODULOR;
}


int hash2 (char* key) {

	int end = strlen (key); 

	int gesamt = 0;

	for (int i = 0; i < end; i++) {
		
			gesamt+= key [i];	
	}


	return (gesamt / MODULOR) % MODULOR;
}

void initialize_hashtables (char hashtable1 [] [MAXLEN], char hashtable2 [] [MAXLEN]) {


		for (int i = 0; i < MODULOR; i++) {

			strcpy (hashtable1 [i], "NULL");
		}

		for (int i = 0; i < MODULOR; i++) {

			strcpy (hashtable2 [i], "NULL");
		}
}

double belegungsrechner (char hashtable1 [] [MAXLEN], char hashtable2 [] [MAXLEN]) {

	int anz_belegt = 0;


	for (int i = 0; i < MODULOR; i++) {
		
		if ( strcmp (hashtable1 [i], "NULL") != 0 ) {
			anz_belegt++;
		}

		if ( strcmp (hashtable2 [i], "NULL") != 0 ) {
			anz_belegt++;
		}

	}

//	return (double) anz_belegt / (double) (MODULOR*2);
	return (double) anz_belegt;
}


void trytoplace (char*, char [] [MAXLEN], char [] [MAXLEN]);

void cuckoofrom1to2 (char*, char [] [MAXLEN], char [] [MAXLEN]);

void cuckoofrom2to1 (char*, char [] [MAXLEN], char [] [MAXLEN]);

void printhashes (char [] [MAXLEN], int);

void printhashtables (char [] [MAXLEN], char [] [MAXLEN]);



int main (void) {
	bool rehash_needed_bool = true;
	
	while (rehash_needed_bool == true) { // START REHASH TRUE

		rehash_needed_bool = false;

		
		/* Loop-haltiges Array Mod 11, wegen der 6 */
		char array [] [MAXLEN] = { "Baden-Wuerttemberg", "Bayern", "Berlin", "Brandenburg", "Bremen", "Hamburg", "Hessen", "Mecklenburg-Vorpommern", "Niedersachsen", "Nordrhein-Westfalen", "Rheinland-Pfalz", "Saarland", "Sachsen", "Sachsen-Anhalt", "Schleswig-Holstein", "Thueringen"};

//		int array_size = sizeof(array) / sizeof (char);

		int array_size = 16;

		printhashes (array, array_size);

		std::cout << std::endl << "Array Size: " << array_size << std::endl;

		
		char hashtable1 [MODULOR] [MAXLEN];

		char hashtable2 [MODULOR] [MAXLEN];


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


			char userinput [MAXLEN];

			std::cout << std::endl << "String eingeben: ";
			std::cin >> userinput;

			while (strcmp (userinput, "NULL") == 0) {

				std::cout << std::endl << "Bitte nicht NULL eingeben: ";
				std::cin >> userinput;
			}


    		std::cout << std::endl << "Es soll überprüft werden ob der String " << userinput 
    		<< " in dem Array enthalten ist" << std::endl << std::endl;


    		if (strcmp (userinput, hashtable1 [hash1 (userinput)]) == 0 ) {

    			std::cout << "Der String " << userinput << " ist in Hashtable 1 am Index " << hash1 (userinput) << std::endl;
    		}

    		else if ( strcmp (userinput, hashtable2 [hash2 (userinput)]) == 0 ) {

    			std::cout << "Der String " << userinput << " ist in Hashtable 2 am Index " << hash2 (userinput) << std::endl;
    		}

    			else {

    				std::cout << "Der String " << userinput << " ist nicht im Array enthalten" << std::endl;
    		}

		
		std::cout << std::endl << "Aufgrund von Loops mussten die Hashfunktionen " << amount_of_rehashes_needed <<
		" mal rehashed werden" << std::endl << std::endl;

		std::cout << "Belegungsfaktor am Ende ist ~" 
		<< (int) ( (belegungsrechner (hashtable1, hashtable2)/(MODULOR*2) ) * 100) << "%" 
		<< " bzw. " << belegungsrechner (hashtable1, hashtable2)
		<< "/" << MODULOR*2 << " als Bruch" << std::endl;

		}	


		else {
		
			std::cout << std::endl << std::endl << std::endl 
    		<< "Aufgrund der Loop(s) wird Rehashed..." << std::endl << std::endl << std::endl;

    		std::cout << "Der neue MODULOR ist: " << MODULOR << std::endl;
		}


	

    } // END REHASH TRUE LOOP

	return 0;
} ////////////////////////// MAIN END //////////////////////////








void trytoplace (char* key, char hashtable1 [] [MAXLEN], char hashtable2 [] [MAXLEN]) {

	int hash_value_1 = hash1 (key);


	if (strcmp (hashtable1 [hash_value_1], "NULL") == 0) {

//		hashtable1 [hash_value_1] = key;

		strcpy (hashtable1 [hash_value_1], key);
	}

	else {

		char opfer [MAXLEN];

		strcpy (opfer, hashtable1 [hash_value_1]);

		strcpy (hashtable1 [hash_value_1], key);
//		hashtable1 [hash_value_1] = key;

		cuckoofrom1to2 (opfer, hashtable1, hashtable2);
	}

}





void cuckoofrom1to2 (char* key, char hashtable1 [] [MAXLEN], char hashtable2 [] [MAXLEN]) {


	int hash_value_2 = hash2 (key);

	if (cycle_counter >= (MODULOR*2) ) {

		std::cout << std::endl << "Loop Detected. Bitte Rehashen" << std::endl;
		return;
	}

	if (strcmp (hashtable2 [hash_value_2], "NULL") == 0) {

		strcpy (hashtable2 [hash_value_2], key);
		cycle_counter = 0;	// cycle counter reset
	}

	else {

		char opfer [MAXLEN];
		strcpy (opfer, hashtable2 [hash_value_2]);
		strcpy (hashtable2 [hash_value_2], key);

		
		cycle_counter++;	// cycle counter incrementieren
		cuckoofrom2to1 (opfer, hashtable1, hashtable2);
	}
}

void cuckoofrom2to1 (char* key, char hashtable1 [] [MAXLEN], char hashtable2 [] [MAXLEN]) {
	

	int hash_value_1 = hash1 (key);

	if (cycle_counter >= (MODULOR*2) ) {

		std::cout << std::endl << "Loop Detected. Bitte Rehashen" << std::endl;
		return;
	}

	if (strcmp (hashtable1 [hash_value_1], "NULL") == 0) {

		strcpy (hashtable1 [hash_value_1], key);
		cycle_counter = 0;	// cycle counter reset
	}

	else {

		char opfer [MAXLEN];

		strcpy (opfer, hashtable1 [hash_value_1]);
		strcpy (hashtable1 [hash_value_1], key);

		cycle_counter++;	// cycle counter incrementieren
		cuckoofrom1to2 (opfer, hashtable1, hashtable2);
	}
}



void printhashes (char array [] [MAXLEN], int array_size) {

	std::cout << "Keys mit dazugehörigen Hashes Hash 1 und Hash 2" << std::endl;

	for (int i = 0; i < array_size; i++) {


    		std::cout <<  "Key "; 
    		std::cout << array[i] << ":";
 //   		printblank (10-digit_count (array [i]) );
    		printblank (25 - strlen (array [i]));
 //   		printblank (10);


    		std::cout << hash1 (array [i]);

 //   		printblank (10-digit_count (hash1 (array [i])) );
    		printblank (10- digit_count (hash1 (array [i])));
    		std::cout << hash2 (array [i]) << std::endl;
    	
	}

}

void printhashtables (char hashtable1 [] [MAXLEN], char hashtable2 [] [MAXLEN]) {

	std::cout << std::endl << "Hashtables Hashtable 1 und Hashtable 2" << std::endl;

    for (int i = 0; i < MODULOR; i++) {

    		std::cout <<  "Index "; 
    		std::cout << i << ":";
    		printblank (10-digit_count (i) );
 //   		printblank (20 - strlen ());


    		std::cout << hashtable1 [i];
//    		printblank ( 10-digit_count (hashtable1 [i]) );
    		printblank (25 - strlen (hashtable1 [i]));
 //   		printblank (10);

    		std::cout << hashtable2 [i] << std::endl;

  	}

}  		
