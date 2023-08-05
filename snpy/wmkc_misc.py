from typing import Union

def wmkcMisc_PRINT_BOX(content :bytes) -> str:
    size = len(content)
    text = ''
    for x in range(size):
        text += f'0x{content[x]:02x}'
        if (x + 1) != size:
            if not (x + 1) % 16:
                text += ',\n'
            else:
                text += ', '
    return text
