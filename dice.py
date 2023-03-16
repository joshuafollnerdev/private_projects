__author__ = "Joshua Follner"
__copyright__ = "Copyright (C) 2020 Joshua Follner"
__license__ = "GPL-3-0-only"
__version__ ="1.0"

import Keyboardinput
from random import randint
import os

class Dice:
    def __init__(self):
        self.three_d20 = [0,0,0]
        self.d6 = 0
        self.d20 = 0

    def get_three_d20(self):
        for counter in range(3):
            self.three_d20[counter] = randint(1,20)
        return self.three_d20

    def get_one_d20(self):
        self.d20 = randint(1,20)
        return self.d20

    def get_one_d6(self):
        self.d6 = randint(1,6)
        return self.d6

    pass


if __name__ == "__main__":
    dice = Dice()
    clear = lambda: os.system('clear')
    print("Supertolles Würfelpartyparadies")
    print("Exit : Esc\n3-D20 : Space\nD20 : m\nD6 : Enter\nHelp: h")

    while True:
        ch = Keyboardinput.Getch.getch()
        if ch == "b":
            clear()
            print("Drei - D20:")
            print(dice.get_three_d20())
            continue
        if ch == "n":
            clear()
            print("Ein - D6:")
            print(dice.get_one_d6())
            continue
        if ch == "m":
            clear()
            print("Ein - D20:")
            print(dice.get_one_d20())
            continue
        if ch == "h":
            clear()
            print("Exit : Esc\n3-D20 : Space\nD20 : m\nD6 : Enter\nHelp: h")
        if ch == "\x1b":
            exit()
        else:
            continue
