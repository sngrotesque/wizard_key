#include "./mode.h"
#include "./utils.h"
#include "./AES.h"


// len stands for the length of string in xor
static void byte_xor(uint8_t* data, const uint8_t* in, int len) {
    for (int i=0; i<len; i++) {
        data[i] ^= in[i];
    }
}


static void Move(uint8_t* data, uint8_t in) {
    for (int i=0; i<15; i++) {
        data[i] = data[i+1];
    }
    data[15] = in;
}

// ECB
void ECB() {
    readToMemory();
    cipherText = (char*)malloc(plainSize+1);
    byteCipherText = (uint8_t*)malloc(plainSize/2*sizeof(uint8_t));
    if (!cipherText || !byteCipherText) {
        printf("[-] Failed to allocate memory\n");
        exit(-1);
    }
    cipherText[plainSize] = '\0';
    printf("\n[*] Test launched, generating cipher\n[*] Writing cipher...\n");
    ECB_Encode();

    FILE* fp = fopen(cipherFile, "w");
    if (fp == NULL) {
        printf("[-] Failed to load the specific file");
        exit(1);
    }
    fputs(cipherText, fp);
    fclose(fp);
    putchar('\n');

    // Speed test
    time_t c_start, c_end;
    c_start = clock();
    int count = 1;
    printf("[*] 10 tests launched.\n");
    for (; count <= 10; count++) {
        printf("[*] No. %d encode and decode\n", count);
        ECB_Encode();
        ECB_Decode();
    }
    c_end = clock();
    printf("\n[*] ECB time cost: %.2lfms\n\n", difftime(c_end, c_start));
}


void ECB_Encode() {
    strToByte(plainText, bytePlainText, plainSize);
    int groupNum = plainSize / 2 / 16;
    uint8_t byteCurrentPlain[16];
    uint8_t byteFinal[16];
    char final[33];
    final[32] = '\0';

    for (int i=0; i<groupNum; i++) {
        for (int j=0; j<16; j++) {
            byteCurrentPlain[j] = bytePlainText[j+i*16];
        }
        AESEncrypt(byteKeyText, 16, byteCurrentPlain,byteFinal);
        byteToStr(byteFinal, final, 16);
        for (int j=0; j<32; j++) {
            cipherText[i*32+j] = final[j];
        }
    }
    printf("[+] Cipher text: ");
    for (int i=0; i<64; i++) {
        printf("%c", cipherText[i]);
    }
    printf("...\n");
}


void ECB_Decode() {
    strToByte(cipherText, byteCipherText, plainSize);
    int groupNum = plainSize / 2 / 16;
    uint8_t byteCurrentCipher[16];
    uint8_t byteFinal[16];
    char final[33];
    final[32] = '\0';

    for (int i=0; i<groupNum; i++) {
        for (int j=0; j<16; j++) {
            byteCurrentCipher[j] = byteCipherText[j+i*16];
        }
        AESDecrypt(byteKeyText, 16, byteCurrentCipher, byteFinal);
        byteToStr(byteFinal, final, 16);
        for (int j=0; j<32; j++) {
            plainText[i*32+j] = final[j];
        }
    }
    printf("[+] Plain text: ");
    for (int i=0; i<64; i++) {
        printf("%c", plainText[i]);
    }
    printf("...\n");
}

// CBC
void CBC() {
    readToMemory();
    cipherText = (char*)malloc(plainSize+1);
    byteCipherText = (uint8_t*)malloc(plainSize/2*sizeof(uint8_t));
    if (!byteCipherText || !cipherText) {
        printf("[-] Failed to allocate memory \n");
        exit(-1);
    }
    cipherText[plainSize] = '\0';
    printf("\n[*] Test launched, generating cipher\n[+] Writing cipher...\n");
    CBC_Encode();
    FILE* fp = fopen(cipherFile, "w");
    if (!fp) {
        printf("[-] Failed to load the specific file\n");
        exit(1);
    }
    fputs(cipherText, fp);
    fclose(fp);
    putchar('\n');

    printf("[*] 10 tests launched.\n");
    time_t c_start, c_end;
    c_start = clock();
    int count = 1;
    for (; count <= 10; count++) {
        printf("[*] No. %d encode and decode\n", count);
        CBC_Encode();
        CBC_Decode();
    }
    c_end = clock();
    printf("\n[*] CBC time cost: %lfms\n", difftime(c_end, c_start));
}

