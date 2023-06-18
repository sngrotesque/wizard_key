#include <wmkc_coder.h>

WMKC_PUBLIC(wmkcErr_obj) wmkcCoder_convert WMKC_OPEN_API
WMKC_OF((wmkcChar **dst, wmkcChar *src, wmkcChar *code_string))
{
    wmkcErr_obj error;
    wmkcChar *src_ptr = src;
    wmkcChar *dst_ptr = wmkcNull;
    wmkcChar *tocode = wmkcNull;
    wmkcChar *fromcode = wmkcNull;
    wmkcSize src_size = strlen(src);
    wmkcSize dst_size = src_size << 1;

    error = wmkc_split(&tocode, &fromcode, code_string, '<');
    if(error.code) return error;

    if(!wmkcMemoryNew(wmkcChar *, (*dst), dst_size)) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcCoder_convert: Failed to allocate memory for (*dst).");
    }
    dst_ptr = (*dst);
    wmkcMemoryZero(dst_ptr, dst_size);

    iconv_t cd = iconv_open(tocode, fromcode);
    if(cd == (iconv_t)-1) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcCoder_convert: iconv_open failed.");
    }

    wmkcSize rc = iconv(cd, &src_ptr, &src_size, &dst_ptr, &dst_size);
    if(rc == -1) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcCoder_convert: iconv failed.");
    }

    iconv_close(cd);
    wmkcMemoryFree(tocode);
    wmkcMemoryFree(fromcode);

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
