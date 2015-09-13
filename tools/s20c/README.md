# Cimsys Developer Project Compiler interface (cc)

Author: Henrik Juvonen <heolju@gmail.com>

License: Apache License Version 2.0

## How to build

Make sure you have .NET Framework 4.0 or later version installed. This should be included in the newer Windows versions by default.

Run `build.cmd` in src directory or compile solution using Visual Studio and copy cc.exe from bin directory to `tools/s20c/` directory.

## How to run

Usage:
  -n <name>
  -i <include-dir> <include-dir> ...
  -f <source-file> <source-file> ...
  -o <object-dir>
  -r <root-dir>
  -d <preprocessor-definition> <preprocessor-definition> ...
  -u <ucc-path> (default: C:\Program Files (x86)\Cimsys\Cimsys Developer\bin\ucc)
  -p <profile-path> (default: C:\Program Files (x86)\Imsys Technologies\Profiles\IM3910-M210 v1.4.4)

The makefile in `platform/s20c` uses this tool and sets the arguments by itself. Therefore there shouldn't be any need to execeute this tool manually.
