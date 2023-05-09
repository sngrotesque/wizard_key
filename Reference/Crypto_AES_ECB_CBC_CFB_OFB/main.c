#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "./src/AES.h"
#include "./src/utils.h"
#include "./src/mode.h"


int main() {
    printf("[*] Generating 5M data of random plain text\n[+] Writing plain text...");
    FILE* fp = fopen("./file/AES_plain.txt", "w");
    if (fp == NULL) {
        printf("[-] Failed to load the specific file");
        exit(1);
    }
    for (int i = 0; i < 1024 * 5; i++) {
        for (int k = 0; k < 1024; k++) {
            char num = to_ascii(rand() % 16);
            if (num >= 10) {
                num = to_ascii(num - 10 + 'A');
            }
            else {
                num = to_ascii(num + '0');
            }
            fprintf(fp, "%c", num);
        }
    }
    fclose(fp);
    putchar('\n');

    int i;
    while (true) {
        printf("[+] Enter command: ");
        gets(command);
        tokenNumber = commandTokens();
        for (i = 1; i < tokenNumber; i++) {
            switch (argc[i][0]) {
                case 'p': {
                    strcpy(plainFile, &argc[i][2]);
                    break;
                }
                case 'k': {
                    strcpy(keyFile, &argc[i][2]);
                    break;
                }
                case 'v': {
                    strcpy(ivFile, &argc[i][2]);
                    break;
                }
                case 'm': {
                    strcpy(operationMode, &argc[i][2]);
                    operationMode[3] = '\0';
                    break;
                }
                case 'c': {
                    strcpy(cipherFile, &argc[i][2]);
                    break;
                }
                default:
                    break;
            }
        }

        strupr(operationMode);
        printf("[+] Mode: %s \n", operationMode);
        if (strcmp(operationMode, "ECB") == 0) {
            ECB();
        }
        else if (strcmp(operationMode, "CBC") == 0) {
            CBC();
        }
        else if (strcmp(operationMode, "CFB") == 0) {
            CFB();
        }
        else if (strcmp(operationMode, "OFB") == 0) {
            OFB();
        }
        else {
            puts("[-] Wrong mode(only ECB, CBC, CFB, OFB available)");
            continue;
        }
        if (_kbhit() && _getch() == 0x1b) {
            break;
        }
    }
    return 0;
}

