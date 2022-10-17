import os
from os import mkdir
from PIL import Image
import sys

# ConvertImages.py <inputfile path> <size>
if (len(sys.argv) != 3):
    print("Invalid options provided")
    exit(1)
else:
    image = Image.open(sys.argv[1])
    image = image.convert("RGBA")
    image = image.resize((int(sys.argv[2]),int(sys.argv[2])))
    canvas = Image.new('RGBA', (int(sys.argv[2]),int(sys.argv[2])), (0,0,0,0))
    canvas.paste(image, mask=image)
    if (os.path.isdir("bmp") == False):
        mkdir("bmp")
    newFilename = sys.argv[1].split(os.sep)[-1].split(".")[0]
    canvas.save("bmp" + os.sep + newFilename +"_"+ sys.argv[2] + "px" + ".bmp", format="BMP")
    exit(0)