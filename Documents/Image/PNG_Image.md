# PNG

块的数据结构: [`块长度`][`块名称`][`块内容`][`块CRC`]

- 块长度
> 用4字节表示，不包括块名称和CRC32值，范围为[0, 2^32-1]
- 块名称
> 用4字节表示，必要的有IDAT，IHDR，IEND。
- 块内容
> 通常不固定长度，块长度必须与此内容的长度一致。
- 块CRC
> 用4字节表示，由zlib库提供的CRC32校验。  
> 用于校验[块名称][块内容]的CRC32值。

### 文件部分数据解析
```text
89 50 4E 47 0D 0A 1A 0A                代表此文件头部，表明此为PNG图像文件。
00 00 00 0D                            代表当前块的长度。
49 48 44 52                            代表此块的类型(IHDR)。
00 00 03 20                            代表此图像的宽度。
00 00 01 90                            代表此图像的高度。
08                                     代表此图像的位深。
02                                     代表此图像的颜色类型。
00                                     压缩方法，LZ77派生算法（目前仅定义了0）。
00                                     滤波器方法，（目前仅定义了0）。
00                                     隔行扫描方法，0为非隔行扫描，1为Adam7隔行扫描方法。
D9 47 93 63                            代表CRC校验值。
00 00 00 00 49 45 4E 44 AE 42 60 82    代表文件末尾，表示此文件已结束。
```

| Color Type | Allowed Bit depths | Interpretation                                                 |
| --         | --                 | --                                                             |
| 0          | 1,2,4,8,16         | 每个像素都是一个灰度样本。                                   |
| 2          | 8,16               | 每个像素都是R、G、B三元组。                                  |
| 3          | 1,2,4,8            | 每个像素都是一个调色板索引；PLTE区块必须出现。              |
| 4          | 8,16               | 每个像素都是一个灰度样本，后面跟着一个alpha样本。           |
| 6          | 8,16               | 每个像素是一个R、G、B三元组，后面跟着一个alpha样本。        |

| Type   | Necessity | Interpretation |
| -      | -         | -                                           |
| IHDR   | 1         | 文件头数据块                               |
| PLTE   | 0         | 调色板块（并未实际在PNG图像中找到过此块）|
| IDAT   | 1         | 图像数据块                                 |
| IEND   | 1         | 图像结束块                                 |
| bKGD   | 0         | 背景颜色数据块                             |
| cHRM   | 0         | 基色和白色度数据块                         |
| gAMA   | 0         | 图像γ数据块                                |
| hIST   | 0         | 图像直方图数据块                           |
| iCCP   | 0         | 嵌入式ICC配置文件块                        |
| iTXt   | 0         | 国际文本数据块                             |
| pHYs   | 0         | 物理像素尺寸数据块                         |
| sBIT   | 0         | 样本有效位数据块                           |
| sRGB   | 0         | 颜色空间块                                 |
| sPLT   | 0         | 建议调色板块                               |
| tEXt   | 0         | 文本信息数据块                             |
| tIME   | 0         | 图像最后修改时间数据块                     |
| tRNS   | 0         | 图像透明数据块                             |
| zTXt   | 0         | 压缩文本数据块                             |

### IDAT

IDAT内的数据经过`zlib`库的压缩，函数为`compress`。
位深为`08`时，颜色类型`06`有四个通道，是`RGBA`，每个通道占1字节总共32位。
位深为`08`时，颜色类型`02`有三个通道，是`RGB`，每个通道占1字节总共24位。

PNG图像支持5种不同类型的过滤器，分别为: 

- 过滤器类型`0`: None过滤器。这种过滤器不对像素数据进行任何处理。
- 过滤器类型`1`: Sub过滤器。这种过滤器将每个像素值减去其左侧像素的值。
- 过滤器类型`2`: Up过滤器。这种过滤器将每个像素值减去其上方像素的值。
- 过滤器类型`3`: Average过滤器。这种过滤器将每个像素值减去其左侧像素和上方像素的平均值。
- 过滤器类型`4`: Paeth过滤器。这种过滤器使用Paeth预测算法来预测每个像素的值，并将实际像素值减去预测值。

每个扫描行的第一个字节是过滤器类型字节，它指示了该扫描行使用的过滤器类型。
例如，如果过滤器类型字节的值为`0`，则表示该扫描行使用了`None过滤器`；
如果过滤器类型字节的值为`1`，则表示该扫描行使用了`Sub过滤器`，以此类推。

在解压缩后，您需要对每个扫描行应用相应的逆过滤器，以恢复原始的像素数据，逆过滤器的具体实现方式取决于所使用的过滤器类型。

在PNG图像中，每个扫描行的第一个字节是过滤器类型字节，它指示了该扫描行使用的过滤器类型。
在解压缩后，您需要对每个扫描行应用相应的逆过滤器，以恢复原始的像素数据。

##### 以下为代码示例

