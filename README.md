# AnotherSTLConverter
An STL converter written in C++ that makes use of the standard C++ library. The converter takes in two arguments,
the file you are looking to convert and where to save the converted version.

#Install
```
git clone
cd AnotherSTLConverter
g++ main_desktop.cpp ReadingSTL.cpp ReadingSTL.h -o convertSTL
sudo mv convertSTL /bin/
cd ..
rm AnotherSTLConverter/ -r
```

#Using the Converter
In order to run the converter, use the following format in terminal.
```
convertSTL /path/to/STL/file /path/to/save/STL/file
```
