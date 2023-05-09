> git@github.com:w1bb/Wi-Crypt.git

# Wi-Crypt
An object-oriented C++ implementation of the AES algorithm, allowing for 128, 192 and 256 bit encryption using ECB, CBC, CFB, OFB and CTR modes. Currently, the implementation only works for Linux.

Here are some of the key features:

* 128, 192 and 256-bit encryption (decryption) support;
* ECB, CBC, CFB, OFB and CTR encryption (decryption) modes;
* GF(2^8) precomputations used instead of meaningless lookup tables;
* Tests against other known implementations, such as dhuertas' and SergeyBel's in order to verify correctness;
* Logging capabilities;
* Great code modularization.

## License

The code is availabe under the MIT license.
