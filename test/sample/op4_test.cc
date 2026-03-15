#include <crypto/WukOP4.hh>
#include <WukRandom.hh>
#include <WukMisc.hh>

void test()
{
    wuk::Random random;
    wuk::Buffer plaintext("hello, world");
    wuk::Buffer ciphertext;
    wuk::Buffer decrypted;

    wuk::Buffer key(random.bytes(32));
    wuk::Buffer nonce(random.bytes(12));

    wuk::crypto::OP4 op4(key);

    ciphertext = std::move(op4.ctr_stream(plaintext, nonce));

    op4.reset_counter();

    decrypted = std::move(op4.ctr_stream(ciphertext, nonce));

    fmt::print("Plaintext:\n");
    wuk::misc::print_hex(plaintext.data(), plaintext.size(), 16, true, true);

    fmt::print("Ciphertext:\n");
    wuk::misc::print_hex(ciphertext.data(), ciphertext.size(), 16, true, true);

    fmt::print("Decrypted:\n");
    wuk::misc::print_hex(decrypted.data(), decrypted.size(), 16, true, true);

    fmt::print("plaintext {} decrypted.\n", ((plaintext == decrypted) ? "==" : "!="));

    decrypted = std::move(op4.ctr_stream(ciphertext, nonce));

    fmt::print("Decrypted:\n");
    wuk::misc::print_hex(decrypted.data(), decrypted.size(), 16, true, true);

    fmt::print("plaintext {} decrypted.\n", ((plaintext == decrypted) ? "==" : "!="));
}

int main()
{
    test();

    return 0;
}
