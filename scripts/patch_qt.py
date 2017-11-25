#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import mmap
import glob

def patch_binary_file(filename, src_path, dst_path):
    with open(filename, 'r+b') as f:
        mm = mmap.mmap(f.fileno(), 0)
        last_pos = -1
        while True:
            last_pos = mm.find(src_path, last_pos + 1)
            if last_pos == -1: break
            mm.seek(last_pos)
            mm.write(dst_path)
            len_diff = len(src_path) - len(dst_path)
            while len_diff > 0:
                mm.write('\x00')
                len_diff -= 1
        mm.flush()
        mm.close()

def patch_text_file(filename, src_path, dst_path):
    with open(filename, 'r+t') as f:
        txt_data = f.read()
        txt_data = txt_data.replace(src_path, dst_path)
        f.seek(0)
        f.write(txt_data)
        f.truncate(len(txt_data))

def get_source_path(qtcore_path):
    with open(qtcore_path, 'r+b') as f:
        mm = mmap.mmap(f.fileno(), 0)
        mm.seek(mm.find('qt_prfxpath') + 12)
        buf = mm.read(260)
        mm.close()
        return buf.strip('\x00')


def patch_qt(qt_path, dst_path):
    bindir = os.path.join(qt_path, "bin")
    libdir = os.path.join(qt_path, "lib")

    target_platform = 'unknown'
    if glob.glob(os.path.join(libdir, "*.dll")): target_platform = 'windows'
    if glob.glob(os.path.join(libdir, "*.framework")): target_platform = 'macos'
    if glob.glob(os.path.join(libdir, "*.so")): target_platform = 'linux'

    bin_files = []
    if target_platform == 'windows':
        bin_files.append(os.path.join(bindir, 'qmake.exe'))
        bin_files.append(os.path.join(libdir, 'Qt5Core.dll'))
    else:
        bin_files.append(os.path.join(bindir, 'qmake'))
        if target_platform == 'macos':
            bin_files.append(os.path.join(libdir, 'QtCore.framework', 'QtCore'))
        else:
            bin_files.append(os.path.join(libdir, 'libQt5Core.so'))

    txt_files = []
    txt_files.extend(glob.glob(os.path.join(libdir, '*.la')))
    txt_files.extend(glob.glob(os.path.join(libdir, 'pkgconfig', '*.pc')))

    src_path = get_source_path(bin_files[-1])
    for f in bin_files: patch_binary_file(f, src_path, dst_path)
    for f in txt_files: patch_text_file(f, src_path, dst_path)


def main():
    import argparse

    parser = argparse.ArgumentParser(description="Patch Qt files to make them usable at new place.")
    parser.add_argument('-d', '--dst', type=str, required=True, metavar='path',
                        help="Qt destination path (must be absolute)")
    parser.add_argument('-q', '--qt', type=str, default=os.getcwd(), metavar='path',
                        help="Qt root path to be patched. Current directory will be used if not specified.")

    args = parser.parse_args()

    qt_path = args.qt
    dst_path = args.dst
    if dst_path[-1] == os.path.sep: dst_path = dst_path[:-1]

    patch_qt(qt_path, dst_path)


if __name__ == '__main__': main()
