#include <server/includes/WukAccount.hh>

namespace wuk::im::server {
    std::string Generator::random_string(
                    const std::vector<char> &set,
                    wuk::i32 min_size,
                    wuk::i32 max_size)
    {
        wuk::ulong result_size = this->rnd.randint(min_size, max_size);
        std::string result(result_size, '\0');
        for (auto &item : result) {
            item = this->rnd.choice(set);
        }
        return result;
    }

    wuk::i64 Generator::uid()
    {
        return this->sf.generate_id();
    }

    std::string Generator::name()
    {
        constexpr char name[] = \
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        constexpr wuk::ulong size = sizeof(name) - 1;
        std::vector<char> name_set(name, name + size);

        return this->random_string(name_set, 6, 12);
    }

    std::string Generator::password()
    {
        constexpr char password[] = \
            R"(0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz)"
            R"(!"#%&'()*+,-./:;<=>?@[\]^_`{|}~)";
        constexpr wuk::ulong password_size = sizeof(password) - 1;
        std::vector<char> password_set(password, password + password_size);

        return this->random_string(password_set, 8, 16);
    }

    wuk::Buffer Generator::salt()
    {
        return this->rnd.bytes(16);
    }

    wuk::Buffer Generator::hash(const std::string &password, const wuk::Buffer &salt)
    {
        wuk::Buffer user_hash(32);
        PKCS5_PBKDF2_HMAC(password.data(), password.size(),
                        salt.data(), salt.size(),
                        ITER_COUNT, EVP_sha256(),
                        32, user_hash.write<wuk::byte>(32));
        return user_hash;
    }

    STATUS Generator::status()
    {
        return static_cast<STATUS>(this->rnd.randint(0, 3));
    }

    wuk::f64 Generator::created()
    {
        return this->tm.time<wuk::f64>();
    }

    wuk::f64 Generator::modified()
    {
        return this->created();
    }

    wuk::f64 Generator::accessed()
    {
        return this->created();
    }

    wuk::f64 Generator::deleted()
    {
        return this->created();
    }

    std::string Generator::uid_str()
    {
        return fmt::format("{0}", this->uid());
    }

    std::string Generator::name_str()
    {
        return this->name();
    }

    std::string Generator::password_str()
    {
        return this->password();
    }

    std::string Generator::salt_str()
    {
        return this->salt().to_str();
    }

    std::string Generator::hash_str(const std::string &password, const wuk::Buffer &salt)
    {
        return this->hash(password, salt).to_str();
    }

    std::string Generator::status_str()
    {
        std::vector<std::string> set{
            "NORMAL",
            "BANNED",
            "MUTED",
            "DELETED"
        };
        return set.at(static_cast<int>(this->status()));
    }

    std::string Generator::created_str()
    {
        return fmt::format("{0:.3f}", this->created());
    }

    std::string Generator::modified_str()
    {
        return fmt::format("{0:.3f}", this->modified());
    }

    std::string Generator::accessed_str()
    {
        return fmt::format("{0:.3f}", this->accessed());
    }

    std::string Generator::deleted_str()
    {
        return fmt::format("{0:.3f}", this->deleted());
    }
}


namespace wuk::im::server {
    Account::Account(const std::string &bind_addr, wuk::u16 bind_port)
        : fd(AF_INET, SOCK_STREAM, IPPROTO_TCP)
    {
        this->fd.setsockopt(SOL_SOCKET, SO_REUSEADDR, true);
        this->fd.set_timeout(300);
        this->fd.bind(bind_addr, bind_port);
        this->fd.listen(30000);
    }

    // 先以单个客户端的方式试运行。
    void Account::hanlder()
    {
        while (true) {
            try {

            } catch (wuk::Exception &e) {
                if (e.get_err_code() == 1) {
                    continue;
                } else {
                    throw;
                }
            }
        }
    }
}
