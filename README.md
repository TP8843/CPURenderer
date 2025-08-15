# CPU Raster/Pathtraced Renderer

## Installation

### Install SDL2

- First, install SDL2 as a package. Examples of packages for package managers include:
```
apt install libsdl2-dev
rpm -i SDL2-devel
yum install SDL2-devel
brew install sdl2
port install libsdl2
pacman -S sdl2
```

- If using Windows, make use of WSL2 (Windows Subsystem for Linux) to run the program as a Linux application, running one of the commands as above (e.g. `apt install libsdl2-dev` for Debian based distros like Ubuntu)

## Implemented Features
- [x] Diffuse Shading
- [x] Specular Highlights
- [x] Flat Shading
- [x] Gourard Shading
- [x] Phong Shading
- [x] Mirrors
- [x] Multiple models
- [x] Redo colour system to support HDR and arbitrary values during calculation steps
- [x] Add gamma correction to improve colour accuracy (https://learnopengl.com/Advanced-Lighting/Gamma-Correction)
- [x] Animation system
  - [x] Linear Interpolation
  - [ ] Cubic Interpolation
  - [ ] Bézier curves (for smoother movement)
- [ ] Metallic shading (tinted mirror surface with perturbations)
- [ ] Multiple lights
- [x] Concurrency

## Generating Debug and Release Makefiles

### Debug Makefile
 
```shell
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

### Release Makefile
```shell
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

Then, to build the project, enter the corresponding folder and
run `make`. Run the project with `./RedNoise`
