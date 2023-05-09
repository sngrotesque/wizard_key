// Copyright (C) 2022 Valentin-Ioan VINTILA.
// All rights reserved.

#include "../include/wi-aes.hpp"

namespace wi {
	// ------------------------------------------------------------------------
	// This is the constructor of AESEngine. It allows for AES with 128, 192
	// and 256 bits keys. It does all the precomputation itself instead of
	// storing some constant values beforehand.
	// ------------------------------------------------------------------------
	// Parameters: keyLength_bits = The length of the keys that will be used
	//                              with this engine. The only accepted values
	//                              are 128, 192 and 256.
	// ------------------------------------------------------------------------
	// Throws: This function will throw an exception if keyLength_bits is
	//         non-standard.
	// ------------------------------------------------------------------------
	AESEngine::AESEngine(const int32_t keyLength_bits)
#ifdef WI_AES_LOG_REQUIRED
        : m_log("logs/aes-log", true)
#endif
	{
#ifdef WI_AES_LOG_REQUIRED
		m_log.start(true);
#endif
		if (keyLength_bits == 128)
			m_numberOfRounds = 10;
		else if (keyLength_bits == 192)
			m_numberOfRounds = 12;
		else if (keyLength_bits == 256)
			m_numberOfRounds = 14;
		else {
			string aux = string("When calling AESEngine::AESEngine(), the keyLength_bits can only be 128, 192 or 256. ") +
						 string("The constructor was called with the value ") + std::to_string(keyLength_bits) + ".";
#ifdef WI_AES_LOG_REQUIRED
			m_log.printError(aux);
			m_log.stop(true);
#endif
			throw std::domain_error(aux);
		}
#ifdef WI_AES_LOG_PRECOMP
		m_log.printInfo("Starting an AES Engine...");
#endif
		m_keyLength_bits = keyLength_bits;
		m_keyLength_bytes = m_keyLength_bits >> 3;
		m_blockSize_bits = 128;
		m_blockSize_bytes = m_blockSize_bits >> 3;
		m_N = m_keyLength_bytes >> 2;
#ifdef WI_AES_LOG_PRECOMP
		m_log.printText(
			"m_keyLength_bits = " + std::to_string(m_keyLength_bits) +
			" => m_keyLength_bytes = " + std::to_string(m_keyLength_bytes) +
			" => m_N = " + std::to_string(m_N), 1
		);
		m_log.printText(
			"m_blockSize_bits = " + std::to_string(m_blockSize_bits) +
			" => m_blockSize_bytes = " + std::to_string(m_blockSize_bytes), 1
		);
		m_log.printText("Starting precomputing...");
#endif
		for (uint32_t i = 0; i < 256; ++i) {
			m_GF256Powers[i] = m_GF256Logs[i] =
			m_SBox[i] = m_invSBox[i] = 0;
		}
		this->precompute_m_GF256Powers();
		this->precompute_m_GF256Logs();
		this->precompute_m_SBox();
		this->precompute_m_invSBox();
	}

	int32_t AESEngine::GF256_slow_product(
		int32_t a,
		int32_t b
	) {
		const int32_t c_MOD = 0x11b;
		int32_t result = 0;
		while (a) {
			if (a & 1) result ^= b;
			b <<= 1; a >>= 1;
			if (b & 0x100) b ^= c_MOD;
		}
		return result;
	}

	void AESEngine::precompute_m_GF256Powers() {
#ifdef WI_AES_LOG_PRECOMP
		m_log.printInfo("Precomputing m_GF256Powers... <=> AESEngine::precompute_m_GF256Powers()", 1);
#endif
		const int32_t c_GENERATOR = 0x03;
		m_GF256Powers[0] = 1;
		for (int32_t i = 1; i < 256; ++i)
			m_GF256Powers[i] = GF256_slow_product(
				m_GF256Powers[i - 1],
				c_GENERATOR
			);
#ifdef WI_AES_LOG_PRECOMP
		for (int32_t i = 0; i < 256; ++i)
			m_log.printText("m_GF256Powers[" + std::to_string(i) + "] = 0x" + wi::to_hex_string(m_GF256Powers[i], 2), 2);
#endif
	}
	
