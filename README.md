# Zia

### What is Zia ?

Zia is a **cross-platform**, **intermodulable**, **HTTP server** written in **C++17**.
It is using **conan** for packet management and **Cmake** for the build process.
The API used to write module was designed by MatthieuMv : https://github.com/MatthieuMv/openZia.

### How to build ?

##### Unix

Simply execute build.sh script to init submodules and build the project.

##### Window

Init submodule : *git submodule init && git submodule update*.
Install **conan** dependencies into an empty folder.
Use **Cmake** to create your .vcproj into the folder.
Build .exe and .dll using Visual Studio.