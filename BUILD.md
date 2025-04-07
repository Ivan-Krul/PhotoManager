# Build
If you would like to build this, then you should read and look up like in account balance.
After cloning the repository, you will need package inclusion.
In this stage it depends in Operating System.
There are those dependancies:

- libjpeg: manipulation with .jpeg images
- Exiv2: managing with metadata with 
- wxWidgets (and their 3rd party library): graphic interface for users
- libwebp: manipulation with .webp images


## For Windows
dependancies are handled via
[vcpkg](https://github.com/Microsoft/vcpkg/) and
[gitmodule](https://git-scm.com/docs/gitmodules).
First you need to install vcpkg by following instructions of their
[README](https://github.com/Microsoft/vcpkg/blob/master/README.md).

Then you include dependancies within `gitmodules`.
Just type this command in root directory `git submodule update --init --recursive`. 

Then you launch CMake with using local enviroumental preset.
To start generating, you have to create a file 


## For Ubuntu/Debian
I still have not discovered how to do this in those OSes.


## For MacOS
I have no idea what to do in this OS. I have never had in any device that uses MacOS.
I guess you should compile as for Ubuntu/Debian.
