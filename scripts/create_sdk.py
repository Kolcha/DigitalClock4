#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Script for clock SDK creating. Clock must be compiled and all needed paths
must specified before running this script.
"""

CLOCK_SRC_PATH = "~/src/DigitalClock4"
CLOCK_SDK_PATH = "~/src/DigitalClock_SDK"

SDK_LIBS = ["clock_common", "skin_draw"]


import glob, os, shutil

def find_builds(path):
    """
    Find folders with clock builds in given path. Returns list of full paths.
    """
    return glob.iglob(os.path.join(path, "build-DigitalClock-Desktop_Qt_5_*-Release"))


def parse_build(build_path):
    """
    Parse build path and extract build info: build type, compiler name, architecture.
    """
    parts = os.path.basename(build_path).split('-')
    btype = parts[-1].lower()
    parts = parts[-2].split('_')
    return parts[-2].lower(), parts[-1], btype


# copy headers
for lib in SDK_LIBS:
    src_path = os.path.join(CLOCK_SRC_PATH, lib)
    dst_path = os.path.join(CLOCK_SDK_PATH, "include", lib)
    if not os.path.exists(dst_path): os.makedirs(dst_path)
    files = glob.iglob(os.path.join(src_path, "*.h"))
    for f in files: shutil.copy(f, dst_path)

# find build dirs and copy libraries
folders = find_builds(os.path.dirname(CLOCK_SRC_PATH))
for f in folders:
    compiler, arch, build = parse_build(f)
    for lib in SDK_LIBS:
        src_path = os.path.join(f, lib) + os.sep
        if os.path.exists(src_path + build): src_path += build + os.sep
        dst_path = os.path.join(CLOCK_SDK_PATH, "lib", compiler + '-' + arch)
        if not os.path.exists(dst_path): os.makedirs(dst_path)
        # compiler specific code
        if compiler.find("mingw") != -1 or compiler.find("msvc") != -1:
            shutil.copy(src_path + lib + ".dll", dst_path)
            if compiler.find("mingw") != -1:
                shutil.copy(src_path + "lib" + lib + ".a", dst_path)
            else: # MSVC compiler
                shutil.copy(src_path + lib + ".lib", dst_path)
