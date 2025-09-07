#pragma once
#include <crypto/WukOP4.hh>
#include <cstdint>
#include <array>
#include <vector>

using namespace wuk::crypto;

class OP4_GCM {
private:
    OP4& op4;
    uint8_t HASH_SUBKEY[16]{}; // 用于GMAC的哈希子密钥

    // GHASH函数实现
    void ghash(uint8_t hash[16], const uint8_t* data, size_t length, 
               const uint8_t* aad, size_t aad_length);

    // 数据填充到16字节倍数
    std::vector<uint8_t> pad_data(const uint8_t* data, size_t length);

    // 处理长度块
    void process_lengths(uint8_t block[16], size_t aad_len, size_t data_len);

public:
    // 构造函数，需要传入一个OP4实例
    explicit OP4_GCM(OP4& op4_instance);

    // 加密并生成认证标签
    // 参数: out-密文输出, tag-认证标签(16字节), in-明文, length-明文长度, 
    //       nonce-随机数(12字节), aad-附加认证数据, aad_length-附加数据长度
    void encrypt(uint8_t* out, const uint8_t* in, size_t length,
                uint8_t tag[16], const uint8_t nonce[12],
                const uint8_t* aad = nullptr, size_t aad_length = 0);

    // 解密并验证认证标签
    // 参数: out-明文输出, in-密文, length-密文长度, tag-认证标签(16字节),
    //       nonce-随机数(12字节), aad-附加认证数据, aad_length-附加数据长度
    // 返回值: 验证成功返回true，否则返回false
    bool decrypt(uint8_t* out, const uint8_t* in, size_t length, 
                const uint8_t tag[16], const uint8_t nonce[12],
                const uint8_t* aad = nullptr, size_t aad_length = 0);
};
