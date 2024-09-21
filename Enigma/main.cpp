//**********************************************************************
// File:					main.cpp
// Author:				Bryant Hayden
// Date:					11/16/2021
// Class:					CS150-01
// Assignment:		Cryptography
// Purpose:       Write program to mimic enigma machine
// Hours Worked:  a lot, 10+, lots of reworking to make program better
//**********************************************************************

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const int LETTERS_IN_ALPHABET = 26;
const string FILE_NAME_PLAINTEXT = "plaintext.txt";
const string FILE_NAME_CIPHERTEXT = "ciphertext.txt";

const char ENCRYPT = 'E';
const char DECRYPT = 'D';
const char QUIT = 'Q';
const char MIN_CHAR = 'A';
const char MAX_CHAR = 'Z';

const int MAX_KEYS = 25;
const int MIN_KEYS = 2;

void displayHeading (const char heading[]);
char getMode ();
void getMappingData (int &numberOfMappings, char keys[]);
void openFileForRead (ifstream &inputFile, const char message[]);
void openFileForWrite (ofstream &outputFile, const char message[]);
void closeFileForRead (ifstream &inputFile);
void closeFileForWrite (ofstream &outputFile);
void constructMappingArray (char mappingArray[][LETTERS_IN_ALPHABET],
	const char keys[], int numberOfMappings);
void printMappingArray (const char mappingArray[][LETTERS_IN_ALPHABET],
	int numberOfMappings);
char decodeCharacter (char ciphertextCharacter,
	const char mappingArray[][LETTERS_IN_ALPHABET],
	int numberOfMappings);
char encodeCharacter (char plaintextCharacter,
	const char mappingArray[][LETTERS_IN_ALPHABET],
	int numberOfMappings);
void decryptMessage (int numberOfMappings,
	const char mappingArray[][LETTERS_IN_ALPHABET],
	ifstream &inFile, ofstream &ofFile);
void encryptMessage (int numberOfMappings,
	const char mappingArray[][LETTERS_IN_ALPHABET],
	ifstream &inFile, ofstream &ofFile);

/***********************************************************************
Function:    main

Description: Engine for project

Parameters:  none

Returned:    EXIT_STATUS
***********************************************************************/
int main () {

	const char HEADER[] = "*** CRYPTOGRAPHY - Enigma Machine ***";
	const char PLAINTEXTMESSAGE[] = "Enter plaintext file name: ";
	const char CIPHERTEXTMESSAGE[] = "Enter ciphertext file name: ";

	ifstream inFile;
	ofstream ofFile;
	char mappingArray[MAX_KEYS][LETTERS_IN_ALPHABET];
	char keys[MAX_KEYS];
	char functionMode;
	char workingChar;
	int numberOfMappings;

	displayHeading (HEADER);
	cout << endl << endl;

	functionMode = getMode ();

	while (QUIT != functionMode) {

		getMappingData (numberOfMappings, keys);

		constructMappingArray (mappingArray, keys, numberOfMappings);

		//UNCOMMENT to get visual aid of mappingArray
		cout << endl;
		printMappingArray (mappingArray, numberOfMappings);
		cout << endl;

		if (ENCRYPT == functionMode) {
			openFileForRead (inFile, PLAINTEXTMESSAGE);
			openFileForWrite (ofFile, CIPHERTEXTMESSAGE);
			encryptMessage (numberOfMappings, mappingArray, inFile, ofFile);

			cout << endl << "Your message has been encrypted!";
		}

		else if (DECRYPT == functionMode) {
			openFileForRead (inFile, CIPHERTEXTMESSAGE);
			openFileForWrite (ofFile, PLAINTEXTMESSAGE);
			decryptMessage (numberOfMappings, mappingArray, inFile, ofFile);
			cout << endl << "Your message has been decrypted!";
		}

		cout << endl << endl;

		closeFileForRead (inFile);
		closeFileForWrite (ofFile);

		functionMode = getMode ();
	}
	return EXIT_SUCCESS;
}


/***********************************************************************
Function:     displayHeading

Description:  Prints the heading as displayed on the sample output of
							the previous page.

Parameters:   const char heading[]

Returned:     none
***********************************************************************/
void displayHeading (const char heading[]) {
	cout << heading;
}

