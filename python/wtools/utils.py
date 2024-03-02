import lzma

class w_utils:
    def fread(self, path :str, open_mode :str = 'rb', encoding :str = None):
        with open(path, open_mode, encoding = encoding) as f_obj:
            return f_obj.read()

    def fwrite(self, path :str, open_mode :str = 'wb', content :bytes = None, encoding :str = None):
        with open(path, open_mode, encoding = encoding) as f_obj:
            return f_obj.write(content)

    def lzma2_compress(self, data :bytes):
        compressor = lzma.LZMACompressor(format = lzma.FORMAT_RAW, filters=[{"id": lzma.FILTER_LZMA2, "preset": 9 | lzma.PRESET_EXTREME}])
        return compressor.compress(data) + compressor.flush()

    def lzma2_decompress(self, data :bytes):
        decompressor = lzma.LZMADecompressor(format = lzma.FORMAT_RAW, filters = [{"id": lzma.FILTER_LZMA2, "preset": 9 | lzma.PRESET_EXTREME}])
        return decompressor.decompress(data)

