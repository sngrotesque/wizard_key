#include <fstream>
#include <stdexcept>

#include <iostream>
#include <filesystem>

#include <network/wmkc_net.c>
#include <network/wmkc_ssl.c>
#include <crypto/snc.c>
#include <wmkc_memory.c>
#include <wmkc_struct.c>
#include <wmkc_file.c>
#include <wmkc_misc.c>
#include <wmkc_time.c>

using namespace std;

typedef struct PNG_Chunk_t {
    wmkcByte size[4];
    wmkcByte name[4];
    wmkcByte *data;
    wmkcByte crc[4];
    wmkc_u32 size32;
    struct PNG_Chunk_t *next;
} PNG_Chunk_t;

typedef struct {
    wmkc_u32 width;
    wmkc_u32 height;
    wmkcByte bitDepth;
    wmkcByte colorType;
    wmkcByte compressMethod;
    wmkcByte filterMethod;
    wmkcByte interlaceMethod;
} PNG_IHDR_Chunk_t;

class PNG {
    private:
        wmkcFile_obj *FileStream;
        wmkcErr_obj error;
        wmkcCSTR _path;
        wmkcCSTR _mode;

        wmkcByte chunk_data[4];
        wmkc_u32 chunk_size;

        wmkc_u32 offset()
        {
            wmkc_u32 _offset_n = ftell(this->FileStream->fp);
            if(_offset_n == -1) {
                throw runtime_error("Error in ftell function.");
            }
            return _offset_n;
        }

        wmkcVoid offset(wmkc_u32 offset, wmkc_u32 origin)
        {
            if(fseek(this->FileStream->fp, offset, origin)) {
                throw runtime_error("Error in fseek function.");
            }
        }

        wmkcVoid is_png()
        {
            if(this->offset())
                this->offset(0, SEEK_SET);
            wmkcByte png_header[8];
            fread(png_header, sizeof(wmkcByte), 8, this->FileStream->fp);
            if(memcmp(png_header, "\x89\x50\x4e\x47\x0d\x0a\x1a\x0a", 8)) {
                throw runtime_error("This file is not a PNG image.");
            }
        }

        wmkcVoid read_IHDR()
        {
            if(this->offset() != 8) {
                throw runtime_error("File offset error, expected 8.");
            }
            wmkc_u32 size = 0;

            this->IHDR = new PNG_IHDR_Chunk_t;

            // 获取长度
            fread(this->chunk_data, 1, 4, this->FileStream->fp);
            wmkcStruct_unpack(">I", &size, this->chunk_data);
            // 获取块名
            fread(this->chunk_data, 1, 4, this->FileStream->fp);
            if(memcmp(this->chunk_data, "IHDR", 4)) {
                throw runtime_error("Detected that the current block is not an IHDR block.");
            }
            // 获取宽度与高度
            fread(this->chunk_data, 1, 4, this->FileStream->fp);
            wmkcStruct_unpack(">I", &this->IHDR->width, this->chunk_data);
            fread(this->chunk_data, 1, 4, this->FileStream->fp);
            wmkcStruct_unpack(">I", &this->IHDR->height, this->chunk_data);
            // 获取位深，颜色类型，压缩方法，过滤方法，交错方法
            this->IHDR->bitDepth = fgetc(this->FileStream->fp);
            this->IHDR->colorType = fgetc(this->FileStream->fp);
            this->IHDR->compressMethod = fgetc(this->FileStream->fp);
            this->IHDR->filterMethod = fgetc(this->FileStream->fp);
            this->IHDR->interlaceMethod = fgetc(this->FileStream->fp);
            // 跳过CRC校验
            this->offset(4, SEEK_CUR);
        }

    public:
        PNG_IHDR_Chunk_t *IHDR;
        PNG_Chunk_t *sRGB;
        PNG_Chunk_t *gAMA;
        PNG_Chunk_t *pHYs;
        PNG_Chunk_t *IDAT;

        PNG(wmkcCSTR pngPath, wmkcCSTR openMode)
        : FileStream(), error(), IHDR(), sRGB(), gAMA(), pHYs(), IDAT()
        {
            this->_path = pngPath;
            this->_mode = openMode;
        }

