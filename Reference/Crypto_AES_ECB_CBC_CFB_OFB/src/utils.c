#include "./utils.h"


void move(bool raw[64], const bool in[]) {
    for(int i=0; i<64; i++) {
        if(i<56) {
            raw[i] = raw[i+8];
        } else {
            raw[i] = in[i-56];
        }
    }
}


int commandTokens() {
    char* token;
    int i = 0;
    token = strtok(command, "-");
    strcpy(argc[i++], token);
    while(token!=NULL){
        token = strtok(NULL, "-");
        if(token!=NULL){
            strcpy(argc[i++], token);
        }
    }
    return i;   // Return the number of fields
}


int hexToDec(char ch) {
    if(ch>='A') {
        return ch - 'A' +10;
    } else {
        return ch - '0';
    }
}


void strToByte(char* plain_text, uint8_t *byte_plain_text, int len) {
    for(int i=0; i<len/2; i++) {
        int cur = 2 * i;
        byte_plain_text[i] = hexToDec(plain_text[cur]) * 16 + hexToDec(plain_text[cur + 1]);
    }
}


void byteToStr(uint8_t *byte_plain_text, char* plain_text, int len) {
    for(int i=0; i<len; i++) {
        int cur = 2 * i;
        sprintf(plain_text+cur, "%.2X", byte_plain_text[i]);
    }
}


void hexToBin(char* string, bool* binString, long int num) {
    strupr(string);
    int m, n;
    char tmp;
    // Hex-to-binary table
    // Notice "5", one bit left for '\0'
    char myTable[16][5] = {
            "0000", "0001", "0010", "0011",
            "0100", "0101", "0110", "0111",
            "1000", "1001", "1010", "1011",
            "1100", "1101", "1110", "1111"
    };
    for(m=0; m<num; m++) {
        if(string[m]>='A') {
            tmp = to_ascii(string[m]-'A'+10);
        } else {
            tmp = to_ascii(string[m]-'0');
        }
        for(n=0; n<4; n++) {
            // Write binary value into binary string
            binString[m*4+n] = myTable[tmp][n] - '0';
        }
    }
}


void readToMemory() {
    for(int i=1; i<tokenNumber; i++){
        printf("[+] Command No. %d: %s\n", i, argc[i]);
        // Read file according to current command field
        switch(argc[i][0]){
            // Read plain file
            case 'p': {
                printf("[+] Plain file: %s \n", plainFile);
                FILE *fp = fopen(plainFile, "r");
                // Failure
                if (fp == NULL) {
                    printf("[-] Failed to load the specific file");
                    exit(1);
                }
                fseek(fp, 0, SEEK_END);
                // How many bytes the file takes
                int size = ftell(fp);
                // Redirect pointer to beginning
                fseek(fp, 0, SEEK_SET);
                // Allocate memory
                plainText = (char *) malloc(size + 1);
                // Allocate memory(for the binary)
                bytePlainText = (uint8_t *) malloc((size / 2) * sizeof(uint8_t));
                if (!plainText || !bytePlainText) {
                    printf("[+] Failed to allocate memory\n");
                    exit(1);
                }
                plainText[size] = '\0';
                plainSize = size;
                // Read bytes of 'size' into memory
                fread(plainText, size, 1, fp);
                fclose(fp);
                // printf("[+] Plain Text: %s \n", plainText);
                printf("[+] Plain text: ");
                for (int j = 0; j < 16; j++) {
                    printf("%c", plainText[j]);
                }
                printf(" ...\n");
                break;
            }

            case 'k': {
                printf("[+] Key file: %s\n", keyFile);
                FILE* fp = fopen(keyFile, "r");
                // Failure
                if(fp==NULL){
                    printf("[-] Failed to load the specific file");
                    exit(1);
                }
                fseek(fp, 0, SEEK_END);
                // How many bytes the file takes
                int size = ftell(fp);
                // Redirect pointer to beginning
                fseek(fp, 0, SEEK_SET);
                // Allocate memory
                keyText = (char*)malloc(size+1);
                // Allocate memory(for the binary)
                byteKeyText = (uint8_t*)malloc((size/2)*sizeof(uint8_t));
                if (!keyText || !byteKeyText) {
                    printf("[+] Failed to allocate memory\n");
                    exit(1);
                }
                keyText[size] = '\0';
                fread(keyText, size, 1, fp);
                fclose(fp);
                printf("[+] Key text: %s\n", keyText);
                strToByte(keyText, byteKeyText, size);
                break;
            }

            case 'v': {
                printf("[+] Initial vector file: %s\n", ivFile);
                FILE* fp = fopen(ivFile, "r");
                if(fp==NULL){
                    printf("[-] Failed to load the specific file");
                    exit(1);
                }
                fseek(fp, 0, SEEK_END);
                int size = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                ivText = (char*)malloc(size+1);
                byteIvText = (uint8_t*)malloc((size/2)*sizeof(uint8_t));
                if(!ivText || !byteIvText) {
                    printf("[+] Failed to allocate memory\n");
                    exit(1);
                }
                ivText[size] = '\0';
                fread(ivText, size, 1, fp);
                fclose(fp);
                printf("[+] Initial vector text: %s \n", ivText);
                strToByte(ivText, byteIvText, size);
                break;
            }

            case 'm': {
                printf("[+] Mode: %s\n", operationMode);
                break;
            }

            case 'c': {
                printf("[+] Cipher file: %s \n", cipherFile);
                break;
            }

            default: break;
        }
    }
}


void XOR(bool* DatA, const bool* DatB) {
    for(int i=0; i<64; i++) {
        DatA[i] = DatA[i] ^ DatB[i];
    }
}


void XOR_8(bool* DatA, const bool* DatB) {
    for(int i=0; i<8; i++) {
        DatA[i] = DatA[i] ^ DatB[i];
    }
}
