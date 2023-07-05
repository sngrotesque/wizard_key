import struct
import io

def parse_jpeg(file):
    with open(file, 'rb') as f:
        jpeg = io.BytesIO(f.read())

    try:
        # 检查 SOI 标记
        if jpeg.read(2) != b'\xff\xd8':
            raise Exception('Not a JPEG file')

        while True:
            # 查找下一个标记
            byte = jpeg.read(1)
            while byte != b'\xff':
                byte = jpeg.read(1)

            # 跳过填充字节
            while byte == b'\xff':
                byte = jpeg.read(1)

            # 检查是否为 SOF0 标记
            if 0xc0 <= byte[0] <= 0xcf:
                # 跳过段长度和采样精度
                jpeg.read(3)
                # 读取图像高度和宽度
                height, width = struct.unpack('>HH', jpeg.read(4))
                return (width, height)

            # 跳过其他段
            else:
                length = struct.unpack('>H', jpeg.read(2))[0]
                jpeg.read(length - 2)
    finally:
        jpeg.close()

# 测试函数
file = 'p:/bilibili_2233.jpeg'
width, height = parse_jpeg(file)
print(f'Image dimensions: {width} x {height}')
