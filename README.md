# baseCJK algorithm
a bin-to-text algorithm writen in C++ (with asm), which can map all bytes to CJK characters, working like BASE64

## How does it work?
1. the `CJK Unified Ideographs` defined by unicode table ranges from `0x4e00`(0b100111000000000) to `0x9fff`(0b1001111111111111), containing more than 2^14 items. so we can map every 14bits to a single CJK char. for convinience, the baseCJK uses the unicode chars value from `0x4e00` to `0x8e01`.
2. to encode bytes, join 7 bytes together into a 56 bits sequence and convert each 14 bits into individual number
3. add the number to 0x4e00 and print out the unicode character.
4. if the remaining bytes are not as much long to 7, padding with 0s to 14 bits.

+ *in fact, there is a special process against the ending 0s. just check out the code*
