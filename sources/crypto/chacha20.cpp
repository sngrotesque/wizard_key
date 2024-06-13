#include "crypto/chacha20.hpp"

#define CHACHA20_CONSTANT       "expand 32-byte k"
#define CHACHA20_ROTL(x, n)     (((x) << (n)) | ((x) >> (32 - (n))))
#define CHACHA20_QR(a, b, c, d)               \
	a += b; d ^= a; d = CHACHA20_ROTL(d, 16); \
	c += d; b ^= c; b = CHACHA20_ROTL(b, 12); \
	a += b; d ^= a; d = CHACHA20_ROTL(d,  8); \
	c += d; b ^= c; b = CHACHA20_ROTL(b,  7)

static uint32_t pack4(const uint8_t* a)
{
	uint32_t res =
		  (uint32_t)a[0] << 0 * 8
		| (uint32_t)a[1] << 1 * 8
		| (uint32_t)a[2] << 2 * 8
		| (uint32_t)a[3] << 3 * 8;

	return res;
}

static void ChaCha20_block_next(const uint32_t in[16], uint32_t out[16], uint8_t** keystream)
{
	for(int i = 0; i < 16; i++)
		out[i] = in[i];

	// Round 1/10
	CHACHA20_QR(out[0], out[4], out[ 8], out[12]); // Column 0
	CHACHA20_QR(out[1], out[5], out[ 9], out[13]); // Column 1
	CHACHA20_QR(out[2], out[6], out[10], out[14]); // Column 2
	CHACHA20_QR(out[3], out[7], out[11], out[15]); // Column 3
	CHACHA20_QR(out[0], out[5], out[10], out[15]); // Diagonal 1 (main diagonal)
	CHACHA20_QR(out[1], out[6], out[11], out[12]); // Diagonal 2
	CHACHA20_QR(out[2], out[7], out[ 8], out[13]); // Diagonal 3
	CHACHA20_QR(out[3], out[4], out[ 9], out[14]); // Diagonal 4

	// Round 2/10
	CHACHA20_QR(out[0], out[4], out[ 8], out[12]);
	CHACHA20_QR(out[1], out[5], out[ 9], out[13]);
	CHACHA20_QR(out[2], out[6], out[10], out[14]);
	CHACHA20_QR(out[3], out[7], out[11], out[15]);
	CHACHA20_QR(out[0], out[5], out[10], out[15]);
	CHACHA20_QR(out[1], out[6], out[11], out[12]);
	CHACHA20_QR(out[2], out[7], out[ 8], out[13]);
	CHACHA20_QR(out[3], out[4], out[ 9], out[14]);

	// Round 3/10
	CHACHA20_QR(out[0], out[4], out[ 8], out[12]);
	CHACHA20_QR(out[1], out[5], out[ 9], out[13]);
	CHACHA20_QR(out[2], out[6], out[10], out[14]);
	CHACHA20_QR(out[3], out[7], out[11], out[15]);
	CHACHA20_QR(out[0], out[5], out[10], out[15]);
	CHACHA20_QR(out[1], out[6], out[11], out[12]);
	CHACHA20_QR(out[2], out[7], out[ 8], out[13]);
	CHACHA20_QR(out[3], out[4], out[ 9], out[14]);

	// Round 4/10
	CHACHA20_QR(out[0], out[4], out[ 8], out[12]);
	CHACHA20_QR(out[1], out[5], out[ 9], out[13]);
	CHACHA20_QR(out[2], out[6], out[10], out[14]);
	CHACHA20_QR(out[3], out[7], out[11], out[15]);
	CHACHA20_QR(out[0], out[5], out[10], out[15]);
	CHACHA20_QR(out[1], out[6], out[11], out[12]);
	CHACHA20_QR(out[2], out[7], out[ 8], out[13]);
	CHACHA20_QR(out[3], out[4], out[ 9], out[14]);

	// Round 5/10
	CHACHA20_QR(out[0], out[4], out[ 8], out[12]);
	CHACHA20_QR(out[1], out[5], out[ 9], out[13]);
	CHACHA20_QR(out[2], out[6], out[10], out[14]);
	CHACHA20_QR(out[3], out[7], out[11], out[15]);
	CHACHA20_QR(out[0], out[5], out[10], out[15]);
	CHACHA20_QR(out[1], out[6], out[11], out[12]);
	CHACHA20_QR(out[2], out[7], out[ 8], out[13]);
	CHACHA20_QR(out[3], out[4], out[ 9], out[14]);

	// Round 6/10
	CHACHA20_QR(out[0], out[4], out[ 8], out[12]);
	CHACHA20_QR(out[1], out[5], out[ 9], out[13]);
	CHACHA20_QR(out[2], out[6], out[10], out[14]);
	CHACHA20_QR(out[3], out[7], out[11], out[15]);
	CHACHA20_QR(out[0], out[5], out[10], out[15]);
	CHACHA20_QR(out[1], out[6], out[11], out[12]);
	CHACHA20_QR(out[2], out[7], out[ 8], out[13]);
	CHACHA20_QR(out[3], out[4], out[ 9], out[14]);

	// Round 7/10
	CHACHA20_QR(out[0], out[4], out[ 8], out[12]);
	CHACHA20_QR(out[1], out[5], out[ 9], out[13]);
	CHACHA20_QR(out[2], out[6], out[10], out[14]);
	CHACHA20_QR(out[3], out[7], out[11], out[15]);
	CHACHA20_QR(out[0], out[5], out[10], out[15]);
	CHACHA20_QR(out[1], out[6], out[11], out[12]);
	CHACHA20_QR(out[2], out[7], out[ 8], out[13]);
	CHACHA20_QR(out[3], out[4], out[ 9], out[14]);

	// Round 8/10
	CHACHA20_QR(out[0], out[4], out[ 8], out[12]);
	CHACHA20_QR(out[1], out[5], out[ 9], out[13]);
	CHACHA20_QR(out[2], out[6], out[10], out[14]);
	CHACHA20_QR(out[3], out[7], out[11], out[15]);
	CHACHA20_QR(out[0], out[5], out[10], out[15]);
	CHACHA20_QR(out[1], out[6], out[11], out[12]);
	CHACHA20_QR(out[2], out[7], out[ 8], out[13]);
	CHACHA20_QR(out[3], out[4], out[ 9], out[14]);

	// Round 9/10
	CHACHA20_QR(out[0], out[4], out[ 8], out[12]);
	CHACHA20_QR(out[1], out[5], out[ 9], out[13]);
	CHACHA20_QR(out[2], out[6], out[10], out[14]);
	CHACHA20_QR(out[3], out[7], out[11], out[15]);
	CHACHA20_QR(out[0], out[5], out[10], out[15]);
	CHACHA20_QR(out[1], out[6], out[11], out[12]);
	CHACHA20_QR(out[2], out[7], out[ 8], out[13]);
	CHACHA20_QR(out[3], out[4], out[ 9], out[14]);

	// Round 10/10
	CHACHA20_QR(out[0], out[4], out[ 8], out[12]);
	CHACHA20_QR(out[1], out[5], out[ 9], out[13]);
	CHACHA20_QR(out[2], out[6], out[10], out[14]);
	CHACHA20_QR(out[3], out[7], out[11], out[15]);
	CHACHA20_QR(out[0], out[5], out[10], out[15]);
	CHACHA20_QR(out[1], out[6], out[11], out[12]);
	CHACHA20_QR(out[2], out[7], out[ 8], out[13]);
	CHACHA20_QR(out[3], out[4], out[ 9], out[14]);

	for(int i = 0; i < 16; i++)
		out[i] += in[i];

	if(keystream != NULL)
		*keystream = (uint8_t*)out;
}

