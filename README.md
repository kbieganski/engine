3D Game Engine
==============

A simple 3D game engine that uses Vulkan for rendering, OpenAL for sound and Bullet for physics.

## Dependencies

 * Boost.Test *(for unit tests)*
 * Bullet
 * GLFW
 * GLI *(included as a Git submodule)*
 * GLM *(included as a Git submodule)*
 * `libogg`
 * `libvorbis`
 * `libvorbisfile`
 * OpenAL
 * `tinyobjloader` *(included as a Git submodule)*
 * Vulkan

## Cloning

The repository contains Git submodules for some of the dependencies. To acquire them, use the `--recursive` flag while cloning the repository:

```git clone --recursive https://github.com/kbieganski/engine.git```

## Compilation

Make sure all dependencies are satisfied. To build the engine, run unit tests and compile the example, run `make release` or just `make` in the root directory. To build a debug build, use `make debug`.

If you only want to build the engine, which is a static library, run `make engine`, `make engine-release`, or `make engine-debug`.

To build the example app, run `make example`, `make example-release`, or `make example-debug`. To build and launch the example app, use `make example-run`, `exmaple-release-run`, or `example-debug-run`.

To build and run the unit tests, use `make test`. To only build the tests, use `make test-build`.

Run `make clean` to remove all build products and artifacts.
