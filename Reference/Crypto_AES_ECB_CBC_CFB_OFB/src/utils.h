#ifndef AES_UTILS_H
#define AES_UTILS_H

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// 0x7f stands for 127, ascii starts from 0x7f and ends in 0xff
#define to_ascii(c) (((unsigned char)(c))&0x7f)


// Displacement
void move(bool raw[64], const bool in[]);
// Get command
int commandTokens();
// Read file before encryption, in binary format
void readToMemory();
// Hex to bin
void hexToBin(char* string, bool* binString, long int num);
// XOR
void XOR(bool* DatA, const bool* DatB);
// XOR(8 bits)
void XOR_8(bool* DatA, const bool* DatB);
// Hex to dec(hexadecimal string to decimal number)
int hexToDec(char ch);
// String to bytes
// Len stands for the length of plain text
void strToByte(char* plain_text, uint8_t byte_plain_text[], int len);
// Bytes to string
// Len stands for the length of plain text
void byteToStr(uint8_t byte_plain_text[], char* plainText, int len);


// Global
char argc[6][30];
char plainFile[50];
char keyFile[30];
char ivFile[30];
char operationMode[4];
char cipherFile[30];
char* plainText;
char* keyText;
char* ivText;
char* cipherText;
uint8_t* bytePlainText;
uint8_t* byteKeyText;
uint8_t* byteIvText;
uint8_t* byteCipherText;
char command[256];
int tokenNumber;
long int plainSize;

#endif
