import os
import subprocess

cwd = os.getcwd()
buildPath = cwd + "/build"

exist = os.path.isdir(buildPath)

if not exist:
    os.mkdir(buildPath)

os.chdir(buildPath)

#for some reason, CMake cannot find glfw3 header file.   Maybe I can use SDL2......
subprocess.call("cmake -DCMAKE_TOOLCHAIN_FILE=E:/vcpkg/scripts/buildsystems/vcpkg.cmake -DFORCEGL=ON ..")