#! /usr/bin/env python3
# converts all the given svg files in a directory into pgn files

from svglib.svglib import svg2rlg
from reportlab.graphics import renderPM
import glob
import os
import sys

#directory = 'piece/alpha/*.svg'
out_dir = '.'
if len(sys.argv) != 2:
    raise ValueError('Please enter input directory')
directory = os.path.normpath(sys.argv[1])
directory = os.path.join(directory, "*.svg")
lst = glob.glob(directory)

for filename in lst:
    drawing = svg2rlg(filename)
    
    temp = filename.split("/")

    out_name = temp[len(temp)-1].split(".")[0] + '.png'

    renderPM.drawToFile(drawing, os.path.join(out_dir, out_name), fmt = "PNG")


    

