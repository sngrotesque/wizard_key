import struct
import zlib

class wmkcPng:
    def __init__(self, path :str):
        self.path      = path

        self.width     = None
        self.height    = None
        self.bpp       = None # bytes per pixel（每个像素字节数）
        self.bitDepth  = None # 位深度
        self.colorType = None # 颜色类型

        self.data      = None

    def read_image(self):
        with open(self.path, 'rb') as f:
            f.read(16) # 跳过文件头，IHDR块长度，IHDR块名称
            self.width  = struct.unpack('>I', f.read(4))[0] # 宽度
            self.height = struct.unpack('>I', f.read(4))[0] # 高度
            self.bitDepth = ord(f.read(1))
            self.colorType = ord(f.read(1)) # 颜色类型
            self.data = f.read()

        if self.colorType == 2:
            self.bpp = 3
        elif self.colorType == 6:
            self.bpp = 4

    def extract_rows(self, pixels :bytes, width :int, height :int):
        rows, i = [], 0
        self.bpp = 3 if self.colorType == 2 else 4
        rowSize = width * self.bpp
        for _ in range(height):
            filterType = pixels[i]
            i += 1
            rows.append((filterType, pixels[i:i+rowSize]))
            i += rowSize
        return rows

    def unfilter_scanline(self, filter :int, prevRow :bytes, row :bytes, bpp :int):
        def paeth_predictor(a :int, b :int, c :int):
            p = a + b - c
            pa = abs(p - a)
            pb = abs(p - b)
            pc = abs(p - c)
            if pa <= pb and pa <= pc:
                return a
            elif pb <= pc:
                return b
            else:
                return c

        result = bytearray(row)
        if filter == 0:
            return row
        elif filter == 1:
            for i in range(bpp, len(row)):
                result[i] = (result[i] + result[i-bpp]) & 0xff
        elif filter == 2:
            for i in range(len(row)):
                result[i] = (result[i] + prevRow[i]) & 0xff
        elif filter == 3:
            for i in range(len(row)):
                left = result[i-bpp] if i >= bpp else 0
                result[i] = (result[i] + (left + prevRow[i]) // 2) & 0xff
        elif filter == 4:
            for i in range(len(row)):
                left = result[i-bpp] if i >= bpp else 0
                upleft = prevRow[i-bpp] if i >= bpp else 0
                result[i] = (result[i] + paeth_predictor(left, prevRow[i], upleft)) & 0xff
        return bytes(result)

    def png_idat_chunks(self, return_rows :bool = False):
        idat_index = [x for x in range(len(self.data)) if self.data[x:x+4] == b'IDAT']

        idat = [self.data[idat_index[x]+4:idat_index[x]+4+struct.unpack('>I',
            self.data[idat_index[x]-4 : idat_index[x]])[0]] for x in range(len(idat_index))]

        idat = zlib.decompress(b''.join(idat))
        if return_rows:
            return self.extract_rows(idat, self.width, self.height)
        return idat

    def analyzeOverallPixels(self):
        self.read_image()
        IDAT_Pixels = self.png_idat_chunks(1)
        pixels = []

        for i in range(len(IDAT_Pixels)):
            pixel_index = 0
            filterType = IDAT_Pixels[i][0]
            prev_line = IDAT_Pixels[abs(i-1)][1] if not i else IDAT_Origin_Pixels
            line = IDAT_Pixels[i][1]

            IDAT_Origin_Pixels = self.unfilter_scanline(filterType, prev_line, line, self.bpp)
            for _ in range(len(IDAT_Origin_Pixels) // self.bpp):
                rgb = int(IDAT_Origin_Pixels[pixel_index : pixel_index+self.bpp].hex(), 16)
                if self.bpp == 3:
                    pixels.append((rgb >> 16, (rgb >> 8) & 0xff, rgb & 0xff))
                elif self.bpp == 4:
                    pixels.append((rgb >> 24, (rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff))
                pixel_index += self.bpp
        return pixels