/***********************************************************************
Function:     getMappingData

Description:  Prompts the user for the number of mappings and the keys
							as displayed on the sample output. The number of mappings
							and keys are returned through the parameters
							numberOfMappings and keys.

Parameters:   int& numberOfMappings, char keys[]

Returned:     none
***********************************************************************/
void getMappingData (int &numberOfMappings, char keys[]) {
	bool bKeyCheck = true;
	bool bAllKeysCheck;
	do {
		bAllKeysCheck = true;//Reset at top, has to stay true all the way.

		cout << "Enter your keys: ";
		cin >> keys;

		//This FOR checks that we have the right amount and all upper case
		for (int key = 0; key < strlen (keys); key++) {
			bKeyCheck = isupper (keys[key]);
			if (false == bKeyCheck) {
				bAllKeysCheck = false;
				cout << endl << "Must be Upper Case!" << endl;
				break;
			}
		}
	} while ((static_cast<int>(strlen (keys)) < MIN_KEYS ||
		static_cast<int>(strlen (keys)) > MAX_KEYS) || false == bAllKeysCheck);
	numberOfMappings = static_cast<int>(strlen (keys));
}

/***********************************************************************
Function:    getMode

Description: Prompt user for mode

Parameters:  none

Returned:    mode
***********************************************************************/
char getMode () {
	char mode;

	do {
		cout << "Enter E) for encryption, D) for decryption,"
			" or Q) for Quit: ";
		cin >> mode;
		if (islower (mode)) {
			mode = toupper (mode);
		}
	} while (ENCRYPT != mode && DECRYPT != mode && QUIT != mode);
	// 'e' != mode && 'd' != mode && 'q' != mode);
	return  mode;
}

/***********************************************************************
Function:    openFileForRead

Description: Accepts a message to be displayed
						 (e.g. Enter plaintext filename:), then prompts the user to
						 enter a plaintext file validating that the file exists and
						 is open. Do not proceed until a proper file can is opened.

Parameters: (ifstream& inputFile, const char message[])

Returned:   none
***********************************************************************/
void openFileForRead (ifstream &inputFile, const char message[]) {

	string fileName;

	do {
		cout << message;
		cin >> fileName;
	} while (FILE_NAME_PLAINTEXT != fileName &&
		FILE_NAME_CIPHERTEXT != fileName);

	inputFile.open (fileName);
	if (inputFile.fail ()) {
		cout << "ERROR OPENING INPUT FILE";
		exit (EXIT_FAILURE);
	}
}

/***********************************************************************
Function:    closeFileForRead

Description: Closes a file open for reading.
						 Make sure file is open before closing.

Parameters:  ifstream& inputFile

Returned:    none
***********************************************************************/
void closeFileForRead (ifstream &inputFile) {
	if (inputFile.is_open ()) {
		inputFile.close ();
	}
}
/***********************************************************************
Function:    openFileForWrite

Description: Similar to openFileForRead only we are writing to a file.

Parameters:  ofstream & outputFile, const char message[]

Returned:    none
***********************************************************************/
void openFileForWrite (ofstream &outputFile, const char message[]) {

	string fileName;

	do {
		cout << message;
		cin >> fileName;
	} while (FILE_NAME_PLAINTEXT != fileName &&
		FILE_NAME_CIPHERTEXT != fileName);

	outputFile.open (fileName);

	if (outputFile.fail ()) {
		cout << "ERROR OPENING OUTPUT FILE";
		exit (EXIT_FAILURE);
	}
}

/***********************************************************************
Function:    closeFileForWrite

Description: Closes a file open for writing.
						 Make sure file is open before closing.

Parameters:  ofstream& outputFile

Returned:    none
***********************************************************************/
void closeFileForWrite (ofstream &outputFile) {
	if (outputFile.is_open ()) {
		outputFile.close ();
	}
}

/***********************************************************************
Function:    constructMappingArray

Description: Using the keysand numberOfMappings, you are to create the
						 mappingArray as discussed in the assignment.

Parameters:  char mappingArray[][LETTERS_IN_ALPHABET],
						 const char keys[], int numberOfMappings

Returned:    none
***********************************************************************/
void constructMappingArray (char mappingArray[][LETTERS_IN_ALPHABET],
	const char keys[], int numberOfMappings) {
	int keyIndex = 0;
	char letter;
	for (int row = 0; row < numberOfMappings; row++) {
		letter = keys[keyIndex];
		for (int col = 0; col < LETTERS_IN_ALPHABET; col++) {
			mappingArray[row][col] = ((letter - MIN_CHAR) % 26) + MIN_CHAR;
			letter++;
		}
		keyIndex++;
	}
}

