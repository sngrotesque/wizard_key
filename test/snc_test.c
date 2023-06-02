/**
 * @file snc_test.c
 * @author SN-Grotesque
 * @brief SNC算法测试代码
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <image/wmkc_png.c>
#include <crypto/snc.c>
#include <wmkc_file.c>
#include <wmkc_random.c>
#include <wmkc_base64.c>
#include <wmkc_misc.c>
#include <wmkc_time.c>
#include <wmkc_pad.c>

#define CIPHER_TEST true

#if defined(WMKC_SNC) && (CIPHER_TEST)
static wmkcByte testKey[96] = {
    0x67, 0x65, 0x5b, 0x7b, 0x33, 0x78, 0x74, 0x6e, 0x49, 0x62, 0x6d, 0x3b, 0x7a, 0x77, 0x5d, 0x53,
    0x71, 0x52, 0x4a, 0x30, 0x3e, 0x2e, 0x63, 0x24, 0x67, 0x3b, 0x6e, 0x61, 0x6b, 0x3a, 0x21, 0x73,
    0x30, 0x66, 0x6c, 0x23, 0x74, 0x76, 0x45, 0x5d, 0x21, 0x3d, 0x50, 0x79, 0x60, 0x6c, 0x5d, 0x75,
    0x70, 0x6a, 0x68, 0x66, 0x52, 0x6e, 0x55, 0x4c, 0x7c, 0x51, 0x4e, 0x28, 0x31, 0x72, 0x7a, 0x40,
    0x49, 0x4e, 0x3b, 0x36, 0x6b, 0x69, 0x54, 0x37, 0x36, 0x70, 0x30, 0x3e, 0x3d, 0x60, 0x73, 0x42,
    0x5f, 0x61, 0x6f, 0x79, 0x7a, 0x36, 0x5e, 0x6d, 0x35, 0x40, 0x5f, 0x6d, 0x63, 0x57, 0x49, 0x31};
static wmkcByte testIv[32] = {
    0x7b, 0x75, 0x76, 0x37, 0x24, 0x59, 0x72, 0x29, 0x6e, 0x40, 0x5f, 0x65, 0x2d, 0x55, 0x75, 0x68,
    0x3d, 0x41, 0x78, 0x35, 0x48, 0x50, 0x7d, 0x73, 0x60, 0x4e, 0x33, 0x6f, 0x23, 0x47, 0x4c, 0x36};
#endif

void snc_speed_test()
{
    wmkcSNC_obj *snc = wmkcNull;
    wmkcByte *buf = wmkcNull;
    wmkcTime_obj *timer = wmkcNull;
    wmkcSize size = 268435456; // 256 MB
    wmkcFloat perSec = 0;

    wmkcMemoryNew(wmkcTime_obj *, timer, sizeof(wmkcTime_obj));
    wmkcMemoryNew(wmkcByte *, buf, size);

    SNC_new(&snc, SNC_768);
    SNC_init(snc, testKey, testIv);

    wmkcTime_TimerBegin(timer);
    SNC_CBC_Encrypt(snc, buf, size);
    wmkcTime_TimerEnd(timer);

    perSec = (wmkcFloat)size / timer->totalTime;
    printf("SNC-%u-CBC, total size: %llu, timer: %.2lf.\n",
        (snc->mode + 1) * 256, size, timer->totalTime);
    printf("\t1s, size: %.2lf Bytes, %.2lf MB.\n",
        perSec, perSec / (wmkcFloat)1048576);

    wmkcMemoryFree(timer);
    wmkcMemoryFree(buf);
    SNC_free(&snc);
}

void snc_test()
{
    wmkcSNC_obj  *snc = wmkcNull;
    wmkcByte *buf = wmkcNull;
    wmkcFile *fp = wmkcNull;
    LPCWSTR pathSrc = wmkcNull;
    LPCWSTR pathDst = wmkcNull;

    wmkcSize fileSize = 0;

    wchar_t filePathList[7][2][32] = {
        {{L"P:/SNC/snc_test_001.mp4"},  {L"P:/SNC/snc_test_001.mp4.lock"}},
        {{L"P:/SNC/snc_test_002.mp4"},  {L"P:/SNC/snc_test_002.mp4.lock"}},
        {{L"P:/SNC/snc_test_003.mp4"},  {L"P:/SNC/snc_test_003.mp4.lock"}},
        {{L"P:/SNC/snc_test_004.mp4"},  {L"P:/SNC/snc_test_004.mp4.lock"}},
        {{L"P:/SNC/snc_test_005.mp4"},  {L"P:/SNC/snc_test_005.mp4.lock"}},
        {{L"P:/SNC/snc_test_006.jpg"},  {L"P:/SNC/snc_test_006.mp4.lock"}},
        {{L"P:/SNC/snc_test_007.html"}, {L"P:/SNC/snc_test_007.html.lock"}}};

    SNC_new(&snc, SNC_768);
    SNC_init(snc, testKey, testIv);

    for(wmkc_u32 i = 0; i < 7; ++i) {
        pathSrc = filePathList[i][0];
        pathDst = filePathList[i][1];
        wmkcFile_fileSize(&fileSize, pathSrc);

        wprintf(L"path: %ls, ", pathSrc);
        printf("fileSize: %llu\n", fileSize);

        if(!wmkcMemoryNew(wmkcByte *, buf, wmkcPad_offset(SNC_BLOCKLEN, fileSize) + fileSize)) {
            printf("Memory error.\n");
            return;}

        fp = wmkcFile_fopen(pathSrc, L"rb");
        if(fread(buf, 1, fileSize, fp) != fileSize) {
            printf("fread's size not is fileSize.\n");
            return;}
        fclose(fp);

        wmkcPad_add(buf, &fileSize, SNC_BLOCKLEN, false);
        SNC_CBC_Encrypt(snc, buf, fileSize);

        fp = wmkcFile_fopen(pathDst, L"wb");
        if(fwrite(buf, 1, fileSize, fp) != fileSize) {
            printf("fwrite's size not is fileSize.\n");
            return;}
        fclose(fp);

        free(buf);
    }

    SNC_free(&snc);
}

void test()
{
    wmkcChar text[2048] = {
        "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
        "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"
    };
    wmkcByte *buf = (wmkcByte *)text;
    wmkcSize size = strlen(text);
    wmkcSNC_obj *snc  = wmkcNull;
    wmkc_u32 encrypt_method = 2;
    wmkcSize group = 32;

    SNC_new(&snc, SNC_768);
    SNC_init(snc, testKey, testIv);

    if(size % SNC_BLOCKLEN)
        wmkcPad_add(buf, &size, SNC_BLOCKLEN, false);

    printf("Cipher info:\n");
    printf("\tMode: SNC"WMKC_SET_COLOR(WMKC_FORE_COLOR_LIGHTRED, "%u, "), (snc->mode + 1) * 256);
    printf("\tKeySize: "WMKC_SET_COLOR(WMKC_FORE_COLOR_LIGHTCYAN, "%u, "),   snc->KN);
    printf("\tRounds:  "WMKC_SET_COLOR(WMKC_FORE_COLOR_LIGHTMAGENTA, "%u")"\n", snc->NR);

    printf("Plaintext:\n"); wmkcMisc_PRINT(buf, size, group, 0, true);
    printf("Key used: \n"); wmkcMisc_PRINT(snc->roundKey, snc->KN, group, 0, true);
    printf("Iv used:  \n"); wmkcMisc_PRINT(snc->iv, SNC_BLOCKLEN, group, 0, true);

    printf("Encrypt method:\n");
    if(encrypt_method == 1) {
        printf("\tECB (Electronic Codebook).\n");
        SNC_ECB_Encrypt(snc, buf, size);
    } else if(encrypt_method == 2) {
        printf("\tCBC (Cipher Block Chaining).\n");
        SNC_CBC_Encrypt(snc, buf, size);
    }

    printf("Ciphertext:\n"); wmkcMisc_PRINT(buf, size, group, 0, true);

    SNC_free(&snc);
}

int main(int argc, char **argv)
{
    test();
    return 0;
}
