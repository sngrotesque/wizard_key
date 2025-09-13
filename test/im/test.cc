#include <crypto/WukChaCha20.hh>
#include <WukBinascii.hh>
#include <WukBuffer.hh>
#include <WukRandom.hh>
#include <openssl/evp.h>

#include <im/WukIM.hh>
#include <im/WukPsql.hh>
#include <iostream>
#include <vector>

constexpr wuk::u32 salt_size = 16;
constexpr wuk::u32 iterations = 102401;

wuk::Random random;

wuk::Buffer derive_key_and_nonce(const std::string &password,
                                 const wuk::Buffer &salt)
{
    using namespace wuk::crypto;
    wuk::Buffer derived;
    wuk::u32 dklen = WukCC20_KL + WukCC20_NL;

    PKCS5_PBKDF2_HMAC(password.data(), password.length(),
                      salt.data(), salt.get_length(),
                      102401, EVP_sha256(), dklen,
                      derived.write(dklen));
    return derived;
}

wuk::Buffer chacha20_encrypt(const wuk::Buffer &data, const std::string &password)
{
    wuk::Buffer salt = random.bytes(salt_size);
    wuk::Buffer derived = derive_key_and_nonce(password, salt);
    const wuk::byte *key = derived.data();
    const wuk::byte *nonce = derived.data() + wuk::crypto::WukCC20_KL;

    wuk::crypto::ChaCha20 cipher(key);

    wuk::Buffer ciphertext;
    cipher.crypto_stream(ciphertext.append(data.get_length()),
                         data.data(),
                         data.get_length(),
                         nonce);
    derived.clear(true);

    wuk::Buffer final_ciphertext = salt + ciphertext;

    return final_ciphertext;
}

wuk::Buffer chacha20_decrypt(const wuk::Buffer &data, const std::string &password)
{
    wuk::Buffer salt(data.data(), salt_size);
    wuk::Buffer derived = derive_key_and_nonce(password, salt);
    const wuk::byte *key = derived.data();
    const wuk::byte *nonce = derived.data() + wuk::crypto::WukCC20_KL;

    wuk::Buffer ciphertext(data.data() + salt_size, data.get_length() - salt_size);
    wuk::Buffer plaintext;

    wuk::crypto::ChaCha20 cipher(key);
    cipher.crypto_stream(plaintext.append(ciphertext.get_length()),
                         ciphertext.data(),
                         ciphertext.get_length(),
                         nonce);
    derived.clear(true);

    return plaintext;
}

void psql_query_test(wuk::im::Psql &psql)
{
    auto res = psql.query_all("SELECT * FROM users WHERE name = $1", {"sngrotesque"});

    for (const auto &items : res) {
        for (const auto &item : items) {
            std::cout << item;
            std::cout << "\t";
        }
        std::cout << std::endl;
    }
}

void psql_insert_test(wuk::im::Psql &psql)
{
    wuk::im::UserInfo (*create_account)(const void *, const void *) = nullptr;

    std::string sql(
        "INSERT INTO users (uid, name, salt, pwd_hash, created, active)\n"
        "VALUES ($1, $2, $3, $4, $5, $6)"
    );
    wuk::im::UserInfo info = create_account("我的", "12345678");
    std::vector<std::string> params{
        info.get_uid_str(),
        info.get_name_str(),
        info.get_salt_str(),
        info.get_hash_str(),
        info.get_cadt_str(),
        info.get_active_str()
    };

    psql.insert(sql, params);
}

void psql_test()
{
#   ifdef A
    auto host_hex = wuk::binascii::a2b_hex("0941f1eab5cc2f2b09b10143675cef871812511d8fd29d0e73686df4ce");
    auto password_hex = wuk::binascii::a2b_hex("966fb67b33be9f22461d78203e376ca56e5b2214785945a48fd3516bcf06e6");
    std::string derive_password("");
    std::string host = chacha20_decrypt(host_hex, derive_password).to_str();
    std::string password = chacha20_decrypt(password_hex, derive_password).to_str();
    wuk::u16 port = 54324;
#   else
    std::string host("127.0.0.1");
    std::string password("sngrotesque");
    wuk::u16 port = 5432;
#   endif

    wuk::im::PsqlConnInfo psql_conninfo;
    wuk::im::Psql psql;
    psql_conninfo.set_host(host)
                 .set_port(port)
                 .set_user("postgres")
                 .set_dbname("im")
                 .set_password(password);

    psql.connect_db(psql_conninfo);
    std::cout << "Connection complete.\n";

    psql_insert_test(psql);
}

int main()
{
    try {
        psql_test();
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
