#include <config/wmkc.hpp>

#if WMKC_SUPPORT
#ifndef WMKC_CPP_SNC
#define WMKC_CPP_SNC
#include <config/exception.hpp>
#include <memory.hpp>

#define SNC_BLOCKLEN 32
#define SNC_NB 8
#define SNC_NK 4

static const wByte SNC_NR[3] = {7, 8, 9};
static const wByte SNC_KN[3] = {32, 64, 96};

namespace wmkc {
    namespace crypto {
        typedef wByte state_t[SNC_NB][SNC_NK];
        typedef enum snc_keyMode {SNC_256, SNC_512, SNC_768} snc_keyMode;
        enum class snc_xcryptMode {ECB, CBC, CTR, CFB};

        class snc {
            private:
                wU16 numberRounds;
                wU16 subKey_length;
                snc_keyMode keyMode;
                wByte iv[SNC_BLOCKLEN];
                wByte rk[864];

                wVoid subBytes(state_t *state);
                wVoid rowsMix(state_t *state);
                wVoid columnShift(state_t *state);

                wVoid invSubBytes(state_t *state);
                wVoid invRowsMix(state_t *state);
                wVoid invColumnShift(state_t *state);

                wVoid xorWithIv(state_t *buf, state_t *iv);

                wVoid cipher(state_t *state, wByte *roundKey);
                wVoid invCipher(state_t *state, wByte *roundKey);

                wVoid keyExtension(wU16 keySize, wByte *iv, wByte *key);

                wVoid ecb_encrypt(wByte *content, wSize size);
                wVoid ecb_decrypt(wByte *content, wSize size);
                wVoid cbc_encrypt(wByte *content, wSize size);
                wVoid cbc_decrypt(wByte *content, wSize size);
                wVoid ctr_xcrypt(wByte *content, wSize size);
                wVoid cfb_encrypt(wByte *content, wSize size, wSize segment_size);
                wVoid cfb_decrypt(wByte *content, wSize size, wSize segment_size);
            public:
                wU32 segmentSize;
                snc(const wByte *keyBuf, const wByte *ivBuf, snc_keyMode mode = snc_keyMode::SNC_256, wU32 segment_size = 256);
                ~snc();
                wVoid encrypt(wByte *content, wSize size, snc_xcryptMode xcryptMode = snc_xcryptMode::CBC);
                wVoid decrypt(wByte *content, wSize size, snc_xcryptMode xcryptMode = snc_xcryptMode::CBC);
                std::string encrypt(std::string content, snc_xcryptMode xcryptMode = snc_xcryptMode::CBC);
                std::string decrypt(std::string content, snc_xcryptMode xcryptMode = snc_xcryptMode::CBC);
        };
    }
}

#endif /* WMKC_CPP_SNC */
#endif /* WMKC_SUPPORT */