逆过滤器的具体实现方式取决于所使用的过滤器类型。
下面是一个简单的示例代码，它展示了如何根据过滤器类型应用逆过滤器：
```python
def unfilter_scanline(filter_type, scanline, prev_scanline, bpp):
    def paeth_predictor(a, b, c):
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

    result = bytearray(scanline)
    if filter_type == 0:
        pass
    elif filter_type == 1:
        for i in range(bpp, len(scanline)):
            result[i] = (result[i] + result[i-bpp]) & 0xff
    elif filter_type == 2:
        for i in range(len(scanline)):
            result[i] = (result[i] + prev_scanline[i]) & 0xff
    elif filter_type == 3:
        for i in range(len(scanline)):
            left = result[i-bpp] if i >= bpp else 0
            result[i] = (result[i] + (left + prev_scanline[i]) // 2) & 0xff
    elif filter_type == 4:
        for i in range(len(scanline)):
            left = result[i-bpp] if i >= bpp else 0
            upleft = prev_scanline[i-bpp] if i >= bpp else 0
            result[i] = (result[i] + paeth_predictor(left, prev_scanline[i], upleft)) & 0xff
    return result
```
上面的代码定义了一个名为`unfilter_scanline`的函数，它接受4个参数：
1. `filter_type`表示过滤器类型；
2. `scanline`表示当前扫描行的数据；
3. `prev_scanline`表示上一个扫描行的数据；
4. `bpp`表示每个像素占用的字节数。

函数内部使用了一个名为`paeth_predictor`的辅助函数，它实现了Paeth预测算法。
在主函数中，我们根据过滤器类型的不同，使用不同的方法来应用逆过滤器。
最后，函数返回应用逆过滤器后得到的扫描行数据。您可以使用这些数据来恢复PNG图像中的原始像素数据。

如果您要处理一个使用过滤器类型1的扫描行，可以这样调用函数：
```python
result = unfilter_scanline(1, scanline, prev_scanline, bpp)
```
其中，`scanline`和`prev_scanline`分别表示当前扫描行和上一个扫描行的数据，`bpp`表示每个像素占用的字节数。
函数返回应用逆过滤器后得到的扫描行数据。
请注意，要正确使用这个函数，您需要确保提供的参数是正确的。
例如，您需要正确计算每个像素占用的字节数，并且在处理第一个扫描行时，需要将`prev_scanline`设置为全0字节数组。

下面是一个简单的示例代码，它展示了如何使用`unfilter_scanline`函数来应用逆过滤器：
```python
import struct
import zlib

def unfilter_scanline(filter_type, scanline, prev_scanline, bpp):
    def paeth_predictor(a, b, c):
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

    result = bytearray(scanline)
    if filter_type == 0:
        pass
    elif filter_type == 1:
        for i in range(bpp, len(scanline)):
            result[i] = (result[i] + result[i-bpp]) & 0xff
    elif filter_type == 2:
        for i in range(len(scanline)):
            result[i] = (result[i] + prev_scanline[i]) & 0xff
    elif filter_type == 3:
        for i in range(len(scanline)):
            left = result[i-bpp] if i >= bpp else 0
            result[i] = (result[i] + (left + prev_scanline[i]) // 2) & 0xff
    elif filter_type == 4:
        for i in range(len(scanline)):
            left = result[i-bpp] if i >= bpp else 0
            upleft = prev_scanline[i-bpp] if i >= bpp else 0
            result[i] = (result[i] + paeth_predictor(left, prev_scanline[i], upleft)) & 0xff
    return result

def get_png_idat_chunks(filename):
    with open(filename, 'rb') as f:
        data = f.read()
    pos = data.index(b'IHDR') + 4
    width, height = struct.unpack('!II', data[pos+4:pos+12])
    pos += 17
    idat_chunks = []
    while pos < len(data):
        chunk_length = struct.unpack('!I', data[pos:pos+4])[0]
        pos += 4
        chunk_type = data[pos:pos+4].decode('ascii')
        pos += 4
        chunk_data = data[pos:pos+chunk_length]
        pos += chunk_length
        crc = struct.unpack('!I', data[pos:pos+4])[0]
        pos += 4
        if chunk_type == 'IDAT':
            idat_chunks.append(chunk_data)
    return width, height, b''.join(idat_chunks)

def get_png_pixels(filename):
    width, height, idat_data = get_png_idat_chunks(filename)
    bpp = 4 # assuming the image is RGBA
    decompressed_data = zlib.decompress(idat_data)
    pixels = []
    pos = 0
    prev_scanline = bytearray([0] * (width * bpp))
    for y in range(height):
        filter_type = decompressed_data[pos]
        pos += 1
        scanline = decompressed_data[pos:pos+width*bpp]
        pos += width * bpp
        unfiltered_scanline = unfilter_scanline(filter_type, scanline, prev_scanline, bpp)
        pixels.append(unfiltered_scanline)
        prev_scanline = unfiltered_scanline
    return pixels

pixels = get_png_pixels('image.png')
for y in range(len(pixels)):
    for x in range(0, len(pixels[y]), 4):
        r, g, b, a = pixels[y][x:x+4]
        print(f'Pixel ({x//4}, {y}): ({r}, {g}, {b}, {a})')
```
上面的代码定义了两个函数：`get_png_idat_chunks`和`get_png_pixels`。
`get_png_idat_chunks`函数用于从PNG文件中提取IDAT块数据；`get_png_pixels`函数用于从PNG文件中提取像素数据。

在主程序中，我们调用`get_png_pixels`函数来提取PNG图像中的像素数据。
这个函数首先调用`get_png_idat_chunks`函数来提取IDAT块数据，然后使用`zlib.decompress`函数将IDAT块数据解压缩。
接下来，函数遍历每个扫描行，使用`unfilter_scanline`函数应用逆过滤器，以恢复原始的像素数据。
最后，函数返回一个包含所有像素数据的列表。在主程序中，我们遍历这个列表，打印出每个像素的坐标和颜色值。
请注意，上面的代码仅作为示例，它假定PNG图像使用RGBA颜色格式，并且未使用隔行扫描。