	void AESEngine::precompute_m_GF256Logs() {
#ifdef WI_AES_LOG_PRECOMP
		m_log.printInfo("Precomputing m_GF256Logs... <=> AESEngine::precompute_m_GF256Logs()", 1 );
#endif
		for (int32_t i = 0; i < 256 - 1; ++i)
			m_GF256Logs[m_GF256Powers[i]] = i;
#ifdef WI_AES_LOG_PRECOMP
		for (int32_t i = 0; i < 256 - 1; ++i)
			m_log.printText("m_GF256Logs[" + std::to_string(i) + "] = 0x" + wi::to_hex_string(m_GF256Logs[i], 2), 2);
#endif
	}

	int32_t AESEngine::GF256_fast_product(
		int32_t a,
		int32_t b
	) {
		if (!a || !b) return 0;
#ifdef WI_AES_SUPER_SECURE
		a %= 255;
		b %= 255;
#endif
		return m_GF256Powers[(m_GF256Logs[a] + m_GF256Logs[b]) % 255];
	}

	int32_t AESEngine::GF256_inv_mod(int32_t a) {
#ifdef WI_AES_SUPER_SECURE
		a %= 255;
		if (!a) {
			string aux = string("AESEngine::GF256_inv_mod() was called for a value that doesn't have an inverse.") +
						 string("The argument modulo 255 was a = ") + std::to_string(a) + string(".");
#ifdef WI_AES_LOG_REQUIRED
			m_log.printError(aux);
			m_log.stop(true);
#endif
			throw std::domain_error(aux);
		}
#endif
		return m_GF256Powers[0xff - m_GF256Logs[a]];
	}

	void AESEngine::precompute_m_SBox() {
#ifdef WI_AES_LOG_PRECOMP
		m_log.printInfo("Precomputing m_SBox... <=> AESEngine::precompute_m_SBox()", 1);
#endif
		m_SBox[0] = 0x63;
		for (int32_t i = 1; i < 256; ++i) {
			const int32_t inv = this->GF256_inv_mod(i);
			m_SBox[i] = inv ^ 0x63 ^
						((inv << 1) | (inv >> (8 - 1))) ^
						((inv << 2) | (inv >> (8 - 2))) ^
						((inv << 3) | (inv >> (8 - 3))) ^
						((inv << 4) | (inv >> (8 - 4)));
			m_SBox[i] &= 0xff;
		}
#ifdef WI_AES_LOG_PRECOMP
		for (int32_t i = 0; i < 256; ++i)
			m_log.printText("m_SBox[" + std::to_string(i) + "] = 0x" + wi::to_hex_string(m_SBox[i], 2), 2);
#endif
	}

	void AESEngine::precompute_m_invSBox() {
#ifdef WI_AES_LOG_PRECOMP
		m_log.printInfo("Precomputing m_invSBox... <=> AESEngine::precompute_m_invSBox()", 1);
#endif
		for (int32_t i = 0; i < 256; ++i)
			m_invSBox[m_SBox[i]] = i;
#ifdef WI_AES_LOG_PRECOMP
		for (int32_t i = 0; i < 256; ++i)
			m_log.printText("m_invSBox[" + std::to_string(i) + "] = 0x" + wi::to_hex_string(m_invSBox[i], 2), 2);
#endif
	}

	void AESEngine::layer_sub_bytes(unsigned char *block) {
		for (int32_t i = 0; i < m_blockSize_bytes; ++i)
			block[i] = m_SBox[(int32_t)block[i]];
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("After SubBytes: " + wi::uc_to_hex_string(block, 16), 3);
#endif
	}

	void AESEngine::layer_inv_sub_bytes(unsigned char *block) {
		for (int32_t i = 0; i < m_blockSize_bytes; ++i)
			block[i] = m_invSBox[(int32_t)block[i]];
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("After InvSubBytes: " + wi::uc_to_hex_string(block, 16), 3);
#endif
	}

	void AESEngine::layer_shift_rows(unsigned char *block) {
		unsigned char auxByte;
		// Second row
		auxByte   = block[ 1];
		block[ 1] = block[ 5];
		block[ 5] = block[ 9];
		block[ 9] = block[13];
		block[13] = auxByte;
		// Third row
		auxByte   = block[ 2];
		block[ 2] = block[10];
		block[10] = auxByte;
		auxByte   = block[ 6];
		block[ 6] = block[14];
		block[14] = auxByte;
		// Fourth row
		auxByte   = block[ 3];
		block[ 3] = block[15];
		block[15] = block[11];
		block[11] = block[ 7];
		block[ 7] = auxByte;
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("After ShiftRows: " + wi::uc_to_hex_string(block, 16), 3);
#endif
	}

