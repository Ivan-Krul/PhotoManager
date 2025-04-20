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

For vcpkg the path to it's ports has to be defined `VCPKG_ROOT`
in enviroumental variables (or command `$env:VCPKG_ROOT = "C:/path/to/your/root/vcpkg" $env:PATH = "$env:VCPKG_ROOT;$env:PATH"`).

Then you generate and then build CMake project using presets.
To discover your preferable preset, you should look in root directory `cmake --list-presets`
(For that, you should change the build preset to your preferable).
You just choose your preset and build via Visual Studio (default)
or using gcc/clang by modifying configuration preset in `CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER`.
Now you can build using `cmake --build --preset=default-build-windows`. And finally you would get a program.

## For Ubuntu/Debian
I still have not discovered how to do this in those OSes.


## For MacOS
I have no idea what to do in this OS. I have never had in any device that uses MacOS.
I guess you should compile as for Ubuntu/Debian.
