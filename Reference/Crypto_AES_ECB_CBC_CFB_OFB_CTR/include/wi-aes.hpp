// Copyright (C) 2022 Valentin-Ioan VINTILA.
// All rights reserved.

#ifndef WI_AES_HPP
#define WI_AES_HPP

#include <functional>
#include <iostream>
#include <cstring>
#include <string>

#include "wi-utilities.hpp"

#ifdef WI_AES_LOG_ALL
    #define WI_AES_LOG_KEY_EXP
    #define WI_AES_LOG_CRYPT
    #define WI_AES_LOG_PRECOMP
#endif

#if (defined(WI_AES_LOG_KEY_EXP) || defined(WI_AES_LOG_CRYPT) || \
     defined(WI_AES_LOG_PRECOMP))
    #define WI_AES_LOG_REQUIRED
#endif

namespace wi {
    class AESEngine {
    private:
        int32_t m_GF256Powers[256];
        int32_t m_GF256Logs[256];
        int32_t m_SBox[256];
        int32_t m_invSBox[256];

        int32_t m_keyLength_bits, m_keyLength_bytes;
        int32_t m_blockSize_bits, m_blockSize_bytes;
        int32_t m_numberOfRounds, m_N;

#ifdef WI_AES_LOG_REQUIRED
        wi::Log m_log;
#endif

    public:
        AESEngine(const int32_t keyLength_bits);

        uint32_t encrypt_ECB(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output
        );

        uint32_t encrypt_CBC(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output,
            unsigned char *iv
        );

        uint32_t encrypt_CFB(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output,
            unsigned char *iv
        );

        uint32_t encrypt_OFB(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output,
            unsigned char *iv
        );
        
        uint32_t encrypt_CTR(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output,
            void (*CTR_generator)(uint64_t, unsigned char *)
        );

        uint32_t encrypt_CTR(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output,
            unsigned char *nonce
        );

        uint32_t decrypt_ECB(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output
        );

        uint32_t decrypt_CBC(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output,
            unsigned char *iv
        );

        uint32_t decrypt_CFB(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output,
            unsigned char *iv
        );

        uint32_t decrypt_OFB(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output,
            unsigned char *iv
        );
        
        uint32_t decrypt_CTR(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output,
            void (*CTR_generator)(uint64_t, unsigned char *)
        );

        uint32_t decrypt_CTR(
            unsigned char *key,
            unsigned char *input,
            const uint32_t inputSize_bytes,
            unsigned char *output,
            unsigned char *nonce
        );
    
    private:
        int32_t GF256_slow_product(
            int32_t a,
            int32_t b
        );

		void precompute_m_GF256Powers();
		void precompute_m_GF256Logs();

		int32_t GF256_fast_product(
            int32_t a,
            int32_t b
        );
		int32_t GF256_inv_mod(int32_t a);

		void precompute_m_SBox();
		void precompute_m_invSBox();

        void layer_sub_bytes(unsigned char *block);
        void layer_inv_sub_bytes(unsigned char *block);
        void layer_shift_rows(unsigned char *block);
        void layer_inv_shift_rows(unsigned char *block);
        void layer_mix_column(unsigned char *block);
        void layer_inv_mix_column(unsigned char *block);
        void layer_add_round_key(
            unsigned char *block,
            unsigned char *key
        );

        void expand_key(
            unsigned char *key,
            unsigned char *w
        );

        void encrypt_block(
            unsigned char *expandedKeys,
            unsigned char *input,
            unsigned char *output
        );

        void decrypt_block(
            unsigned char *expandedKeys,
            unsigned char *input,
            unsigned char *output
        );
    };
}

#endif // WI_AES_HPP