	void AESEngine::layer_inv_shift_rows(unsigned char *block) {
		unsigned char auxByte;
		// Second row
		auxByte   = block[ 1];
		block[ 1] = block[13];
		block[13] = block[ 9];
		block[ 9] = block[ 5];
		block[ 5] = auxByte;
		// Third row
		auxByte   = block[ 2];
		block[ 2] = block[10];
		block[10] = auxByte;
		auxByte   = block[ 6];
		block[ 6] = block[14];
		block[14] = auxByte;
		// Fourth row
		auxByte   = block[ 3];
		block[ 3] = block[ 7];
		block[ 7] = block[11];
		block[11] = block[15];
		block[15] = auxByte;
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("After InvShiftRows: " + wi::uc_to_hex_string(block, 16), 3);
#endif
	}

	void AESEngine::layer_mix_column(unsigned char *block) {
		const int32_t mixColumnValues[16] = {
			2, 3, 1, 1,
			1, 2, 3, 1,
			1, 1, 2, 3,
			3, 1, 1, 2
		};
		unsigned char auxBlock[16] {};
		for (int32_t i = 0; i < 4; ++i) {
			for (int32_t k = 0; k < 4; ++k) {
				for (int32_t j = 0; j < 4; ++j) {
					auxBlock[i*4 + j] ^= this->GF256_fast_product(
						mixColumnValues[k + j*4],
						block[i*4 + k]
					);
				}
			}
		}
		memcpy(block, auxBlock, m_blockSize_bytes);
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("After MixColumn: " + wi::uc_to_hex_string(block, 16), 3);
#endif
	}

	void AESEngine::layer_inv_mix_column(unsigned char *block) {
		const int32_t mixColumnValues[16] = {
			14, 11, 13,  9,
			 9, 14, 11, 13,
			13,  9, 14, 11,
			11, 13,  9, 14
		};
		unsigned char auxBlock[16] {};
		for (int32_t i = 0; i < 4; ++i) {
			for (int32_t k = 0; k < 4; ++k) {
				for (int32_t j = 0; j < 4; ++j) {
					auxBlock[i*4 + j] ^= this->GF256_fast_product(
						mixColumnValues[k + j*4],
						block[i*4 + k]
					);
				}
			}
		}
		memcpy(block, auxBlock, m_blockSize_bytes);
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("After InvMixColumn: " + wi::uc_to_hex_string(block, 16), 3);
#endif
	}

	void AESEngine::layer_add_round_key(
        unsigned char *block,
        unsigned char *key
    ) {
		memxorcpy(block, key, m_blockSize_bytes);
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("After AddRoundKey: " + wi::uc_to_hex_string(block, 16), 3);
#endif
	}

