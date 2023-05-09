#ifndef AES_AES_H
#define AES_AES_H

#include <stdint.h>


typedef struct {
    // Encryption key and decryption key
    uint32_t e_k[44], d_k[44];
    // Round
    int Nr;
} AESKey;


// Size of a group
#define BLOCKSIZE 16

// uint8_t y[4] -> uint32_t x
#define LOAD32H(x,y) \
do {(x) = ((uint32_t)((y)[0] & 0xff)<<24) | ((uint32_t)((y)[1] & 0xff)<<16) | \
             ((uint32_t)((y)[2] & 0xff)<<8)  | ((uint32_t)((y)[3] & 0xff));} while(0)

// uint32_t x -> uint8_t y[4]
#define STORE32H(x, y) \
  do {(y)[0] = (uint8_t)(((x)>>24) & 0xff); (y)[1] = (uint8_t)(((x)>>16) & 0xff); \
       (y)[2] = (uint8_t)(((x)>>8) & 0xff); (y)[3] = (uint8_t)((x) & 0xff);} while(0)

// Read the No.n byte from uint32_t x
#define BYTE(x, n) (((x) >> (8 * (n))) & 0xff)

// Used for expansion
// Sub bytes and rotate(1 bit to the left)
#define MIX(x) (((SBox[BYTE(x, 2)] << 24) & 0xff000000) ^ ((SBox[BYTE(x, 1)] << 16) & 0xff0000) ^ \
                ((SBox[BYTE(x, 0)] << 8) & 0xff00) ^ (SBox[BYTE(x, 3)] & 0xff))
// uint32_t x rotate to the left(n bits)
#define ROF32(x, n)  (((x) << (n)) | ((x) >> (32-(n))))
// uint32_t x rotate to the right(n bits)
#define ROR32(x, n)  (((x) >> (n)) | ((x) << (32-(n))))


int loadStateArray(uint8_t(*state)[4], const uint8_t* in);  // in[16] to state[4][4]
int storeStateArray(uint8_t(*state)[4], uint8_t* out);  // state[4][4] t out[16]
int keyExpansion(const uint8_t* key, uint32_t keyLen, AESKey* aesKey);
int addRoundKey(uint8_t(*state)[4], const uint32_t* key);
int subBytes(uint8_t(*state)[4]);
int invSubBytes(uint8_t(*state)[4]);
int shiftRows(uint8_t(*state)[4]);
int invShiftRows(uint8_t(*state)[4]);
uint8_t GMul(uint8_t u, uint8_t v);  // Galois Field (256) Multiplication of two Bytes
int mixColumns(uint8_t(*state)[4]);
int invMixColumns(uint8_t(*state)[4]);
int AESEncrypt(const uint8_t* key, uint32_t keyLen, const uint8_t* pt, uint8_t* ct);
int AESDecrypt(const uint8_t* key, uint32_t keyLen, const uint8_t* ct, uint8_t* pt);

#endif