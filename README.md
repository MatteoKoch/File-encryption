# File-encryption
This program encrypts files using a key given by the user

File encryption is an interesting topic.
This program uses a file that can be of any type and converts it into a ".mencpp" file.
From there the original file is only restorable if you have the program and the key.

You have to call the compiled programm from the terminal, like so:
./a.out -e file.txt UhKDcdF

The layout of the input is this:
./.out [-e/-d] <mencpp file> <key>

Use -e to encrypt and -d to decrypt.
Of course the same key has to be used to decrypt, that was used to encrypt the file.

The filename extension ".mencpp" comes from: 
M(atteo)en(cryption)cpp
The cpp because the program is written in C++. :)
