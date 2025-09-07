#include "op4_gcm.hh"
#include <cstring>
#include <algorithm>

OP4_GCM::OP4_GCM(OP4& op4_instance) : op4(op4_instance) {
    // 生成哈希子密钥：加密全零块
    uint8_t zero_block[16] = {0};
    op4.ecb_encrypt(HASH_SUBKEY, zero_block, 16);
    // 注意：在实际实现中，应该将temp_hash_key的值赋给HASH_SUBKEY
    // 这里为了演示简化处理
}

// 伽罗瓦域GF(2^128)乘法
static void gfmul(uint8_t *x, const uint8_t *y) {
    uint8_t z[16] = {0};
    uint8_t v[16];
    memcpy(v, y, 16);

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            if ((x[i] >> (7 - j)) & 1) {
                // z = z XOR v
                for (int k = 0; k < 16; k++) {
                    z[k] ^= v[k];
                }
            }

            // 检查v的最高位
            uint8_t carry = v[0] >> 7;

            // 左移一位
            for (int k = 0; k < 15; k++) {
                v[k] = (v[k] << 1) | (v[k + 1] >> 7);
            }
            v[15] <<= 1;

            // 如果有进位，与R = 0x87异或
            if (carry) {
                v[15] ^= 0x87;
            }
        }
    }
    memcpy(x, z, 16);
}

void OP4_GCM::ghash(uint8_t hash[16], const uint8_t* data, size_t length, 
                   const uint8_t* aad, size_t aad_length) {
    // 初始化哈希值为0
    memset(hash, 0, 16);

    // 处理AAD数据
    if (aad && aad_length > 0) {
        std::vector<uint8_t> padded_aad = pad_data(aad, aad_length);
        for (size_t i = 0; i < padded_aad.size(); i += 16) {
            // hash = (hash XOR block) * H
            for (int j = 0; j < 16; j++) {
                hash[j] ^= padded_aad[i + j];
            }
            gfmul(hash, HASH_SUBKEY);
        }
    }

    // 处理密文数据
    if (data && length > 0) {
        std::vector<uint8_t> padded_data = pad_data(data, length);
        for (size_t i = 0; i < padded_data.size(); i += 16) {
            // hash = (hash XOR block) * H
            for (int j = 0; j < 16; j++) {
                hash[j] ^= padded_data[i + j];
            }
            gfmul(hash, HASH_SUBKEY);
        }
    }

    // 处理长度块
    uint8_t length_block[16];
    process_lengths(length_block, aad_length * 8, length * 8);

    // 最后一步：hash = (hash XOR length_block) * H
    for (int j = 0; j < 16; j++) {
        hash[j] ^= length_block[j];
    }
    gfmul(hash, HASH_SUBKEY);
}

std::vector<uint8_t> OP4_GCM::pad_data(const uint8_t* data, size_t length) {
    // 计算需要填充的字节数，使其成为16的倍数
    size_t pad_length = (16 - (length % 16)) % 16;
    std::vector<uint8_t> padded(length + pad_length);

    // 复制原始数据
    memcpy(padded.data(), data, length);

    // 填充0
    if (pad_length > 0) {
        memset(padded.data() + length, 0, pad_length);
    }

    return padded;
}

void OP4_GCM::process_lengths(uint8_t block[16], size_t aad_len, size_t data_len) {
    // 初始化块为0
    memset(block, 0, 16);

    // 存储AAD长度（64位大端格式）
    for (int i = 0; i < 8; i++) {
        block[i] = (aad_len >> (8 * (7 - i))) & 0xFF;
    }

    // 存储数据长度（64位大端格式）
    for (int i = 0; i < 8; i++) {
        block[i + 8] = (data_len >> (8 * (7 - i))) & 0xFF;
    }
}

void OP4_GCM::encrypt(uint8_t* out, const uint8_t* in, size_t length,
                     uint8_t tag[16], const uint8_t nonce[12],
                     const uint8_t* aad, size_t aad_length) {
    // 1. 使用CTR模式加密数据
    op4.ctr_stream(out, in, length, nonce);

    // 2. 生成GHASH值作为认证标签
    uint8_t hash[16];
    ghash(hash, out, length, aad, aad_length);

    // 3. 生成J0（初始计数器块）并加密，用于与哈希结果异或生成标签
    uint8_t j0[16] = {0};
    memcpy(j0, nonce, 12);
    // j0的最后4字节设为0
    uint8_t encrypted_j0[16];
    op4.ecb_encrypt(encrypted_j0, j0, 16);

    // 4. 计算最终标签：hash XOR encrypted_j0
    for (int i = 0; i < 16; i++) {
        tag[i] = hash[i] ^ encrypted_j0[i];
    }
}

bool OP4_GCM::decrypt(uint8_t* out, const uint8_t* in, size_t length, 
                     const uint8_t tag[16], const uint8_t nonce[12],
                     const uint8_t* aad, size_t aad_length) {
    // 1. 生成GHASH值
    uint8_t hash[16];
    ghash(hash, in, length, aad, aad_length);

    // 2. 生成J0并加密
    uint8_t j0[16] = {0};
    memcpy(j0, nonce, 12);
    uint8_t encrypted_j0[16];
    op4.ecb_encrypt(encrypted_j0, j0, 16);

    // 3. 计算期望的标签并与输入标签比较
    uint8_t expected_tag[16];
    for (int i = 0; i < 16; i++) {
        expected_tag[i] = hash[i] ^ encrypted_j0[i];
    }

    if (memcmp(expected_tag, tag, 16) != 0) {
        return false; // 标签不匹配，验证失败
    }

    // 4. 验证成功，使用CTR模式解密
    op4.ctr_stream(out, in, length, nonce);
    return true;
}
