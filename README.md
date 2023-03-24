# Huffman-Compressor-Decompressor with Password Protection

The compressor component of the tool compresses the input data and stores it as a series of bytes in a .bin file. The decompressor component first reads the key-value pair of the character-to-binary string and then decompresses the data byte by byte.

This tool also provides password protection for compressed files using RSA cryptography. During the compression process, the user can specify a numeric password (optional), which is used to prevent unauthorised access to the data even if it is stored in compressed form.

### How to run the code
1. Download and save all the files in the same folder.
2. Rename the input.txt file as per your wish
3. Execute these commands:
- g++ -o compression HuffmanCompression.cpp
- compression input.txt <numeric-password>
- g++ -o decompression decompression.cpp
- decompression input-compressed.bin <numeric-password>
4. Output will be displayed in input-decompressed.txt file.
