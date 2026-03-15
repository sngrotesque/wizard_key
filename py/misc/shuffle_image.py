from concurrent.futures import ThreadPoolExecutor
from PIL import Image
import numpy as np
import time

####################################################
# Helpers

def _is_color(arr: np.ndarray) -> bool:
    return arr.ndim == 3

def _channels(arr: np.ndarray) -> int:
    return arr.shape[2] if _is_color(arr) else 1

def _as_flat(arr: np.ndarray) -> np.ndarray:
    # Returns shape (N, C) for color, or (N,) for gray
    return arr.reshape(-1, arr.shape[2]) if _is_color(arr) else arr.reshape(-1)

def _from_flat(flat: np.ndarray, H: int, W: int, C: int) -> np.ndarray:
    return flat.reshape(H, W, C) if C > 1 else flat.reshape(H, W)

####################################################
# Flip: row-major -> column-major (shuffle) and inverse (restore)

def flip_shuffle(img: Image.Image) -> Image.Image:
    arr = np.array(img)
    H, W = arr.shape[:2]
    C = _channels(arr)
    out = np.empty_like(arr)

    flat = _as_flat(arr)

    def _copy_column(c: int):
        # flat segment [c*H:(c+1)*H] -> out[:, c]
        segment = flat[c * H:(c + 1) * H]
        if C > 1:
            out[:, c, :] = segment
        else:
            out[:, c] = segment

    with ThreadPoolExecutor() as ex:
        ex.map(_copy_column, range(W))

    return Image.fromarray(out)

def flip_restore(shuffled_img: Image.Image) -> Image.Image:
    arr = np.array(shuffled_img)
    H, W = arr.shape[:2]
    C = _channels(arr)

    flat = np.empty((H * W, C), dtype=arr.dtype) if C > 1 else np.empty(H * W, dtype=arr.dtype)

    def _read_column(c: int):
        segment = arr[:, c, :] if C > 1 else arr[:, c]
        flat[c * H:(c + 1) * H] = segment

    with ThreadPoolExecutor() as ex:
        ex.map(_read_column, range(W))

    out = _from_flat(flat, H, W, C)
    return Image.fromarray(out)

####################################################
# Spiral coords

# Spiral shuffle/restore (chunked multithreading)

'''
def spiral_coords(H, W):
    top, left = 0, 0
    bottom, right = H - 1, W - 1
    while top <= bottom and left <= right:
        for j in range(left, right + 1):
            yield top, j
        top += 1
        for i in range(top, bottom + 1):
            yield i, right
        right -= 1
        if top <= bottom:
            for j in range(right, left - 1, -1):
                yield bottom, j
            bottom -= 1
        if left <= right:
            for i in range(bottom, top - 1, -1):
                yield i, left
            left += 1

def spiral_shuffle(img: Image.Image) -> Image.Image:
    arr = np.array(img)
    H, W = arr.shape[:2]
    C = _channels(arr)
    out = np.empty_like(arr)

    coords = list(spiral_coords(H, W))
    flat = _as_flat(arr)

    # Split into chunks for concurrent write
    N = H * W
    num_workers = min(32, max(1, N // 65536))  # heuristic
    chunk_size = (N + num_workers - 1) // num_workers

    def _write_chunk(chunk_idx: int):
        start = chunk_idx * chunk_size
        end = min(N, start + chunk_size)
        if start >= end:
            return
        for k in range(start, end):
            r, c = coords[k]
            if C > 1:
                out[r, c, :] = flat[k]
            else:
                out[r, c] = flat[k]

    with ThreadPoolExecutor(max_workers=num_workers) as ex:
        ex.map(_write_chunk, range(num_workers))

    return Image.fromarray(out)

def spiral_restore(shuffled_img: Image.Image) -> Image.Image:
    arr = np.array(shuffled_img)
    H, W = arr.shape[:2]
    C = _channels(arr)

    coords = list(spiral_coords(H, W))
    N = H * W
    flat = np.empty((N, C), dtype=arr.dtype) if C > 1 else np.empty(N, dtype=arr.dtype)

    num_workers = min(32, max(1, N // 65536))
    chunk_size = (N + num_workers - 1) // num_workers

    def _read_chunk(chunk_idx: int):
        start = chunk_idx * chunk_size
        end = min(N, start + chunk_size)
        if start >= end:
            return
        for k in range(start, end):
            r, c = coords[k]
            if C > 1:
                flat[k] = arr[r, c, :]
            else:
                flat[k] = arr[r, c]

    with ThreadPoolExecutor(max_workers=num_workers) as ex:
        ex.map(_read_chunk, range(num_workers))

    out = _from_flat(flat, H, W, C)
    return Image.fromarray(out)
'''

