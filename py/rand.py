import string
import random
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
 
baseDir =  "C:\Users\temp2019\Desktop\Receptor\images"
assignRandomNames(baseDir)