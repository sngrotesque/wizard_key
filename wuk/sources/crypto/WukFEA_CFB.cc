#include <crypto/WukFEA.hh>

void wuk::crypto::FEA::cfb_encrypt(wByte *p, wSize n, wU32 segmentSize)
{
    if(segmentSize & 7) {
        throw wuk::Exception(wukErr_Err, "wuk::crypto::FEA::cfb_encrypt",
                                    "The segment size is not a multiple of 8.");
    }
    wSize i, j;
    wByte round_iv[WUK_FEA_BL];

    segmentSize >>= 3; // 将单位从位转到字节
    n = (n + segmentSize - 1) / segmentSize; // 得到总共有多少个数据段

    memcpy(round_iv, this->iv, WUK_FEA_BL);
    for(i = 0; i < n; ++i) {
        this->cipher(round_iv, this->roundKey);
        for(j = 0; j < segmentSize; ++j) {
            *(p + (i * segmentSize + j)) ^= round_iv[j];
        }
        memcpy(round_iv, p + i * segmentSize, segmentSize);
    }
}

void wuk::crypto::FEA::cfb_decrypt(wByte *c, wSize n, wU32 segmentSize)
{
    if(segmentSize & 7) {
        throw wuk::Exception(wukErr_Err, "wuk::crypto::FEA::cfb_encrypt",
                                    "The segment size is not a multiple of 8.");
    }
    wSize i, j;
    wByte round_iv[WUK_FEA_BL];
    wByte tmp_buf[WUK_FEA_BL];
    segmentSize >>= 3; // 将单位从位转到字节
    n = (n + segmentSize - 1) / segmentSize; // 得到总共有多少个数据段

    memcpy(round_iv, this->iv, WUK_FEA_BL);
    for(i = 0; i < n; ++i) {
        memcpy(tmp_buf, c + i * segmentSize, segmentSize);
        this->cipher(round_iv, this->roundKey);
        for(j = 0; j < segmentSize; ++j) {
            *(c + (i * segmentSize + j)) ^= round_iv[j];
        }
        memcpy(round_iv, tmp_buf, segmentSize);
    }
}
