from math import sqrt
from operator import itemgetter
from copy import deepcopy
import datetime
import sys


class ArtePlanner:
    # Map size 200 x 200
    _MAP_SIZE = 200

    _artes = {}
    _offs = {}
    _arteComplete = {}
    _offData = []
    _offDataStr = []

    def __init__(self):
        pass

    def readArte(self):
        """
        Reads the arteData.txt and write it in dict _artes.
        _artes = { ID : [ NAME, [X, Y] ] }
        ID = string
        """
        with open("artefacts/arteData", "r") as fobj:
            for line in fobj:
                data = line.rstrip().split()
                if data[0] == "#":
                    continue
                coords = data[2].split("/")
                self._artes[data[0]] = [data[1], [coords[0], coords[1]]]
        print("Artes parsed.\n")
        return

    def readOffs(self):
        """
        Reads the offData.txt and write it in dict _offs.
        _offs = { ID : [ ACCNAME, VILLAGE, COORDS, OFFTYPE, SPEED_OFF, TS, SHOES ] }
        ID = string
        TS = string (integer), "0" else
        SHOES = "0" or "0.25" or "0.5"
        """
        with open("offs/offData", "r") as fobj:
            for line in fobj:
                data = line.rstrip().split()
                if data[0] == "#":
                    continue
                coords = data[3].split("/")
                self._offs[data[0]] = [data[1], data[2], [coords[0], coords[1]],
                                              data[4], data[5], data[6], data[7]]
        print("Offs parsed.\n")
        return

    def deleteOff(self, offID):
        try:
            del self._offs[offID]
        except KeyError:
            print("Off nicht vorhanden bzw. schon gelöscht\n")
            return


    def printArteAndRuntimes(self, arteID):
        try:
            arte = self._artes[arteID]
        except KeyError:
            print("Falsche Arte ID!\n")
            return

        # Go through offs and get the runtimes and store them.
        for key in self._offs:
            off = [self._offs[key]]
            runtime = self.calculateRuntime(arteID, key)
            self._offData.insert(len(self._offData),[ self._offs[key][0],
                        self._offs[key][1], self._offs[key][2],
                        self._offs[key][3], runtime, "ID: " + key])

        # Sort the offs by runtime and copy it for converting the runtime.
        self._offData = sorted(self._offData, key=itemgetter(4))
        self._offDataStr = deepcopy(self._offData)

        # Coverting the runtime in HH:MM:SS format.
        for i in self._offDataStr:
            rt = i[4]
            i[4] = str(datetime.timedelta(rt)).split(".")[0]
        # Complete Data.
        self._arteComplete[arteID] = self._offDataStr

        print(arte)

        for off in self._arteComplete[arteID]:
            if off[3] == "Kattas":
                sys.stdout.write("\033[1;31m")
                print(off)
                sys.stdout.write("\033[0;0m")
            else:
                print(off)


        # Delete tmp lists.
        self._offDataStr = []
        self._offData = []

        return


    def calculateDistance(self, x1, y1, x2, y2):
        #if x1 < 0 and x2 < 0:
        a = abs(x1 - x2)
        b = abs(y1 - y2)

        if a < self._MAP_SIZE:
            a **= 2
        else:
            a = (self._MAP_SIZE * 2 + 1 - a)
            a **= 2

        if b < self._MAP_SIZE:
            b **= 2
        else:
            b = (self._MAP_SIZE * 2 + 1 - b)
            b **= 2
        return sqrt(a + b)


    def calculateRuntime(self, arteID, offID):
        # get the data from _offs and _artes
        x1 = int(self._offs[offID][2][0])
        y1 = int(self._offs[offID][2][1])
        x2 = int(self._artes[arteID][1][0])
        y2 = int(self._artes[arteID][1][1])

        distance = self.calculateDistance(x1, y1, x2, y2)
        speed = int(self._offs[offID][4])
        ts = int(self._offs[offID][5])
        shoes = float(self._offs[offID][6])

        # compute partial results
        if distance <= 20:
            rt = (distance / speed) / 24
        else:
            dist = distance - 20
            spd = speed * (1 + shoes + (ts * 0.2))
            # runtime in seconds
            rt = ((20 / speed) + (dist / spd)) / 24
        return rt

if __name__ == "__main__":
    ap = ArtePlanner()
    ap.readOffs()
    ap.readArte()

    main_menue = ("*** ARTEPLANER 6000 *** \n"
          "Type a or A to parse artes\n"
          "Type o or O to parse offs\n"
          "Type p or P to print one artefact with it´s offs\n"
          "Type d or D for deleting a off\n"
          "Type b or B to go back to main menue\n")
    print(main_menue)
    while (True):
        user = input("\nEingabe: ")
        if user == "d" or user == "D":
            off_id = input("Bitte Off ID eingeben: ")
            ap.deleteOff(off_id)
        elif user == "a" or user == "a":
            ap.readArte()
        elif user == "o" or user == "O":
            ap.readOffs()
        elif user == "p" or user == "P":
            arte_id = input("Bitte Arte ID eingeben: ")
            ap.printArteAndRuntimes(arte_id)
        elif user == "b" or user == "B":
            print(main_menue)
        else:
            print(main_menue)
