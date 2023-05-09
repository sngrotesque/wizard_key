// Copyright (C) 2022 Valentin-Ioan VINTILA.
// All rights reserved.

// These tests are inspired by the tests used by SergeyBel.

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

// SB (C++ alternative implementation)
#include "SergeyBel-AES/AES.h"

// DH (C alternative implementation)
extern "C" {
    uint8_t *aes_init(size_t key_size);
    void aes_key_expansion(
        uint8_t *key,
        uint8_t *w
    );
    void aes_cipher(
        uint8_t *in,
        uint8_t *out,
        uint8_t *w
    );
    void aes_inv_cipher(
        uint8_t *in,
        uint8_t *out,
        uint8_t *w
    );
}

// WI (my implementation)
#include "../include/wi-aes.hpp"

namespace wi {
    bool AES_enc_ECB_test_gen(
        wi::Log& log,
        unsigned char uc_plain[],
        const uint32_t uc_plainLength,
        unsigned char uc_key[],
        const uint32_t uc_keyLength
    ) {
        bool result = true;
        // SB
        AES SB_aes(static_cast<AESKeyLength>(uc_keyLength));
        unsigned char *SB_out = SB_aes.EncryptECB(
            uc_plain,
            uc_plainLength,
            uc_key
        );
        log.printInfo(string("SB Result: ") + wi::uc_to_hex_string(SB_out, uc_plainLength), 2);
        // DH
        unsigned char *DH_out = new unsigned char[uc_plainLength]();
        unsigned char *DH_expandedKey = aes_init(uc_keyLength);
        aes_key_expansion(uc_key, DH_expandedKey);
        for (uint32_t i = 0; i < uc_plainLength; i += 16)
            aes_cipher(
                uc_plain + i,
                DH_out + i,
                DH_expandedKey
            );
        log.printInfo(string("DH Result: ") + wi::uc_to_hex_string(DH_out, uc_plainLength), 2);
        // WI
		wi::AESEngine WI_aes(uc_keyLength << 3);
        unsigned char *WI_out = new unsigned char[uc_plainLength]();
        WI_aes.encrypt_ECB(
            uc_key,
            uc_plain,
            uc_plainLength,
            WI_out
        );
        log.printInfo(string("WI Result: ") + wi::uc_to_hex_string(WI_out, uc_plainLength), 2);

        if (memcmp(SB_out, WI_out, uc_plainLength) != 0 ||
            memcmp(DH_out, WI_out, uc_plainLength) != 0)
            result = false;

        delete[] SB_out;
        delete[] DH_out;
        free(DH_expandedKey);
        delete[] WI_out;
        return result;
    }

    bool AES_dec_ECB_test_gen(
        wi::Log& log,
        unsigned char uc_encrypted[],
        const uint32_t uc_encryptedLength,
        unsigned char uc_key[],
        const uint32_t uc_keyLength
    ) {
        bool result = true;
        // SB
        AES SB_aes(static_cast<AESKeyLength>(uc_keyLength));
        unsigned char *SB_out = SB_aes.DecryptECB(
            uc_encrypted,
            uc_encryptedLength,
            uc_key
        );
        log.printInfo(string("SB Result: ") + wi::uc_to_hex_string(SB_out, uc_encryptedLength), 2);
        // DH
        unsigned char *DH_out = new unsigned char[uc_encryptedLength]();
        unsigned char *DH_expandedKey = aes_init(uc_keyLength);
        aes_key_expansion(uc_key, DH_expandedKey);
        for (uint32_t i = 0; i < uc_encryptedLength; i += 16)
            aes_inv_cipher(
                uc_encrypted + i,
                DH_out + i,
                DH_expandedKey
            );
        log.printInfo(string("DH Result: ") + wi::uc_to_hex_string(DH_out, uc_encryptedLength), 2);
        // WI
		wi::AESEngine WI_aes(uc_keyLength << 3);
        unsigned char *WI_out = new unsigned char[uc_encryptedLength]();
        WI_aes.decrypt_ECB(
            uc_key,
            uc_encrypted,
            uc_encryptedLength,
            WI_out
        );
        log.printInfo(string("WI Result: ") + wi::uc_to_hex_string(WI_out, uc_encryptedLength), 2);

        if (memcmp(SB_out, WI_out, uc_encryptedLength) != 0 ||
            memcmp(DH_out, WI_out, uc_encryptedLength) != 0)
            result = false;

        delete[] SB_out;
        delete[] DH_out;
        free(DH_expandedKey);
        delete[] WI_out;
        return result;
    }

    bool AES_test_1(wi::Log& log) {
        log.printInfo("Running AES Test 1:", 1);
        unsigned char uc_plain[] = {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
            0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
        };
        unsigned char uc_key[] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        };
        return AES_enc_ECB_test_gen(
            log,
            uc_plain, sizeof(uc_plain),
            uc_key, sizeof(uc_key)
        );
    }

    bool AES_test_2(wi::Log& log) {
        log.printInfo("Running AES Test 2:", 1);
        unsigned char uc_plain[] = {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
            0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
        };
        unsigned char uc_key[] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
        };
        return AES_enc_ECB_test_gen(
            log,
            uc_plain, sizeof(uc_plain),
            uc_key, sizeof(uc_key)
        );
    }

    bool AES_test_3(wi::Log& log) {
        log.printInfo("Running AES Test 3:", 1);
        unsigned char uc_plain[] = {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
            0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
        };
        unsigned char uc_key[] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
        };
        return AES_enc_ECB_test_gen(
            log,
            uc_plain, sizeof(uc_plain),
            uc_key, sizeof(uc_key)
        );
    }

    bool AES_test_4(wi::Log& log) {
        log.printInfo("Running AES Test 4:", 1);
        unsigned char uc_plain[] = {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
            0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
        };
        unsigned char uc_key[] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        };
        return AES_enc_ECB_test_gen(
            log,
            uc_plain, sizeof(uc_plain),
            uc_key, sizeof(uc_key)
        );
    }

    bool AES_test_5(wi::Log& log) {
        log.printInfo("Running AES Test 5:", 1);
        unsigned char uc_encrypted[] = {
            0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30,
            0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a
        };
        unsigned char uc_key[] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
        };
        return AES_dec_ECB_test_gen(
            log,
            uc_encrypted, sizeof(uc_encrypted),
            uc_key, sizeof(uc_key)
        );
    }

    bool AES_test_all() {
        wi::Log log("results", false);
        log.start();
        log.printInfo("Testing AES...");

        bool ok = true;
        ok &= wi::AES_test_1(log);
        ok &= wi::AES_test_2(log);
        ok &= wi::AES_test_3(log);
        ok &= wi::AES_test_4(log);
        ok &= wi::AES_test_5(log);

        if (ok)
            log.printInfo("Every test went alright!");
        else
            log.printWarn("At least one test was incorrect!");

        log.stop();
        return ok;
    }
}

int main() {
    wi::AES_test_all();
    return 0;
}
