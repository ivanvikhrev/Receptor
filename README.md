# Receptores
CV summer camp project

## Out-of-source build
Make directory  Receptor-build, then open  cmd and run next:
  cmake -DOpenCV_DIR="<Path_to_OpenCV_build>" -G"<generatoe name>" <Path_to_dir_with_CMakeLists>  
  *Например:*  
  cmake -DOpenCV_DIR="C:/OpenCV41/opencv/build/" -G"Visual Studio 15 2017 Win64" ../Receptor
