// Copyright (C) 2022 Valentin-Ioan VINTILA.
// All rights reserved.

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

#include "include/wi-aes.hpp"

int main() {
	wi::AESEngine engine(192);
	unsigned char in[128] = "The Cake Is A Lie", enc[1024]{}, dec[1024]{};
	unsigned char key[64] = "abcdefghijklmnopqrstuvwx", iv[64] = "0123456789ABCDEF";
	const uint32_t inSize = 17;

	cout << "--------------------" << endl;
	cout << "Unencrypted message: " << wi::uc_to_hex_string(in, inSize) << endl;
	cout << "--------------------" << endl;
	const uint32_t encLength = engine.encrypt_CTR(key, in, inSize, enc, iv);
	cout << "  Encryption length: " << encLength << endl;
	cout << "  Encrypted message: " << wi::uc_to_hex_string(enc, encLength) << endl;
	cout << "--------------------" << endl;
	const uint32_t decLength = engine.decrypt_CTR(key, enc, encLength, dec, iv);
	cout << "  Decryption length: " << decLength << endl;
	cout << "  Decrypted message: " << wi::uc_to_hex_string(dec, decLength) << endl;
	cout << "--------------------" << endl;
	return 0;
}