####################################################
# Checkerboard shuffle/restore (vectorized, size-safe)

def checkerboard_shuffle(img: Image.Image) -> Image.Image:
    arr = np.array(img)
    H, W = arr.shape[:2]
    C = _channels(arr)

    # Build mask of even positions (i+j)%2==0
    ii = np.arange(H)[:, None]
    jj = np.arange(W)[None, :]
    mask = ((ii + jj) % 2) == 0

    if C > 1:
        even_flat = arr[mask]
        odd_flat = arr[~mask]
        flat = np.concatenate([even_flat, odd_flat], axis=0)
    else:
        even_flat = arr[mask]
        odd_flat = arr[~mask]
        flat = np.concatenate([even_flat, odd_flat], axis=0)

    out = _from_flat(flat, H, W, C)
    return Image.fromarray(out)

def checkerboard_restore(shuffled_img: Image.Image) -> Image.Image:
    arr = np.array(shuffled_img)
    H, W = arr.shape[:2]
    C = _channels(arr)

    # Recompute mask as in shuffle
    ii = np.arange(H)[:, None]
    jj = np.arange(W)[None, :]
    mask = ((ii + jj) % 2) == 0

    total = H * W
    even_count = (total + 1) // 2
    odd_count = total - even_count

    flat = _as_flat(arr)
    even_flat = flat[:even_count]
    odd_flat = flat[even_count:even_count + odd_count]

    out = np.empty_like(arr)
    if C > 1:
        out[mask] = even_flat
        out[~mask] = odd_flat
    else:
        out[mask] = even_flat
        out[~mask] = odd_flat

    return Image.fromarray(out)

####################################################
# XOR shuffle (self-inverse), chunked multithreading

def xor_shuffle(img: Image.Image) -> Image.Image:
    xor_val = 0x7f
    arr = np.asarray(img)
    out = arr ^ xor_val
    return Image.fromarray(out)

# XOR is self-inverse: apply again to restore
xor_restore = xor_shuffle

####################################################
# Combined (pipeline, in-memory)

def combined_shuffle(img: Image.Image) -> Image.Image:
    # pipeline: checkerboard -> spiral -> xor -> flip
    return flip_shuffle(xor_shuffle(checkerboard_shuffle(img)))

def combined_restore(shuffled_img: Image.Image) -> Image.Image:
    # inverse pipeline: flip^-1 -> xor^-1 -> spiral^-1 -> checkerboard^-1
    return checkerboard_restore(xor_restore(flip_restore(shuffled_img)))

####################################################
# Entry points

shuffle_image = combined_shuffle
restore_image = combined_restore

####################################################
# 示例用法（无中间文件）
if __name__ == "__main__":
    original_path = r'E:/misc/image_test/2025_06_25_03_41_14_131944914_p0.png'
    shuffled_path = r'shuffled_image.png'
    restored_path = r'restored_image.png'

    start = time.time()
    img = Image.open(original_path)  # 输入原图
    shuffled = shuffle_image(img)
    stop = time.time()
    shuffled.save(shuffled_path)
    print(f'编码耗时：{stop-start:.4f}秒。')

    # restored = restore_image(shuffled)
    # restored.save(restored_path)
