from flask import Flask, render_template, request, send_file
import numpy as np
from PIL import Image
import zlib
import io

app = Flask(__name__)

def _seed_from_key(key: bytes) -> int:
    # 用密钥生成确定性随机种子
    return zlib.crc32(key)

def restore_by_rows(shuffled_img: Image.Image, number_rows: int = 3, key: bytes = b'keykeykey') -> Image.Image:
    """
    将按每 number_rows 行打乱后的图像恢复为原图；不足 number_rows 的剩余行保持在末尾不变。
    """
    if number_rows <= 0:
        raise ValueError("number_rows 必须为正整数")

    arr = np.asarray(shuffled_img)
    H, W = arr.shape[:2]
    C = arr.shape[2] if arr.ndim == 3 else 1

    usable_rows = (H // number_rows) * number_rows
    leftover_rows = H - usable_rows
    group_count = usable_rows // number_rows

    rng = np.random.RandomState(_seed_from_key(key))
    shuffle_indices = rng.permutation(group_count)  # 原始组索引 -> 新位置顺序

    # 反向映射：new_pos -> orig_group_idx（即 shuffle_indices）
    out = np.empty_like(arr)

    for new_pos, orig_group_idx in enumerate(shuffle_indices):
        src_start = new_pos * number_rows           # 从打乱图中读取位置（第 new_pos 组）
        dst_start = orig_group_idx * number_rows    # 写回原始位置（第 orig_group_idx 组）
        out[dst_start:dst_start + number_rows] = arr[src_start:src_start + number_rows]

    # 末尾剩余行保持不变
    if leftover_rows > 0:
        out[usable_rows:] = arr[usable_rows:]

    return Image.fromarray(out)

restore_image = restore_by_rows

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/decode', methods=['POST'])
def decode():
    file = request.files['image']
    img = Image.open(file.stream)

    key = b'helloworld'
    restored = restore_image(img, number_rows = 8, key = key)

    # 转换为字节流返回
    buf = io.BytesIO()
    restored.save(buf, format='PNG')
    buf.seek(0)
    return send_file(buf, mimetype='image/png', as_attachment=False, download_name="restored.png")

if __name__ == '__main__':
    app.run(host = '0.0.0.0', port = 47778, debug = False)