void CBC_Encode() {
    strToByte(plainText, bytePlainText, plainSize);
    int groupNum = plainSize / 2 / 16;
    uint8_t byteCurrentPlain[16];
    uint8_t byteFinal[16];
    char final[33];

    for (int i = 0; i < 32; i++) {
        final[i] = ivText[i];
    }

    for (int i = 0; i < groupNum; i++) {
        for (int j = 0; j < 16; j++) {
            byteCurrentPlain[j] = bytePlainText[i * 16 + j];
        }
        strToByte(final, byteFinal, 32);
        byte_xor(byteCurrentPlain, byteFinal, 16);
        AESEncrypt(byteKeyText, 16, byteCurrentPlain, byteFinal);
        byteToStr(byteFinal, final, 16);
        for (int j = 0; j < 32; j++) {
            cipherText[i * 32 + j] = final[j];
        }
    }
    printf("[+] Cipher text: ");
    for (int i = 0; i < 64; i++) {
        printf("%c", cipherText[i]);
    }
    printf("...\n");
}

void CBC_Decode() {
    strToByte(cipherText, byteCipherText, plainSize);
    int groupNum = plainSize / 2 / 16;
    uint8_t byteCurrentCipher[16];
    uint8_t byteFinal[16];
    char final[33];
    uint8_t IV[16];
    for (int i = 0; i < 16; i++) {
        IV[i] = byteIvText[i];
    }

    for (int i = 0; i < groupNum; i++) {
        for (int j = 0; j < 16; j++) {
            byteCurrentCipher[j] = byteCipherText[i * 16 + j];
        }
        AESDecrypt(byteKeyText, 16, byteCurrentCipher, byteFinal);
        byte_xor(byteFinal, IV, 16);
        byteToStr(byteFinal, final, 16);
        for (int j = 0; j < 16; j++) {
            IV[j] = byteCipherText[i + 16 + j];
        }
        for (int j = 0; j < 32; j++) {
            plainText[i * 32 + j] = final[j];
        }
    }
    printf("[+] Plain text: ");
    for (int i = 0; i < 64; i++) {
        printf("%c", plainText[i]);
    }
    printf("...\n");
}


// CFB
void CFB() {
    readToMemory();
    cipherText = (char*)malloc(plainSize + 1);
    byteCipherText = (uint8_t*)malloc(plainSize / 2 * sizeof(uint8_t));
    if (!cipherText || !byteCipherText) {
        printf("[-] Failed to allocate memory\n");
        exit(-1);
    }
    cipherText[plainSize] = '\0';

    printf("\n[*] Test launched, generating cipher\n[*] Writing cipher...\n");
    CFB_Encode();
    FILE* fp = fopen(cipherFile, "w");
    if (fp == NULL) {
        printf("[-] Failed to load the specific file");
        exit(1);
    }
    fputs(cipherText, fp);
    fclose(fp);
    putchar('\n');


    printf("[*] 10 tests launched.\n");
    time_t c_start, c_end;
    c_start = clock();
    for (int i = 0; i < 10; i++) {
        printf("[*] NO. %d encode and decode\n", i + 1);
        CFB_Encode();
        CFB_Decode();
    }
    c_end = clock();
    printf("\n[*] CFB time cost: %lfms\n", difftime(c_end, c_start));
}


