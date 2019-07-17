# Pythono3 code to rename multiple  
# files in a directory or folder 
  
# importing os module 
import os 
import xml.etree.ElementTree as ET
  
def main(): 
    for filename in os.listdir("Labels/"): 
        filenameOnly _ = os.path.splitext(filename) 
        print(filename)
        with open(filename, encoding='utf-8') as f:
            tree = ET.parse(f)
            root = tree.getroot()

            for elem in root.getiterator():
                 if elem.text == "apple":
                     print(filename)
           
 
  
# Driver Code 
if __name__ == '__main__': 
      
    # Calling main() function 
    main() 