        ~PNG()
        {
            if(this->IHDR) delete this->IHDR;
            if(this->sRGB) {
                delete this->sRGB->data;
                delete this->sRGB;
            }
            if(this->gAMA) {
                delete this->gAMA->data;
                delete this->gAMA;
            }
            if(this->pHYs) {
                delete this->pHYs->data;
                delete this->pHYs;
            }
            if(this->IDAT) {
                delete this->IDAT->data;
                delete this->IDAT;
            }
            this->IHDR = wmkcNull;
            this->sRGB = wmkcNull;
            this->gAMA = wmkcNull;
            this->pHYs = wmkcNull;
            this->IDAT = wmkcNull;
        }

        wmkcVoid open()
        {
            this->error = wmkcFile_open(&this->FileStream, this->_path, this->_mode);
            if(this->error.code) {
                throw runtime_error(this->error.func + string(": ") + this->error.message);
            }
        }

        wmkcVoid close()
        {
            this->error = wmkcFile_close(&this->FileStream);
            if(this->error.code) {
                throw runtime_error(this->error.func + string(": ") + this->error.message);
            }
        }

        wmkcVoid read_chunk()
        {
            PNG_Chunk_t *p = new PNG_Chunk_t;

            // 获取块长度
            fread(p->size, 1, 4, this->FileStream->fp);
            wmkcStruct_unpack(">I", &p->size32, p->size);
            // 获取块名称
            fread(p->name, 1, 4, this->FileStream->fp);
            // 获取块内容
            p->data = new wmkcByte[p->size32];
            fread(p->data, 1, p->size32, this->FileStream->fp);
            // 获取块CRC
            fread(p->crc, 1, 4, this->FileStream->fp);

            if(!memcmp(p->name, "sRGB", 4)) {
                this->sRGB = p;
            } else if(!memcmp(p->name, "gAMA", 4)) {
                this->gAMA = p;
            } else if(!memcmp(p->name, "pHYs", 4)) {
                this->pHYs = p;
            } else if(!memcmp(p->name, "IDAT", 4)) {
                this->IDAT = p;
            }
        }

        wmkcVoid read()
        {
            this->is_png();
            this->read_IHDR();
            this->read_chunk();
        }
};

void print(PNG *png_obj)
{
    PNG_Chunk_t *p = wmkcNull;
    if(png_obj->sRGB) {
        p = png_obj->sRGB;
    } else if(png_obj->pHYs) {
        p = png_obj->pHYs;
    } else if(png_obj->IDAT) {
        p = png_obj->IDAT;
    } else if(png_obj->gAMA) {
        p = png_obj->gAMA;
    } else {
        cout << "Error, not matching a supported block name." << endl;
        exit(EOF);
    }

    printf("Chunk size: %u\n", p->size32);
    printf("Chunk name: "); wmkcMisc_PRINT(p->name, 4, 5, 1, 0);
    printf("Chunk data: "); wmkcMisc_PRINT(p->data, p->size32, p->size32+1, 1, 0);
    printf("Chunk CRC:  "); wmkcMisc_PRINT(p->crc, 4, 5, 1, 0);
}

void test()
{
    PNG *png_obj = wmkcNull;
    try {
        png_obj = new PNG("p:/QQ/QQ_Images/45732748e031b6557ab3960aadcbf906.png", "rb");
    } catch(bad_alloc &e) {
        cout << "Failed to create PNG object." << endl;
        return;
    }

    try {
        png_obj->open();
        png_obj->read();
    } catch(exception &e) {
        cout << e.what() << endl;
        return;
    }
    printf("PNG IHDR Width:     %u\n",   png_obj->IHDR->width);
    printf("PNG IHDR Height:    %u\n",   png_obj->IHDR->height);
    printf("PNG IHDR BitDepth:  %02x\n", png_obj->IHDR->bitDepth);
    printf("PNG IHDR ColorType: %02x\n", png_obj->IHDR->colorType);
    printf("PNG IHDR Compress:  %02x\n", png_obj->IHDR->compressMethod);
    printf("PNG IHDR Filter:    %02x\n", png_obj->IHDR->filterMethod);
    printf("PNG IHDR Interlace: %02x\n", png_obj->IHDR->interlaceMethod);

    print(png_obj);

    png_obj->read_chunk();
    print(png_obj);

    delete png_obj;
}

int main()
{
    
    return 0;
}
