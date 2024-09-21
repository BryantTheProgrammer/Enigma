# Enigma Machine - Cryptography Program
This project simulates the encryption and decryption process of an Enigma machine. 
The program takes input from text files, encrypts or decrypts the content using a 
custom mapping mechanism based on user-defined keys, and outputs the result to a file.

## Author
**Bryant Hayden**

Class: CS150-01

Date: 11/16/2021

## Features
- Encryption: Encrypts plaintext using a multi-layer mapping array, mimicking the behavior of the Enigma machine.
- Decryption: Decrypts ciphertext back to plaintext using the same mappings.
- Supports input from files (plaintext.txt, ciphertext.txt).
- Multiple mappings between letters for encryption and decryption.

## Usage
Upon running the program, the user will be prompted to:

1. Select the mode:

- <kbd>E</kbd> for encryption
- <kbd>D</kbd> for decryption
- <kbd>Q</kbd> to quit

2. Input keys: A string of uppercase letters (minimum 2, maximum 25) that will be used for the encryption/decryption mapping.
3. Specify the input file (either plaintext.txt for encryption or ciphertext.txt for decryption).
4. The encrypted or decrypted output will be saved in the corresponding output file.
