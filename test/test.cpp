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

typedef struct {
    wmkcByte size[4];
    wmkcByte name[4];
    wmkcByte *data;
    wmkcByte crc[4];
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
        }

    public:
        PNG_IHDR_Chunk_t *IHDR;

        PNG(wmkcCSTR pngPath, wmkcCSTR openMode)
        : FileStream(), error(), IHDR()
        {
            this->_path = pngPath;
            this->_mode = openMode;
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

        wmkcVoid read()
        {
            this->is_png();
            this->read_IHDR();
        }
};

int main()
{
    PNG *png_obj = wmkcNull;
    try {
        png_obj = new PNG("p:/VisualStudio2022_profile_1560x1560.png", "rb");
    } catch(bad_alloc &e) {
        cout << "Failed to create PNG object." << endl;
        return EOF;
    }

    try {
        png_obj->open();
        png_obj->read();
    } catch(exception &e) {
        cout << e.what() << endl;
        return EOF;
    }

    cout << "IHDR width:      "  << dec << png_obj->IHDR->width << endl;
    cout <<  "IHDR height:     " << dec << png_obj->IHDR->height << endl;
    cout << setfill('0') << hex << endl;
    cout <<  "IHDR Bit Depth:  " << setw(2) << (int)png_obj->IHDR->bitDepth << endl;
    cout <<  "IHDR Color Type: " << setw(2) << (int)png_obj->IHDR->colorType << endl;
    cout <<  "IHDR Compress:   " << setw(2) << (int)png_obj->IHDR->compressMethod << endl;
    cout <<  "IHDR Filter:     " << setw(2) << (int)png_obj->IHDR->filterMethod << endl;
    cout <<  "IHDR Interlace:  " << setw(2) << (int)png_obj->IHDR->interlaceMethod << endl;

    if(png_obj->IHDR)
        delete png_obj->IHDR;
    delete png_obj;
    return 0;
}
