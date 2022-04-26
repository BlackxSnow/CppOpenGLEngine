# CppOpenGLEngine

This is a purely educational project focused on low level game engine creation using OpenGL. Note that this is not intended to be used for anything, and is by all means a very poorly functioning project. Refer to ECSSpaceSim for a more robust engine design (and better architecture).

Requires a C++ ISO 17 capable compiler.

Untested on platforms other than Windows 10 x64.

To generate the solution and project files run `premake5 [version]` on the command line, where `[version]` is replaced by the relevant IDE version. (See: https://premake.github.io/docs/Using-Premake/).

Current major feature set:  
* Obj importer and mesh based rendering
* Custom shader support
* Phong lighting model
* Directional and Spot light shadowmapping
* Immediate text rendering and font support
* AABB && Sphere based elastic physics and collision detection