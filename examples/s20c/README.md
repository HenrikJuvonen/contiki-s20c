s20c
====

Author: Henrik Juvonen <heolju@gmail.com>

## Preparations

Before you can build the examples in this directory, you need to have three things ready:

### Cimsys Developer installed and IM3910-M210 v1.4.4 Profile installed

Expected locations:

- `C:\Program Files (x86)\Cimsys\Cimsys Developer`
- `C:\Program Files (x86)\Imsys Technologies\Profiles\IM3910-M210 v1.4.4`

### Unix utils included in your path

You also need to have win32 ports of unix utils such as make, sed, and sh. You may get these from various sources such as `ezwinports`, `gnuwin32`, `unxutils`, `msys-coreutils` and perhaps even `cygwin`. You will be notified of the missing files during the build process if you are missing files. Remember to put the binary path in your PATH environment variable.

### Compiler interface built

This can be found in `tools\s20c\`. Follow the instructions in `README.md` of that directory. In the end you should have `cc.exe` file in there.


## Building

Now, you can build all examples by writing

    make

or if you want specify the example, then you could write for instance:

    make test-cc2520

When you want to delete the object and project files, write:

    make clean


## Additional information

`cpu\fc3180\` contains files for FC3180 (or similar) microprocessor.

`platform\s20c\` contains files for S20C (or similar) platform.

See `contiki-conf.h`, `platform-conf.h`, and `contiki-main.c` in platform-directory. Stdout is being redirected to UART port 2, you might want to adjust this based on your needs. If you are using the sensors or relays found in the platform-directory, adjust the pin configuration based on your needs.

`core\sys\timer.c` had an issue with signed/unsigned integer value comparisons in timer_expired() method, which has been worked around by using `long long` values in it.

As you may have noticed, other `cpu`, `dev`, `examples`, `platform` and `tools` files are not included in this repository for the time being. Most of the `app` files are also not included. This is done just to keep the size of the repository small. In case you need any of those files, you may copy them from the original Contiki repository. Note that the files in `dev` directory seem to overlap with the ones in the `platform\s20c\dev` directory, which might cause compilation errors.
