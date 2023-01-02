import os
from os import mkdir
from PIL import Image
from PIL import ImageColor
import sys

# ConvertImages.py <inputfile path> <size> [background color]
if (not (len(sys.argv) == 3 or len(sys.argv) == 4)):
    print("Invalid options provided - ", len(sys.argv))
    exit(1)
else:
    image = Image.open(sys.argv[1])
    image = image.convert("RGBA")
    image = image.resize((int(sys.argv[2]),int(sys.argv[2])))
    #bg_color = ImageColor.getcolor(sys.argv[3], "RGB")
    #bg_color = bg_color + (50,);
    canvas = Image.new('RGBA', (int(sys.argv[2]),int(sys.argv[2])), (0,0,0,0))
    canvas.paste(image, mask=image)
    if (os.path.isdir("bmp") == False):
        mkdir("bmp")
    newFilename = sys.argv[1].split(os.sep)[-1].split(".")[0]
    canvas.save("bmp" + os.sep + newFilename +"_"+ sys.argv[2] + "px" + ".bmp", format="BMP")
    exit(0)