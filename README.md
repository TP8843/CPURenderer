# CPU Raster/Pathtraced Renderer

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