wmkc::crypto::ChaCha20::ChaCha20(wByte *key, wByte *nonce, wU32 counter)
: counter(counter)
{
    memcpy(this->key, key, sizeof(this->key));
    memcpy(this->nonce, nonce, sizeof(this->nonce));
}

wVoid wmkc::crypto::ChaCha20::init()
{
    this->state[ 0] = pack4((const uint8_t*)CHACHA20_CONSTANT + 0 * 4);
    this->state[ 1] = pack4((const uint8_t*)CHACHA20_CONSTANT + 1 * 4);
    this->state[ 2] = pack4((const uint8_t*)CHACHA20_CONSTANT + 2 * 4);
    this->state[ 3] = pack4((const uint8_t*)CHACHA20_CONSTANT + 3 * 4);
    this->state[ 4] = pack4(this->key + 0 * 4);
    this->state[ 5] = pack4(this->key + 1 * 4);
    this->state[ 6] = pack4(this->key + 2 * 4);
    this->state[ 7] = pack4(this->key + 3 * 4);
    this->state[ 8] = pack4(this->key + 4 * 4);
    this->state[ 9] = pack4(this->key + 5 * 4);
    this->state[10] = pack4(this->key + 6 * 4);
    this->state[11] = pack4(this->key + 7 * 4);
    this->state[12] = this->counter;
    this->state[13] = pack4(this->nonce + 0 * 4);
    this->state[14] = pack4(this->nonce + 1 * 4);
    this->state[15] = pack4(this->nonce + 2 * 4);
}

wVoid wmkc::crypto::ChaCha20::xcrypt(wByte *buffer, wSize length)
{
	wByte *keystream = nullptr;
	wU32 tmp[16];

	for(wSize i = 0; i < length; i += 64)
	{
		ChaCha20_block_next(this->state, tmp, &keystream);
		this->state[12]++;

		if(this->state[12] == 0)
		{
			this->state[13]++;
			assert(this->state[13] != 0);
		}

		for(wSize j = i; j < i + 64; j++)
		{
			if(j >= length)
				break;

			buffer[j] = buffer[j] ^ keystream[j - i];
		}
	}
}
