#include <wmkc_png.h>

WMKC_PRIVATE(wmkcVoid) wmkcPNG_match
WMKC_OF((wmkcPNG_chunk *chunk, wmkcPNG_type type))
{
    switch(type) {
        case IDAT:
        case tEXt:
        case sRGB:
        case pHYs:
        case iTXt:
        case iCCP:
        case cHRM:
        case gAMA:
        case sBIT:
        case hIST:
        case sPLT:
        case bKGD:
        case tIME:
        case tRNS:
        case zTXt:
            break;
    }
}

WMKC_PUBLIC(wmkcErr_obj) wmkcPNG_new WMKC_OPEN_API
WMKC_OF((wmkcPNG_obj **obj))
{
    wmkcErr_obj error;
    if(!obj) {
        wmkcErr_return(error, wmkcErr_OK, "wmkcPNG_new", "obj is NULL.");
    }

    if(!wmkcMem_new(wmkcPNG_obj *, (*obj), sizeof(wmkcPNG_obj))) {
        wmkcErr_return(error, wmkcErr_OK, "wmkcPNG_new",
            "Failed to allocate memory for (*obj).");
    }
    wmkcMem_zero((*obj), sizeof(wmkcPNG_obj));

    wmkcErr_return(error, wmkcErr_OK, "wmkcPNG_new", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcPNG_free WMKC_OPEN_API
WMKC_OF((wmkcPNG_obj **obj))
{
    wmkcErr_obj error;
    if(!obj || !(*obj)) {
        wmkcErr_return(error, wmkcErr_OK, "wmkcPNG_free", "obj is NULL.");
    }

    wmkcPNG_chunk *p = wmkcNull;
    wmkcPNG_chunk *q = wmkcNull;
    if((*obj)->chunk) {
        p = (*obj)->chunk;
        while(p) {
            q = p->next;
            wmkcMem_free(p->data);
            wmkcMem_free(p);
            p = q;
        }
    }
    wmkcMem_free((*obj));

    wmkcErr_return(error, wmkcErr_OK, "wmkcPNG_free", "OK.");
}

WMKC_PUBLIC(wmkcErr_obj) wmkcPNG_read WMKC_OPEN_API
WMKC_OF((wmkcPNG_obj *obj, wmkcPNG_type type, wmkcCSTR path))
{
    wmkcErr_obj error;
    if(!obj || !path) {
        wmkcErr_return(error, wmkcErr_OK, "wmkcPNG_read", "obj or path is NULL.");
    }
    wmkcFile_obj *file = wmkcNull;
    wmkcByte header[8];
    wmkcByte buffer[4];

    if((error = wmkcFile_open(&file, path, "rb")).code)
        return error;

    // 检查文件头是否是PNG图像
    fread(header, 1, sizeof(header), file->fp);
    if(memcmp(WMKC_PNG_HEAD, header, sizeof(header))) {
        wmkcErr_return(error, wmkcErr_Err32, "wmkcPNG_read", "This file is not a PNG image.");
    }
    // 让文件指针跳过IHDR块长度与类型
    fseek(file->fp, 8, SEEK_CUR);
    // 读取图像宽度
    fread(buffer, 1, sizeof(buffer), file->fp);
    wmkcStruct_unpack(">I", &obj->width, buffer);
    // 读取图像高度
    fread(buffer, 1, sizeof(buffer), file->fp);
    wmkcStruct_unpack(">I", &obj->height, buffer);
    // 获取位深度与颜色类型
    obj->bitDepth = fgetc(file->fp);
    obj->colorType = fgetc(file->fp);
    // 获取压缩方法与过滤方法与交错方法
    obj->compressMethod = fgetc(file->fp);
    obj->filterMethod = fgetc(file->fp);
    obj->interlaceMethod = fgetc(file->fp);

    wmkcFile_close(&file);
    wmkcErr_return(error, wmkcErr_OK, "wmkcPNG_read", "OK.");
}
