from PIL import Image
import numpy as np
import zlib
import os

def _seed_from_key(key: bytes) -> int:
    return zlib.crc32(key)

def shuffle_by_rows(img: Image.Image, number_rows: int = 3, key: bytes = b'keykeykey') -> Image.Image:
    """
    将图像按每 number_rows 行打乱；不足 number_rows 的剩余行保持在末尾不变。
    """
    if number_rows <= 0:
        raise ValueError("number_rows 必须为正整数")

    arr = np.asarray(img)
    H, W = arr.shape[:2]
    C = arr.shape[2] if arr.ndim == 3 else 1

    usable_rows = (H // number_rows) * number_rows
    leftover_rows = H - usable_rows
    group_count = usable_rows // number_rows

    rng = np.random.RandomState(_seed_from_key(key))
    shuffle_indices = rng.permutation(group_count)  # 原始组索引 -> 新位置顺序

    out = np.empty_like(arr)

    # 将每个原始组写入到打乱后的新位置
    for new_pos, orig_group_idx in enumerate(shuffle_indices):
        src_start = orig_group_idx * number_rows
        dst_start = new_pos * number_rows
        out[dst_start:dst_start + number_rows] = arr[src_start:src_start + number_rows]

    # 末尾剩余行保持不变
    if leftover_rows > 0:
        out[usable_rows:] = arr[usable_rows:]

    return Image.fromarray(out)

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

# 示例用法
if __name__ == "__main__":
    # original_path = "E:/misc/image_test/2025_06_25_03_41_14_131944914_p0.png"
    original_path = r"F:\Pitchers\二次元玉足\133343667_p0.png"
    shuffled_path = "shuffled.png"
    restored_path = "restored.png"
    number_rows   = 8
    test_key      = b'helloworld'

    img = Image.open(original_path).convert("RGB")

    # 打乱
    shuffled_img = shuffle_by_rows(img, number_rows = number_rows, key = test_key)
    shuffled_img.save(shuffled_path)

    # 还原
    # restored_img = restore_by_rows(shuffled_img, number_rows = number_rows, key = test_key)
    # restored_img.save(restored_path)