void CFB_Encode() {
    strToByte(plainText, bytePlainText, plainSize);
    int groupNum = plainSize / 2 / 16;
    uint8_t byteFinal[16];
    uint8_t byteZ[16];
    for (int i = 0; i < 16; i++) {
        byteZ[i] = byteIvText[i];
    }
    for (int i = 0; i < groupNum * 16; i++) {
        uint8_t tem;
        uint8_t current;
        AESEncrypt(byteKeyText, 16, byteZ, byteFinal);
        tem = byteFinal[0];
        current = bytePlainText[i];
        tem ^= current;
        Move(byteZ, tem);
        sprintf(cipherText + i * 2, "%.2X", tem);
    }
    printf("[+] Cipher text: ");
    for (int i = 0; i < 64; i++) {
        printf("%c", cipherText[i]);
    }
    printf("...\n");
}

void CFB_Decode() {
    strToByte(cipherText, byteCipherText, plainSize);
    int groupNum = plainSize / 2 / 16;
    uint8_t byteFinal[16];
    uint8_t byteZ[16];
    for (int i = 0; i < 16; i++) {
        byteZ[i] = byteIvText[i];
    }

    for (int i = 0; i < groupNum * 16; i++) {
        uint8_t tem;
        uint8_t current;
        AESEncrypt(byteKeyText, 16, byteZ, byteFinal);
        tem = byteFinal[0];
        current = byteCipherText[i];
        tem ^= current;
        Move(byteZ, current);
        sprintf(plainText + i * 2, "%.2X", tem);
    }
    printf("[+] Plain text: ");
    for (int i = 0; i < 64; i++) {
        printf("%c", plainText[i]);
    }
    printf("...\n");
}


// OFB
void OFB() {
    readToMemory();
    cipherText = (char*)malloc(plainSize + 1);
    byteCipherText = (uint8_t*)malloc(plainSize / 2 * sizeof(uint8_t));
    if (!cipherText || !byteCipherText) {
        printf("[-] Failed to allocate memory\n");
        exit(-1);
    }
    printf("\n[*] Test launched, generating cipher\n[*] Writing cipher...\n");
    OFB_Encode();
    FILE* fp = fopen(cipherFile, "w");
    if (fp == NULL) {
        printf("[-] Failed to load the specific file");
        exit(1);
    }
    fputs(cipherText, fp);
    fclose(fp);
    putchar('\n');


    printf("[*] 10 tests launched.\n");
    time_t c_start, c_end;
    c_start = clock();
    for (int i = 0; i < 10; i++) {
        printf("[*] No. %d encode and decode\n", i + 1);
        OFB_Encode();
        OFB_Decode();
    }
    c_end = clock();
    printf("\n[*] OFB time cost: %lfms\n", difftime(c_end, c_start));
}

void OFB_Encode() {
    strToByte(plainText, bytePlainText, plainSize);
    int groupNum = plainSize / 2 / 16;
    uint8_t byteFinal[16];
    uint8_t byteZ[16];
    for (int i = 0; i < 16; i++) {
        byteZ[i] = byteIvText[i];
    }

    for (int i = 0; i < groupNum * 16; i++) {
        uint8_t tem;
        uint8_t current;
        AESEncrypt(byteKeyText, 16, byteZ, byteFinal);
        tem = byteFinal[0];
        Move(byteZ, tem);
        current = bytePlainText[i];
        tem ^= current;
        sprintf(cipherText + i * 2, "%.2X", tem);
    }
    printf("[+] Cipher text: ");
    for (int i = 0; i < 64; i++) {
        printf("%c", cipherText[i]);
    }
    printf("...\n");
}


void OFB_Decode() {
    strToByte(cipherText, byteCipherText, plainSize);
    int groupNum = plainSize / 2 / 16;
    uint8_t byteFinal[16];
    uint8_t byteZ[16];
    for (int i = 0; i < 16; i++) {
        byteZ[i] = byteIvText[i];
    }

    for (int i = 0; i < groupNum * 16; i++) {
        uint8_t tem;
        uint8_t current;
        AESEncrypt(byteKeyText, 16, byteZ, byteFinal);
        tem = byteFinal[0];
        current = byteCipherText[i];
        Move(byteZ, tem);
        tem ^= current;
        sprintf(plainText + i * 2, "%.2X", tem);
    }
    printf("[+] Plain text: ");
    for (int i = 0; i < 64; i++) {
        printf("%c", plainText[i]);
    }
    printf("...\n");
}
