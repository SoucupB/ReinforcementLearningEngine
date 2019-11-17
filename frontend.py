import tkinter as tk
from tkinter import Tk, Canvas, Frame, BOTH, NW
from PIL import Image, ImageTk
import sys

import socket
TCP_IP = '127.0.0.1'
TCP_PORT = 5555
BUFFER_SIZE = 1024
MESSAGE = "".encode('UTF-8')

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

class GraphicDesign():
    def __init__(self, game=None, size=None, name=None):
        if size != None:
            self.size = size
        self.name = name
        self.wind = tk.Tk()
        self.height = 500
        self.width = 500
        self.posX = 0
        self.posY = 0
        self.file = -1
        self.images = {}
        self.game = game
        self.game_map = []
    def clasifyPieces(self, pieces):
        for piece in pieces:
            self.images[piece] = Image.open(piece.imagePath).resize((self.deltaY - 1, self.deltaX - 1))
    def receiveTable(self):
        for indi in range(len(self.table)):
            for indj in range(len(self.table[indi])):
                if self.table[indi][indj] in self.images:
                    self.drawAt(indi, indj, self.images[self.table[indi][indj]])
    def downloadFile(self, file, index):
        img = Image.open(file).resize((self.deltaY - 1, self.deltaX - 1))
        self.image = ImageTk.PhotoImage(img)
        self.images[index] = self.image
        print(self.image)
    def createCanvasGrid(self):
        self.canvas = tk.Canvas(self.wind, height = self.height, width = self.width, bg='white')
        self.canvas.place(relx=0.03, rely=0.03)
    def setSize(self):
        if self.size != None:
            self.wind.geometry(str(self.size[0]) + "x" + str(self.size[1]))
    def createWindow(self):
        self.wind.title(self.name)
        self.setSize()
    def lastInstruction(self):
        self.wind.mainloop()
    def createLine(self, x, y, xF, yF):
        self.canvas.create_line([(x, y), (xF, yF)], tag="grid_line")
    def getDivider(self, sizer, number):
        a = sizer
        b = sizer
        while a % number != 0 and b % number != 0:
            a += 1
            b -= 1
        if a % number == 0:
            return a
        if b % number == 0:
            return b
    def motion(self, event):
        self.posX, self.posY = event.x, event.y
        self.getPosRelativeToCanvasO()
    def connect4_check_if_element_is_right_placed(self, x, y):
        return y == 5 or self.game_map[y + 1][x] != 0
    #FRONT END MOVES
    def check_if_there_is_element_in_table(self, x, y):
        return self.game_map[y][x] == 0
    def getPosRelativeToCanvasO(self):
        self.posX = self.posX // self.deltaY
        self.posY = self.posY // self.deltaX
        if self.game_name == "connect4" and not self.connect4_check_if_element_is_right_placed(self.posX, self.posY):
            return
        if self.check_if_there_is_element_in_table(self.posX, self.posY):
            MESSAGE = (str(self.posX) + str(self.posY)).encode('UTF-8')
            self.game_map[self.posY][self.posX] = 1
            s.send(MESSAGE)
            self.drawAt(self.posY, self.posX, self.images[4])
            data = s.recv(BUFFER_SIZE)
            self.drawAt(data[1], data[0], self.images[5])
            self.game_map[data[1]][data[0]] = 1
    #FRONT END MOVES
    def getPosRelativeToCanvasX(self):
        self.posX = self.posX // self.deltaY
        self.posY = self.posY // self.deltaX
    def bind(self):
        self.canvas.bind('<Button-1>', self.motion)
    def createGrid(self, x, y, game_name):
        self.x = x
        self.y = y
        self.height = self.getDivider(self.height, y)
        self.width = self.getDivider(self.width, x)
        self.createCanvasGrid()
        self.deltaX = self.width // x
        self.deltaY = self.height // y
        self.game_name = game_name
        for row in range(x):
            row_array = []
            for column in range(y):
                row_array.append(0)
            self.game_map.append(row_array)
        for index in range(0, self.height, self.deltaY):
            self.createLine(index, 0, index, index + self.width)
        for index in range(0, self.width, self.deltaX):
            self.createLine(0, index, index + self.height, index)
    def drawAt(self, x, y, img):
        self.canvas.create_image(y * self.deltaY + 1, x * self.deltaX + 1, anchor=NW, image=img)
print("start")
window = GraphicDesign(size = (530, 530), name = "BoardTool")
window.createWindow()
if sys.argv[1] == "5x5":
    window.createGrid(5, 5, "5x5")
if sys.argv[1] == "connect4":
    window.createGrid(6, 7, "connect4")
if sys.argv[1] == "3x3":
    window.createGrid(3, 3, "3x3")
if sys.argv[1] == "7x7":
    window.createGrid(7, 7, "7x7")
window.downloadFile("first.png", 4)
window.downloadFile("second.png", 5)
window.bind()
window.lastInstruction()
print("stop")