	void AESEngine::expand_key(
		unsigned char *key,
		unsigned char *w
	) {
#ifdef WI_AES_LOG_KEY_EXP
		m_log.printInfo("Computing key expansion... <=> AESEngine::expand_key()", 3);
		m_log.printInfo("Original key: " + wi::uc_to_hex_string( key, m_keyLength_bytes, '\0', " "), 4);
#endif
		const int32_t rc[10] = {
			0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
		};
		for (int32_t i = 0; i < m_N; ++i) {
			w[i * 4]     = key[i * 4];
			w[i * 4 + 1] = key[i * 4 + 1];
			w[i * 4 + 2] = key[i * 4 + 2];
			w[i * 4 + 3] = key[i * 4 + 3];
		}
		for (int32_t i = m_N; i < (m_numberOfRounds + 1) * 4; ++i) {
			const int32_t iModN = i % m_N;
			// W[i] = W[i-1]
			w[i * 4]     = w[(i-1) * 4];
			w[i * 4 + 1] = w[(i-1) * 4 + 1];
			w[i * 4 + 2] = w[(i-1) * 4 + 2];
			w[i * 4 + 3] = w[(i-1) * 4 + 3];
			if (iModN == 0) {
				// W[i] = RotWord
				unsigned char aux = w[i * 4];
				w[i * 4]     = w[i * 4 + 1];
				w[i * 4 + 1] = w[i * 4 + 2];
				w[i * 4 + 2] = w[i * 4 + 3];
				w[i * 4 + 3] = aux;
			}
			if (iModN == 0 || (m_N > 6 && iModN == 4)) {
				// W[i] = SubWord
				w[i * 4]     = m_SBox[(int32_t)w[i * 4]];
				w[i * 4 + 1] = m_SBox[(int32_t)w[i * 4 + 1]];
				w[i * 4 + 2] = m_SBox[(int32_t)w[i * 4 + 2]];
				w[i * 4 + 3] = m_SBox[(int32_t)w[i * 4 + 3]];
			}
			// xor with W[i-1]
			w[i * 4]     ^= w[(i-m_N) * 4];
			w[i * 4 + 1] ^= w[(i-m_N) * 4 + 1];
			w[i * 4 + 2] ^= w[(i-m_N) * 4 + 2];
			w[i * 4 + 3] ^= w[(i-m_N) * 4 + 3];
			// xor rcon[i/N]
			if (iModN == 0)
				w[i * 4] ^= rc[i / m_N - 1];
		}
#ifdef WI_AES_LOG_KEY_EXP
		m_log.printInfo("Key expansion computed: " + wi::uc_to_hex_string(w, 4 * (m_numberOfRounds + 1) * 4, '\0', " "), 4);
#endif
	}

	void AESEngine::encrypt_block(
		unsigned char *expandedKeys,
		unsigned char *input,
		unsigned char *output
	) {
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing block encryption... <=> AESEngine::encrypt_block()", 1);
#endif
		for (int32_t i = 0; i < m_blockSize_bytes; ++i)
			output[i] = input[i];
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("block (named 'output') = " + wi::uc_to_hex_string(output, 16), 2);
		m_log.printInfo("Round 0:", 2);
#endif
		this->layer_add_round_key(output, expandedKeys);
		for (int32_t round = 1; round < m_numberOfRounds; ++round) {
#ifdef WI_AES_LOG_CRYPT
			m_log.printInfo("Round " + std::to_string(round) + ":", 2);
#endif
			this->layer_sub_bytes(output);
			this->layer_shift_rows(output);
			this->layer_mix_column(output);			
			this->layer_add_round_key(
				output,
				expandedKeys + round * (m_blockSize_bytes >> 2) * 4
			);
		}
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Round " + std::to_string(m_numberOfRounds) + ":", 2);
#endif
		this->layer_sub_bytes(output);
		this->layer_shift_rows(output);
		this->layer_add_round_key(
			output,
			expandedKeys + m_numberOfRounds * (m_blockSize_bytes >> 2) * 4
		);
	}

	void AESEngine::decrypt_block(
		unsigned char *expandedKeys,
		unsigned char *input,
		unsigned char *output
	) {
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing block decryption... <=> AESEngine::decrypt_block()", 1);
#endif
		for (int32_t i = 0; i < m_blockSize_bytes; ++i)
			output[i] = input[i];
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("block (named 'output') = " + wi::uc_to_hex_string(output, 16), 2);
		m_log.printInfo("Round " + std::to_string(m_numberOfRounds) + ":", 2);
#endif
		this->layer_add_round_key(
			output,
			expandedKeys + m_numberOfRounds * (m_blockSize_bytes >> 2) * 4
		);
		this->layer_inv_sub_bytes(output);
		this->layer_inv_shift_rows(output);
		for (int32_t round = m_numberOfRounds - 1; round > 0; --round) {
#ifdef WI_AES_LOG_CRYPT
			m_log.printInfo("Round " + std::to_string(round) + ":", 2);
#endif	
			this->layer_add_round_key(
				output,
				expandedKeys + round * (m_blockSize_bytes >> 2) * 4
			);
			this->layer_inv_mix_column(output);
			this->layer_inv_sub_bytes(output);
			this->layer_inv_shift_rows(output);
		}
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Round 0:", 2);
#endif
		this->layer_add_round_key(output, expandedKeys);
	}

