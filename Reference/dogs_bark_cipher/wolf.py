class HowlingAnimalsTranslator:
    animalText = "嗷呜啊~"

    # def __init__(self, newAnimalVoice = None):
    #     self.setAnimalVoice(newAnimalVoice)

    def convert(self, txt):
        result = self.animalText[3] + self.animalText[1] \
            + self.animalText[0]
        offset = 0
        for t in txt:
            c = ord(t)
            b = 12
            while(b >= 0):
                hex = (c >> b) + offset & 15
                offset += 1
                result += self.animalText[int(hex >> 2)]
                result += self.animalText[int(hex & 3)]
                b -= 4
        result += self.animalText[2]
        return result

    def deConvert(self, txt):
        if(not self.identify(txt)):
            return "Incorrect format!"
        result = ""
        i = 3
        offset = 0
        while(i < len(txt)-1):
            c = 0
            b = i + 8
            while(i < b):
                n1 = self.animalText.index(txt[i])
                i += 1
                n2 = self.animalText.index(txt[i])
                c = c << 4 | ((n1 << 2 | n2)+offset) & 15
                if(offset == 0):
                    offset = 0x10000 * 0x10000 - 1
                else:
                    offset -= 1
                i += 1
            result += chr(c)
        return result

    def identify(self, txt):
        if(txt):
            if(len(txt) > 11):
                if(
                    txt[0] == self.animalText[3] and
                    txt[1] == self.animalText[1] and
                    txt[2] == self.animalText[0] and
                    txt[-1] == self.animalText[2] and
                    ((len(txt) - 4) % 8) == 0
                ):
                    for t in txt:
                        if(not self.animalText.__contains__(t)):
                            return False
                    return True
        return False

    # def setAnimalVoice(self, voiceTxt):
    #     if(voiceTxt):
    #         if(len(voiceTxt) == 4):
    #             self.animalText = voiceTxt
    #             return True
    #     return False

    # def getAnimalVoice(self):
    #     return self.animalText

if __name__ == "__main__":
    data = HowlingAnimalsTranslator()
    res = data.convert("hello, world")
    print(res)
