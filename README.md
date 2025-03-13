# atmega2560 emulator

This is a minimalistic software emulator for the atmega2560 microcontroller, which can decode and run .hex files, containing machine code for the atmega2560, as if they were flashed and run on the actual processor.

**Note:** It's currently unfinished, only supporting a single instruction.

## Building from source
Ensure a C compiler (clang, gcc, etc) and GNU make are installed on your system. 
By default, the Makefile will use clang. You can change this, however, by changing the CC variable within the Makefile.
```sh
git clone https://github.com/Loravis/atmega2560emulator
cd atmega2560emulator
make
```

## Usage
```sh
./emul2560 <file>
```

You must provide the program with a .hex file, formatted in the Intel HEX format. You may find an example in the examples directory.