	uint32_t AESEngine::encrypt_ECB(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output
	) {
#ifdef WI_AES_LOG_REQUIRED
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			m_log.printWarn("AESEngine::encrypt_ECB expects an input divisible by 16.");
			m_log.printWarn(
				"Will complete inputSize_bytes from " + std::to_string(inputSize_bytes) +
				" to " + std::to_string((inputSize_bytes & 0xfffffff0) + 16) +
				" with zeros behind the scenes."
			);
		}
#endif
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing ECB encryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		const uint32_t inputSizeChunks_bytes = inputSize_bytes & 0xfffffff0;
		for (uint32_t i = 0; i < inputSizeChunks_bytes; i += m_blockSize_bytes)
			this->encrypt_block(expandedKeys, input + i, output + i);
		if (inputSizeChunks_bytes != inputSize_bytes) {
			unsigned char block[m_blockSize_bytes] {};
			memcpy(
				block,
				input + inputSizeChunks_bytes,
				inputSize_bytes - inputSizeChunks_bytes
			);
			this->encrypt_block(expandedKeys, block, output + inputSizeChunks_bytes);
		}
		const uint32_t outputSize = 
			(inputSize_bytes == inputSizeChunks_bytes) ?
			inputSizeChunks_bytes :
			(inputSizeChunks_bytes + 16);
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize), 1);
#endif
		return outputSize;
	}

	uint32_t AESEngine::encrypt_CBC(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output,
		unsigned char *iv
	) {
#ifdef WI_AES_LOG_REQUIRED
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			m_log.printWarn("AESEngine::encrypt_CBC expects an input divisible by 16.");
			m_log.printWarn(
				"Will complete inputSize_bytes from " + std::to_string(inputSize_bytes) +
				" to " + std::to_string((inputSize_bytes & 0xfffffff0) + 16) +
				" with zeros behind the scenes."
			);
		}
#endif
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing CBC encryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		unsigned char block[m_blockSize_bytes];
		memcpy(block, iv, m_blockSize_bytes);

		const uint32_t inputSizeChunks_bytes = inputSize_bytes & 0xfffffff0;
		for (uint32_t i = 0; i < inputSizeChunks_bytes; i += m_blockSize_bytes) {
			memxorcpy(block, input + i, m_blockSize_bytes);
			this->encrypt_block(expandedKeys, block, output + i);
			memcpy(block, output + i, m_blockSize_bytes);
		}
		if (inputSizeChunks_bytes != inputSize_bytes) {
			memxorcpy(
				block,
				input + inputSizeChunks_bytes,
				inputSize_bytes - inputSizeChunks_bytes
			);
			this->encrypt_block(expandedKeys, block, output + inputSizeChunks_bytes);
		}
		const uint32_t outputSize = 
			(inputSize_bytes == inputSizeChunks_bytes) ?
			inputSizeChunks_bytes :
			(inputSizeChunks_bytes + 16);
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize), 1);
#endif
		return outputSize;
	}

	uint32_t AESEngine::encrypt_CFB(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output,
		unsigned char *iv
	) {
#ifdef WI_AES_LOG_REQUIRED
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			m_log.printWarn("AESEngine::encrypt_CFB expects an input divisible by 16.");
			m_log.printWarn(
				"Will complete inputSize_bytes from " + std::to_string(inputSize_bytes) +
				" to " + std::to_string((inputSize_bytes & 0xfffffff0) + 16) +
				" with zeros behind the scenes."
			);
		}
#endif
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing CFB encryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		unsigned char block[m_blockSize_bytes];
		memcpy(block, iv, m_blockSize_bytes);

		const uint32_t inputSizeChunks_bytes = inputSize_bytes & 0xfffffff0;
		for (uint32_t i = 0; i < inputSizeChunks_bytes; i += m_blockSize_bytes) {
			this->encrypt_block(expandedKeys, block, output + i);
			memxorcpy(output + i, input + i, m_blockSize_bytes);
			memcpy(block, output + i, m_blockSize_bytes);
		}
		if (inputSizeChunks_bytes != inputSize_bytes) {
			this->encrypt_block(expandedKeys, block, output + inputSizeChunks_bytes);
			memxorcpy(
				output + inputSizeChunks_bytes,
				input + inputSizeChunks_bytes,
				inputSize_bytes - inputSizeChunks_bytes
			);
		}
		const uint32_t outputSize = 
			(inputSize_bytes == inputSizeChunks_bytes) ?
			inputSizeChunks_bytes :
			(inputSizeChunks_bytes + 16);
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize), 1);
#endif
		return outputSize;
	}

	uint32_t AESEngine::encrypt_OFB(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output,
		unsigned char *iv
	) {
#ifdef WI_AES_LOG_REQUIRED
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			m_log.printWarn("AESEngine::encrypt_OFB expects an input divisible by 16.");
			m_log.printWarn(
				"Will complete inputSize_bytes from " + std::to_string(inputSize_bytes) +
				" to " + std::to_string((inputSize_bytes & 0xfffffff0) + 16) +
				" with zeros behind the scenes."
			);
		}
