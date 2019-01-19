## tubes connect
> almost completed

Given a stl file which contains many unconnected tubes, the program will connect them automatically

### PREREQUISITES
* GCC
* VTK
* CMake
* GTest(Optional)

### BUILD
#### LINUX
- just run `cd tube_connect`, `cmake .`and `make` 
  
- or you can use out-of-source-build
```
cd tube_connect
mkdir build
cd build
cmake ..
make
```
- If VTK is not installed but compiled on your system, you will need to specify the path to your VTK build  
`cmake -DVTK_DIR:PATH=/home/me/vtk_build ..`
#### WINDOWS
Be sure to add the VTK **bin** and **lib** directory to your path. This will resolve the VTK dll's at run time

### RUN
`./tube_connect`

### TEST
If you want to run the test, just run `cd test` and `./tube_connect_test`

### MORE
1. You can now run the test stl file given by me. Later, I will add GUI to the program and you can import your own stl file
2. Sadly, though my program perform not bad on test stl file, it probably perform not well on other stl file