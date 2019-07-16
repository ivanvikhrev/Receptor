import string
import random
import os


def main(): 
    i = 0
    flag = False
    for filename in os.listdir("Images/"):
       if i%5 == 0 :
         filenameOnly, _ = os.path.splitext(filename) 
         os.rename("Images/" + filename, "TestImg/"+filename)
         os.rename("Labels/" + filenameOnly +".xml", "TestLab/"+filenameOnly+".xml")
         if (i==186): break
       i = i+1
           
 
  
# Driver Code 
if __name__ == '__main__': 
      
    # Calling main() function 
    main() 