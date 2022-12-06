#!/bin/usr/python3

import sys
import chess
import chess.svg
from PyQt5 import QtSvg
from PyQt5.QtWidgets import QApplication

START_FEN = r'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1'

def fenToSvg(fen = START_FEN, size = 400):
    
    board = chess.Board(fen)
    svgTxt = chess.svg.board(board, size=size)
    file = open('temp.svg', 'w+')
    
    file.write(svgTxt)
    

    app = QApplication(sys.argv)
    svgWidget = QtSvg.QSvgWidget()

    svgWidget.load('temp.svg')
    svgWidget.show()
    file.close()

    sys.exit(app.exec_())

fenToSvg()
