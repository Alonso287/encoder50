# encoder50: A Base64 encoding and decoding tool
#### Video Demo:  [https://youtu.be/8_C2XhbPW6Q)](https://youtu.be/8_C2XhbPW6Q)
#### Description:
encoder50 is a command-line interface (CLI) application developed in C that implements the Base64 encoding and decoding algorithms from scratch.

This project is an example of dynamic memory management, low-level bit manipulation and dynamic memory management.

#### What's Base 64?
In modern computing, we often need to transmit binary data (like images or executable files) over channels that only support text (like email or JSON). Base64 solves this by representing any sequence of bytes using a limited set of 64 printable characters: `A-Z`, `a-z`, `0-9`, `+`, and `/`.

#### How does it work?
The process begins by breaking down the input string into its corresponding ASCII bytes. Each byte is converted into an 8-bit binary sequence through bitwise operations. For example, the letter `M` is represented as the decimal 77, which translates to 01001101 in binary.

Instead of reading the bits in groups of 8 (bytes), it reads them in groups of 6 bits (sextets). This shift is crucial because 2^6 equals 64, which matches the number of characters in the Base64 alphabet.

Each 6-bit group is then treated as a decimal index. The program looks up this index in the `B64_TABLE`, a constant string containing ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/. For example: if we have the bits 010011, the decimal value is 19, which corresponds to the character `T` in the table.

However, because the input (8-bit) and the output (6-bit) do not always align perfectly, the algorithm requires padding. If the total number of bits is not a multiple of 24 (the lowest common multiple of 8 and 6), the program adds extra zero bits to complete the last 6-bit group. To inform the decoder that these bits are not part of the original message, the output string is appended with one or two `=` symbols.

When decoding, the process is reversed. The padding symbols get removed (because they don't represent anything) and then each character is mapped back to its 6-bit index. These sextets are then reassembled into 8-bit bytes to recover the original text.

#### A look under the hood
When a user inputs data, the program deconstructs the hardware-level representation of those characters using bitwise operators like right-shift (`>>`) and bitwise AND (`&`) on the `char_to_8bits` function. This allows the algorithm to extract each bit's state and store it in a dynamically allocated integer array, turning a string into a binary map.

Then, the `base64_encode` function iterates through the input's bit array in 6-bit increments, using a temporary integer (`index`) to accumulate values, by shifting existing bits to the left (`<<`) and adding in new ones using the OR (`|`) operator.

On the decoding side, the procces is almost the same, but since Base64 strings often arrive with "noise" such as padding, the `sanitize_b64` function first "trims" the input string, placing a null terminator (`\0`) at the first instance of an equal sign to "clip" the string without losing the pointer to its original memory block. Finally, a loop ensures data integrity by only adding a character to the final output string if its full 8-bit sequence is available, filtering out the fractional "trailing bits" that are the result of the 6-to-8 bit conversion.

#### Usage:
To use the tool, compile it using `make` and run it from the terminal using the `encode` or `decode` flags:

```bash
./encoder50 encode
Input:  Man is distinguished, not only by his reason, but ...
Output: TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCAuLi4=
```

```bash
./encoder50 decode
Input:  TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCAuLi4=
Output: Man is distinguished, not only by his reason, but ...
```
