from PIL import Image, ImageDraw, ImageFont
import numpy as np

class imgToTextImage:
    def __init__(self, src_path :str, dst_path :str,
                resize_factor :float = 0.7,
                sample_step   :int   = 3,
                scale         :int   = 2,
                font_size     :int   = 8,
                font_family   :str   = 'consola.ttf',
                font_encoding :str   = 'UTF-8',
                background    :int   = 0xffffff):
        self.src_path      = src_path
        self.dst_path      = dst_path
        self.resize_factor = resize_factor
        self.sample_step   = sample_step
        self.scale         = scale

        self.font          = ImageFont.truetype(font_family, font_size, encoding=font_encoding)
        self.width         = 0
        self.height        = 0
        self.background    = self.__get_rgb_tuple(background)

        self.pix           = None

        with Image.open(self.src_path) as img:
            # print(f'Original image width: {img.size[0]}, height: {img.size[1]}.')
            new_size = (int(img.size[0] * self.resize_factor), int(img.size[1] * self.resize_factor))
            with img.resize(new_size, Image.Resampling.LANCZOS) as new_img:
                with new_img.convert('RGB') as new_new_img:
                    self.pix       = new_new_img.load()
                    self.width     = new_new_img.size[0]
                    self.height    = new_new_img.size[1]

        self.canvas        = np.ndarray((self.height * self.scale, self.width * self.scale, 3), np.uint8)
        self.canvas[:,:,:] = self.background

        # print(f'New image width: {self.width}, height: {self.height}')

    def __get_rgb_tuple(self, value :int):
        return (value >> 16, (value & 0xff00) >> 8, value & 0xff)

    def __get_brightness_of_color(self, r :int, g :int, b :int, level :int):
        if level == 32: # Level: 32
            index = int((r*0.299 + g*0.587 + b*0.114) / 8)
            return r'WMHSQGFEDB@8&#$%?+\/^][|!*=~-:. '[index-1 if index == 32 else index]
        elif level == 16: # Level: 16
            index = int((r*0.299 + g*0.587 + b*0.114) / 16)
            return r'WM@GB%8&#*+=-:. '[index-1 if index == 16 else index]
        else: # Level: 8
            index = int((r*0.299 + g*0.587 + b*0.114) / 32)
            return r'#*+=-:. '[index-1 if index == 8 else index]

    def __draw_primary_colours(self, draw_obj :ImageDraw.Draw, string :str):
        length = len(string)
        index = 0
        for y in range(self.height):
            for x in range(self.width):
                if (not (x % self.sample_step)) and (not (y % self.sample_step)):
                    draw_obj.text((x * self.scale, y *self.scale), text = string[index % length], fill = self.pix[x, y], font = self.font)
                    index += 1

    def __draw_black_white(self, draw_obj :ImageDraw.Draw, fore_color :int, level :int):
        # file = open('C:/Users/sn/Desktop/收纳/result.txt', 'w', encoding='UTF-8')
        fore_color = self.__get_rgb_tuple(fore_color)
        for y in range(self.height):
            for x in range(self.width):
                if (not (x % self.sample_step)) and (not (y % self.sample_step)):
                    r, g, b = self.pix[x, y]
                    char = self.__get_brightness_of_color(r, g, b, level)
                    draw_obj.text((x * self.scale, y *self.scale), text = char,
                        fill = fore_color, font = self.font)
        #             file.write(char)
        #     file.write('\n')
        # file.close()

    def draw(self, draw_string :str = 'helloworld', fore_color :int = 0x0000000, show :bool = False, blackAndWhite :bool = False, brightnessLevel :int = 32):
        if (brightnessLevel != 32) and (brightnessLevel != 16) and (brightnessLevel != 8):
            raise RuntimeError(f'The black and white levels of the image can only be 32, 16, and 8. You provided {brightnessLevel}.')

        with Image.fromarray(self.canvas) as img:
            draw = ImageDraw.Draw(img)
            if not blackAndWhite:
                self.__draw_primary_colours(draw, draw_string)
            else:
                self.__draw_black_white(draw, fore_color, brightnessLevel)

            img.save(self.dst_path)
            if show:
                img.show()
