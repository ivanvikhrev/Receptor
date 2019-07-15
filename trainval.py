from PIL import Image
import string
import random
import os

from os.path import basename
 
def createTrainvalTxt(baseDirDataSet):
    buffer = ''
    baseDir = baseDirDataSet+'\Images'
    for filename in os.listdir(baseDir):
        filenameOnly, file_extension = os.path.splitext(filename)
        # print (file_extension)
        s = 'Images/'+filenameOnly+'.jpg'+' '+'Labels/'+filenameOnly+'.xml\n'
        print (repr(s))
        img_file, anno = s.strip("\n").split(" ")
        print(repr(img_file), repr(anno))
        buffer+=s
    with open(baseDirDataSet+'\\Structure\\trainval.txt', 'w') as file:
        file.write(buffer)  
    print('Done')   
#Usage
createTrainvalTxt('C:\Temp\Receptor\data')