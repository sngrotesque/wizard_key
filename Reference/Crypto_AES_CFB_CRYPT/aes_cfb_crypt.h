#include "aes.h"

# ifdef  __cplusplus
extern "C" {
# endif

# define AES_IV_SIZE 16

# define ENC_IV_ENCRYPT     1
# define ENC_IV_DECRYPT     2
# define REUSE_IV_ENCRYPT     3
# define REUSE_IV_DECRYPT     4

int randomIV(unsigned char *out);
int randomIv(unsigned char *out);
int random_iv(unsigned char *out);
int sha384_key_iv(char * in, size_t length, unsigned char * key, unsigned char * iv);
int aes_256_cfb_crypt(const unsigned char *in, unsigned char *out, size_t length, char *strPwd, unsigned char *crypt_iv, int *num, int enc);

# ifdef  __cplusplus
}
# endif