/***********************************************************************
Function:    printMappingArray

Description: This function is simply for debugging. After you create the
						 mappingArray, print it out to make sure you have a correct
						 mappingArray. If not, there is no point in going on until
						 this array is correct.

Parameters:  const char mappingArray[][LETTERS_IN_ALPHABET],
						 int numberOfMappings

Returned:    none
***********************************************************************/
void printMappingArray (const char mappingArray[][LETTERS_IN_ALPHABET],
	int numberOfMappings) {
	for (int row = 0; row < numberOfMappings; row++) {
		for (int col = 0; col < LETTERS_IN_ALPHABET; col++) {
			cout << mappingArray[row][col];
		}
		cout << endl;
	}
}

/***********************************************************************
Function:    decodeCharacter

Description: Description: Using the number of mappings and the
						 mappingArray, this function takes an encoded character and
						 returns a decoded character using the process described
						 above.

Parameters:  char ciphertextCharacter,
						 const char mappingArray[][LETTERS_IN_ALPHABET],
						 int numberOfMappings

Returned:    decoded character
***********************************************************************/
char decodeCharacter (char ciphertextCharacter,
	const char mappingArray[][LETTERS_IN_ALPHABET],
	int numberOfMappings) {
	char decodedCharacter = ciphertextCharacter;
	int position = 0;

	if (decodedCharacter >= MIN_CHAR && decodedCharacter <= MAX_CHAR) {
		for (int mapping = numberOfMappings - 1; mapping >= 0; mapping--) {
			position = ((decodedCharacter - MIN_CHAR) +
				(26 - (mappingArray[mapping][0] - MIN_CHAR))) % 26;
			decodedCharacter = MIN_CHAR + position;
		}
	}
	return decodedCharacter;
}

/***********************************************************************
Function:    encodeCharacter

Description: Description: Using the number of mappings and the
						 mappingArray, this function takes a plaintext character and
						 returns a ciphertext character using the process described
						 above.

Parameters:  char plaintextCharacter,
						 const char mappingArray[][LETTERS_IN_ALPHABET],
						 int numberOfMappings

Returned:    encodedCharacter
***********************************************************************/
char encodeCharacter (char plaintextCharacter,
	const char mappingArray[][LETTERS_IN_ALPHABET],
	int numberOfMappings) {
	char encodedCharacter = plaintextCharacter;

	if (encodedCharacter >= MIN_CHAR && encodedCharacter <= MAX_CHAR) {
		for (int mapping = 0; mapping < numberOfMappings; mapping++) {
			encodedCharacter =
				mappingArray[mapping][encodedCharacter - MIN_CHAR];
		}
	}
	return encodedCharacter;
}

/***********************************************************************
Function:    decryptMessage

Description: decodes entire message using decodeCharacter function

Parameters:  numberOfmappings, mappingArray, inFile, ofFile

Returned:    none
***********************************************************************/
void decryptMessage (int numberOfMappings,
	const char mappingArray[][LETTERS_IN_ALPHABET],
	ifstream &inFile, ofstream &ofFile) {
	char workingChar;

	inFile.get (workingChar);

	while (workingChar != EOF) {
		ofFile << decodeCharacter (workingChar, mappingArray,
			numberOfMappings);
		workingChar = inFile.get ();
	}
}

/***********************************************************************
Function:    encryptMessage

Description: encodes entire message using encodeCharacter function

Parameters:  numberOfmappings, mappingArray, inFile, ofFile

Returned:    none
***********************************************************************/
void encryptMessage (int numberOfMappings,
	const char mappingArray[][LETTERS_IN_ALPHABET],
	ifstream &inFile, ofstream &ofFile) {
	char workingChar;

	inFile.get (workingChar);

	while (workingChar != EOF) {
		ofFile << encodeCharacter (workingChar, mappingArray,
			numberOfMappings);
		workingChar = inFile.get ();
	}
}