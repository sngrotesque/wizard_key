from base64 import b64decode as bde, b64encode as ben
from binascii import a2b_hex as a2b, b2a_hex as b2a

def fill(data :bytes):
    data = list(data)
    datalen = len(data)
    index = 0
    for x in range(256 - datalen):
        if index == datalen:
            index = 0
        data.append(data[index])
        index += 1
    return a2b("".join(["%02X"%x for x in data]))

def B64(source):
    _input = bde(source)
    str_input = str(_input, encoding="UTF-8", errors="ignore")

    _hex_input = b2a(_input).decode()
    hex_input = ""
    index = 0
    for x in range(len(_hex_input)//2):
        hex_input += (_hex_input[index:index+2] + "  ").upper()
        if (x + 1) % 42 == 0:
            hex_input += "\n"
        index += 2

    print(f"以下是原Base64编码数据:\n{source}\n")
    print(f"以下是解码之后的数据(二进制):\n{_input}\n")
    print(f"以下是解码之后的数据(硬解UTF-8):\n{str_input}\n")
    print(f"以下是解码之后的数据(十六进制):\n{hex_input}\n")
    return "Done."




data = "lahLy384j3+Cr7TPq5nnh0OdmYuKuuB/YRNi91EkSLGy1XEjq0WAsGr11w0Y0bfGPjvMU5/RZW9mVswjbd9msw04ZPX+8f23h5nsbFYJk4mc/WMaGu9IL3tlocOyoqg3KTva/2lc8OlaPemrE53IGDINHzWQeY++JtOUe/d5TQfOnhTbFefee8VQbC3muXWfj9EU9J4hW1H0ZZsSAsfwd9ONSVu6atDv0QNS50G0uAGiRQHTmxXwoBplh31IwSfW7su8w+/Blb8WxjzT3Y+WI/0IFIVuoY2nNykc3Mb5g/nMLRPqSj84H8sV0VOiktgnGWtK75ls4HnKrXlbA604Bg=="

print(B64(data))