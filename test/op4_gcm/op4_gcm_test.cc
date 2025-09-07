#include "op4_gcm.hh"
#include "op4_gcm.cc"
#include <WukMisc.hh>
#include <iostream>

int main()
{
    wuk::byte key[OP4_KL]{};
    wuk::byte nonce[OP4_NL]{};
    wuk::byte plaintext[OP4_BL]{1};
    wuk::byte ciphertext[OP4_BL]{};
    wuk::byte decrypted[OP4_BL]{};
    wuk::byte tag[OP4_BL]{};

    OP4 op4cipher_encrypt(key);
    OP4 op4cipher_decrypt(key);

    OP4_GCM encryption(op4cipher_encrypt);
    OP4_GCM decryption(op4cipher_decrypt);

    encryption.encrypt(ciphertext, plaintext, OP4_BL, tag, nonce);

    // *tag ^= 1;

    bool suss = decryption.decrypt(decrypted, ciphertext, OP4_BL, tag, nonce);

    std::cout << ((suss) ? "tag true" : "tag false") << "\n";

    if (memcmp(plaintext, decrypted, OP4_BL) == 0) {
        std::cout << "True\n";
    } else {
        std::cout << "False\n";
    }

    std::cout << "Tag:\n";
    wuk::misc::print_hex(tag, 16, 16, true, true);
    std::cout << "Plaintext:\n";
    wuk::misc::print_hex(plaintext, 16, 16, true, true);
    std::cout << "Ciphertext:\n";
    wuk::misc::print_hex(ciphertext, 16, 16, true, true);
    std::cout << "Decrypted:\n";
    wuk::misc::print_hex(decrypted, 16, 16, true, true);

    return 0;
}
