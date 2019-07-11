from PIL import Image
import os
 
from os.path import basename
 
def resizeImages(baseDir, outputdir):
    basewidth = 558
    for filename in os.listdir(baseDir):
        filenameOnly, file_extension = os.path.splitext(filename)
        # print (file_extension)
        if (file_extension in [".jpg", '.png']):
            filepath = baseDir + os.sep + filename
            img = Image.open(filepath)
            wpercent = (basewidth/float(img.size[0]))
            hsize = int((float(img.size[1])*float(wpercent)))
            img = img.resize((basewidth,hsize), Image.ANTIALIAS)
            img.save(filepath)
            print (filenameOnly, "Done")
    print('Done')
         
# Usage
baseDir = "C:/Users/Yuki/source/repos/Receptor/data/in"
outputDir = "C:/Users/Yuki/source/repos/Receptor/out"
resizeImages(baseDir, outputDir)
# baseDir = 'dir'