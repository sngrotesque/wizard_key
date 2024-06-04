def fread(path :str, mode :str = 'rb', encoding :str = None):
        with open(path, mode, encoding = encoding) as f_obj:
            return f_obj.read()

def fwrite(path :str, mode :str = 'wb', data :bytes = None, encoding :str = None):
    with open(path, mode, encoding = encoding) as f_obj:
        return f_obj.write(data)
