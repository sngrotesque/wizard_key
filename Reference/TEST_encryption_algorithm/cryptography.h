#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	char *str;
	uint32_t str_n;
	char *res;
} sn_crypto;

static int fence_code(sn_crypto *data)
{
	data->res = (char *)calloc(data->str_n + 1, 1);
	uint32_t x, y, t, b, r, pwd_n = data->str_n / 2;
	switch(data->str_n % 2) {
		case 0:
			for(x = 0, y = 1, t = 0, b = pwd_n; x < data->str_n; x+=2, y+=2, t++, b++) {
				data->res[t] = data->str[x], data->res[b] = data->str[y];
			}
			break;
		case 1:
			for(x = 0, y = 1, t = 0, b = pwd_n + 1; x < data->str_n; x+=2, y+=2, t++, b++) {
				data->res[t] = data->str[x], data->res[b] = data->str[y];
			}
			break;
	}
	return 0;
}

