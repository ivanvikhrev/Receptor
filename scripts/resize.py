from PIL import Image
import string
import random
import os

from os.path import basename
 
def resizeImages(baseDir, outputDir):
    basewidth = 558
    for filename in os.listdir(baseDir):
        filenameOnly, file_extension = os.path.splitext(filename)
        # print (file_extension)
        if (file_extension in [".jpg", '.png', ".jfif"]):
            filepath = baseDir + os.sep + filename
            img = Image.open(filepath)
            wpercent = (basewidth/float(img.size[0]))
            hsize = int((float(img.size[1])*float(wpercent)))
            img = img.resize((basewidth,hsize), Image.ANTIALIAS)
            if (file_extension == ".jfif" ): 
                print(file_extension )
                file_extension = ".jpg"
                print(file_extension )
            filepath = outputDir + "/"+ filenameOnly + file_extension
            print(filepath)
            img.save(filepath)
            print (filenameOnly, "Done")
    print('Done')

def id_generator(size=6, chars=string.ascii_uppercase + string.digits):
    return ''.join(random.choice(chars) for _ in range(size))
 
def assignRandomNames(baseDir):
    for filename in os.listdir(baseDir):
        filepath = baseDir + os.sep + filename
        if os.path.isfile(filepath):
            finalFolder = baseDir
            filenameOnly, file_extension = os.path.splitext(finalFolder + os.sep + filename)
            os.rename(filepath, finalFolder + os.sep + id_generator()+file_extension)
# Usage
# Usage
baseDir = "raw rice"
outputDir = "raw rice"
resizeImages(baseDir, outputDir)
assignRandomNames(outputDir)     
# baseDir = 'dir'
