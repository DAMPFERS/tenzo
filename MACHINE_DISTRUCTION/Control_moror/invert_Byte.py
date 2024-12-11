text =[
    [0x7e, 0x85, 0x99, 0xa1, 0x7e, 0x00], 
    [0x21, 0x61, 0xff, 0x01, 0x01, 0x00], 
    [0x43, 0x85, 0x89, 0x91, 0x61, 0x00],
    [0x89, 0x89, 0x89, 0x89, 0x7e, 0x00], 
    [0xf0, 0x10, 0x10, 0x10, 0xff, 0x00], 
    [0xf1, 0x89, 0x89, 0x89, 0xc6, 0x00], 
    [0x7e, 0x89, 0x89, 0x89, 0xc6, 0x00], 
    [0xc0, 0x80, 0x8f, 0x90, 0xe0, 0x00], 
    [0x76, 0x89, 0x89, 0x89, 0x76, 0x00], 
    [0x72, 0x89, 0x89, 0x89, 0x7e, 0x00]       
]




text_in = text.copy()


for j in range(len(text)):
    for i in range(len(text[0])):
        a = text[j][i]
        b = 0
        for k in range(8):
            b = b<<1
            b += a%2
            a = a>>1
        text_in[j][i] = b


for i in text_in:
    for j in i:
        print(hex(j), end=', ')
    print(' ')
