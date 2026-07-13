# LSB Steganography

A C-based LSB Steganography project that securely hides and extracts secret text messages in BMP images using bitwise operations. It supports encoding and decoding while preserving image quality.

## Features
- Encode secret text into BMP images
- Decode hidden messages
- Bitwise LSB implementation
- Command-line interface

## Technologies
- C Programming
- GCC
- Linux
- File Handling
- Bitwise Operations

## How to Run

Compile:
```bash
gcc *.c -o steganography
```

Encode:
```bash
./steganography -e beautiful.bmp secret.txt output.bmp
```

Decode:
```bash
./steganography -d output.bmp
```
