from typing import List, Union, Tuple
import random

melody = ['C', 'D', 'E', 'F', 'G', 'A', 'B', 'C#', 'D#', 'F#', 'G#', 'A#']

class wmkcMusic:
    def __init__(self, section :Union[List[int], Tuple[int]]):
        self.sections = [[0 for _ in range(length)] for length in section]
        self.section  = section

    def generate(self):
        for section in self.sections:
            for i in range(len(section)):
                section[i] = random.choice(self.melody)
        return self.sections
