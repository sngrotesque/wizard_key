from qrcode.image.styledpil import StyledPilImage
from typing import Union
import qrcode

QRCODE_ERROR_L = qrcode.constants.ERROR_CORRECT_L
QRCODE_ERROR_M = qrcode.constants.ERROR_CORRECT_M
QRCODE_ERROR_Q = qrcode.constants.ERROR_CORRECT_Q
QRCODE_ERROR_H = qrcode.constants.ERROR_CORRECT_H

# Pillow old version: 8.4.0
# Qrcode old version: 6.1
class wmkcQrcode:
    def __init__(self,
                box_size :int = 16,
                border :int = 1,
                fill_color :Union[str, tuple] = '#000000',
                back_color :Union[str, tuple] = '#ffffff',
                error_correction = QRCODE_ERROR_H,
                image_path :str = None
                ):
        self.box_size   = box_size
        self.border     = border
        self.fill_color = fill_color
        self.back_color = back_color
        self.image_path = image_path
        self.error_correction = error_correction

        self.qr  = None
        self.img = None

    def init(self):
        self.qr = qrcode.QRCode(
            box_size = self.box_size,
            border = self.border,
            error_correction = self.error_correction
        )

    def add(self, content :str):
        self.qr.add_data(content)

    def make_text(self):
        self.qr.make()
        self.img = self.qr.make_image(
            fill_color = self.fill_color,
            back_color = self.back_color
        )

    def make_image(self):
        self.qr.make()
        self.img = self.qr.make_image(
            image_factory = StyledPilImage,
            embeded_image_path = self.image_path,
            back_color = self.back_color
        )

    def show(self):
        self.img.show()

    def save(self, save_path: str):
        self.img.save(save_path)