#endif
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing OFB encryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		unsigned char block[m_blockSize_bytes];
		memcpy(block, iv, m_blockSize_bytes);

		const uint32_t inputSizeChunks_bytes = inputSize_bytes & 0xfffffff0;
		for (uint32_t i = 0; i < inputSizeChunks_bytes; i += m_blockSize_bytes) {
			this->encrypt_block(expandedKeys, block, output + i);
			memcpy(block, output + i, m_blockSize_bytes);
			memxorcpy(output + i, input + i, m_blockSize_bytes);
		}
		if (inputSizeChunks_bytes != inputSize_bytes) {
			this->encrypt_block(expandedKeys, block, output + inputSizeChunks_bytes);
			memxorcpy(
				output + inputSizeChunks_bytes,
				input + inputSizeChunks_bytes,
				inputSize_bytes - inputSizeChunks_bytes
			);
		}
		const uint32_t outputSize = 
			(inputSize_bytes == inputSizeChunks_bytes) ?
			inputSizeChunks_bytes :
			(inputSizeChunks_bytes + 16);
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize), 1);
#endif
		return outputSize;
	}

	uint32_t AESEngine::encrypt_CTR(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output,
		void (*CTR_generator)(uint64_t, unsigned char *)
	) {
#ifdef WI_AES_LOG_REQUIRED
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			m_log.printWarn("AESEngine::encrypt_CTR expects an input divisible by 16.");
			m_log.printWarn(
				"Will complete inputSize_bytes from " + std::to_string(inputSize_bytes) +
				" to " + std::to_string((inputSize_bytes & 0xfffffff0) + 16) +
				" with zeros behind the scenes."
			);
		}
#endif
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing CTR encryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		unsigned char block[m_blockSize_bytes];
		uint64_t momentGen = 0;

		const uint32_t inputSizeChunks_bytes = inputSize_bytes & 0xfffffff0;
		for (uint32_t i = 0; i < inputSizeChunks_bytes; i += m_blockSize_bytes) {
			CTR_generator(momentGen++, block);
			this->encrypt_block(expandedKeys, block, output + i);
			memxorcpy(output + i, input + i, m_blockSize_bytes);
		}
		if (inputSizeChunks_bytes != inputSize_bytes) {
			CTR_generator(momentGen++, block);
			this->encrypt_block(expandedKeys, block, output + inputSizeChunks_bytes);
			memxorcpy(
				output + inputSizeChunks_bytes,
				input + inputSizeChunks_bytes,
				inputSize_bytes - inputSizeChunks_bytes
			);
		}
		const uint32_t outputSize = 
			(inputSize_bytes == inputSizeChunks_bytes) ?
			inputSizeChunks_bytes :
			(inputSizeChunks_bytes + 16);
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize), 1);
#endif
		return outputSize;
	}

	uint32_t AESEngine::encrypt_CTR(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output,
		unsigned char *nonce
	) {
#ifdef WI_AES_LOG_REQUIRED
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			m_log.printWarn("AESEngine::encrypt_CTR expects an input divisible by 16.");
			m_log.printWarn(
				"Will complete inputSize_bytes from " + std::to_string(inputSize_bytes) +
				" to " + std::to_string((inputSize_bytes & 0xfffffff0) + 16) +
				" with zeros behind the scenes."
			);
		}
