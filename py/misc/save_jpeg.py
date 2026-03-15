from PIL import Image

def save_low_quality_jpeg(input_path, output_path, quality=30):
    """
    读取图像并保存为低质量JPEG
    :param input_path: 输入图像路径
    :param output_path: 输出JPEG路径
    :param quality: JPEG质量（1-95，数值越低压缩越大）
    """
    img = Image.open(input_path)
    # 转换为RGB，避免某些格式（如PNG带透明通道）保存JPEG时报错
    img = img.convert("RGB")
    img.save(output_path, "JPEG", quality=quality, optimize=True)

if __name__ == "__main__":
    # 示例调用
    save_low_quality_jpeg("./shuffled_image.jpg", "./shuffled_image.jpg", quality = 5)
