import string
import random
import os


def main(): 
    i = 0
    flag = False
    for filename in os.listdir("Images/"):
       filenameOnly, fileExtension = os.path.splitext(filename) 
       if ( fileExtension == ".png"):
           os.remove("Images/" + filename)
           os.remove("Labels/" + filenameOnly +".xml")
    
 
  
# Driver Code 
if __name__ == '__main__': 
      
    # Calling main() function 
    main() 