#endif
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing CTR encryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		unsigned char block[m_blockSize_bytes];
		uint64_t momentGen = 0;

		const uint32_t inputSizeChunks_bytes = inputSize_bytes & 0xfffffff0;
		for (uint32_t i = 0; i < inputSizeChunks_bytes; i += m_blockSize_bytes) {
			memcpy(block, nonce, 8);
			for (uint32_t j = 1; j <= 8; ++j)
				block[7 + j] = ((momentGen >> (64 - 8*j)) & 0xff);
			++momentGen;
			this->encrypt_block(expandedKeys, block, output + i);
			memxorcpy(output + i, input + i, m_blockSize_bytes);
		}
		if (inputSizeChunks_bytes != inputSize_bytes) {
			memcpy(block, nonce, 8);
			for (uint32_t j = 1; j <= 8; ++j)
				block[7 + j] = ((momentGen >> (64 - 8*j)) & 0xff);
			++momentGen;
			this->encrypt_block(expandedKeys, block, output + inputSizeChunks_bytes);
			memxorcpy(
				output + inputSizeChunks_bytes,
				input + inputSizeChunks_bytes,
				inputSize_bytes - inputSizeChunks_bytes
			);
		}
		const uint32_t outputSize = 
			(inputSize_bytes == inputSizeChunks_bytes) ?
			inputSizeChunks_bytes :
			(inputSizeChunks_bytes + 16);
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize), 1);
#endif
		return outputSize;
	}

	uint32_t AESEngine::decrypt_ECB(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output
	) {
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			string aux = string("AESEngine::decrypt_ECB expects an input divisible by 16. ") +
						 string("Pased value is inputSize_bytes = ") + std::to_string(inputSize_bytes) + ".";
#ifdef WI_AES_LOG_REQUIRED
			m_log.printError(aux);
			m_log.stop(true);
#endif
			throw std::length_error(aux);
		}
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing ECB decryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		for (uint32_t i = 0; i < inputSize_bytes; i += m_blockSize_bytes)
			this->decrypt_block(expandedKeys, input + i, output + i);
		uint32_t outputSize_bytes = inputSize_bytes;
		while (outputSize_bytes > 1 && !output[outputSize_bytes - 1])
			--outputSize_bytes;
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize_bytes), 1);
#endif
		return outputSize_bytes;
	}

	uint32_t AESEngine::decrypt_CBC(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output,
		unsigned char *iv
	) {
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			string aux = string("AESEngine::decrypt_CBC expects an input divisible by 16. ") +
						 string("Pased value is inputSize_bytes = ") + std::to_string(inputSize_bytes) + ".";
#ifdef WI_AES_LOG_REQUIRED
			m_log.printError(aux);
			m_log.stop(true);
#endif
			throw std::length_error(aux);
		}
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing CBC decryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		unsigned char block[m_blockSize_bytes];
		memcpy(block, iv, m_blockSize_bytes);
		for (uint32_t i = 0; i < inputSize_bytes; i += m_blockSize_bytes) {
			this->decrypt_block(expandedKeys, input + i, output + i);
			memxorcpy(output + i, block, m_blockSize_bytes);
			memcpy(block, input + i, m_blockSize_bytes);
		}
		uint32_t outputSize_bytes = inputSize_bytes;
		while (outputSize_bytes > 1 && !output[outputSize_bytes - 1])
			--outputSize_bytes;
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize_bytes), 1);
#endif
		return outputSize_bytes;
	}

	uint32_t AESEngine::decrypt_CFB(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output,
		unsigned char *iv
	) {
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			string aux = string("AESEngine::decrypt_CFB expects an input divisible by 16. ") +
						 string("Pased value is inputSize_bytes = ") + std::to_string(inputSize_bytes) + ".";
#ifdef WI_AES_LOG_REQUIRED
			m_log.printError(aux);
			m_log.stop(true);
#endif
			throw std::length_error(aux);
		}
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing CFB decryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		unsigned char block[m_blockSize_bytes];
		memcpy(block, iv, m_blockSize_bytes);
		for (uint32_t i = 0; i < inputSize_bytes; i += m_blockSize_bytes) {
			this->encrypt_block(expandedKeys, block, output + i);
			memxorcpy(output + i, input + i, m_blockSize_bytes);
			memcpy(block, input + i, m_blockSize_bytes);
		}
		uint32_t outputSize_bytes = inputSize_bytes;
		while (outputSize_bytes > 1 && !output[outputSize_bytes - 1])
			--outputSize_bytes;
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize_bytes), 1);
#endif
		return outputSize_bytes;
	}

	uint32_t AESEngine::decrypt_OFB(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output,
		unsigned char *iv
	) {
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			string aux = string("AESEngine::decrypt_OFB expects an input divisible by 16. ") +
						 string("Pased value is inputSize_bytes = ") + std::to_string(inputSize_bytes) + ".";
#ifdef WI_AES_LOG_REQUIRED
			m_log.printError(aux);
			m_log.stop(true);
#endif
			throw std::length_error(aux);
		}
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing OFB decryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		unsigned char block[m_blockSize_bytes];
		memcpy(block, iv, m_blockSize_bytes);
		for (uint32_t i = 0; i < inputSize_bytes; i += m_blockSize_bytes) {
			this->encrypt_block(expandedKeys, block, output + i);
			memcpy(block, output + i, m_blockSize_bytes);
			memxorcpy(output + i, input + i, m_blockSize_bytes);
		}
		uint32_t outputSize_bytes = inputSize_bytes;
		while (outputSize_bytes > 1 && !output[outputSize_bytes - 1])
			--outputSize_bytes;
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize_bytes), 1);
#endif
		return outputSize_bytes;
	}

	uint32_t AESEngine::decrypt_CTR(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output,
		void (*CTR_generator)(uint64_t, unsigned char *)
	) {
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			string aux = string("AESEngine::decrypt_CTR expects an input divisible by 16. ") +
						 string("Pased value is inputSize_bytes = ") + std::to_string(inputSize_bytes) + ".";
#ifdef WI_AES_LOG_REQUIRED
			m_log.printError(aux);
			m_log.stop(true);
#endif
			throw std::length_error(aux);
		}
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing CTR decryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		unsigned char block[m_blockSize_bytes];
		uint64_t momentGen = 0;
		
		for (uint32_t i = 0; i < inputSize_bytes; i += m_blockSize_bytes) {
			CTR_generator(momentGen++, block);
			this->encrypt_block(expandedKeys, block, output + i);
			memxorcpy(output + i, input + i, m_blockSize_bytes);
		}
		uint32_t outputSize_bytes = inputSize_bytes;
		while (outputSize_bytes > 1 && !output[outputSize_bytes - 1])
			--outputSize_bytes;
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize_bytes), 1);
#endif
		return outputSize_bytes;
	}

	uint32_t AESEngine::decrypt_CTR(
		unsigned char *key,
		unsigned char *input,
		const uint32_t inputSize_bytes,
		unsigned char *output,
		unsigned char *nonce
	) {
		if ((inputSize_bytes & 0xf) != 0) { // inputSize_bytes % 16 != 0
			string aux = string("AESEngine::decrypt_CTR expects an input divisible by 16. ") +
						 string("Pased value is inputSize_bytes = ") + std::to_string(inputSize_bytes) + ".";
#ifdef WI_AES_LOG_REQUIRED
			m_log.printError(aux);
			m_log.stop(true);
#endif
			throw std::length_error(aux);
		}
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Computing CTR decryption...");
		m_log.printInfo("Full input: " + wi::uc_to_hex_string(input, inputSize_bytes), 1);
#endif
		// Set w to maximum hold
		unsigned char expandedKeys[240] {};
		this->expand_key(key, expandedKeys);

		unsigned char block[m_blockSize_bytes];
		uint64_t momentGen = 0;
		
		for (uint32_t i = 0; i < inputSize_bytes; i += m_blockSize_bytes) {
			memcpy(block, nonce, 8);
			for (uint32_t j = 1; j <= 8; ++j)
				block[7 + j] = ((momentGen >> (64 - 8*j)) & 0xff);
			++momentGen;
			this->encrypt_block(expandedKeys, block, output + i);
			memxorcpy(output + i, input + i, m_blockSize_bytes);
		}
		uint32_t outputSize_bytes = inputSize_bytes;
		while (outputSize_bytes > 1 && !output[outputSize_bytes - 1])
			--outputSize_bytes;
#ifdef WI_AES_LOG_CRYPT
		m_log.printInfo("Full output: " + wi::uc_to_hex_string(output, outputSize_bytes), 1);
#endif
		return outputSize_bytes;
	}
}
