text =[
    [0x1c, 0x22, 0x2a, 0x22, 0x1c, 0x00]       
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


# om_0 = list()
# for i in range(1, 20):
#     om_0.append(i*5)
# # print(om_0)


# n = list()

# for j in om_0:
#     n.append(round(360 * 62500 / 4096 / j))
    
    
# v = list()
# for i in n:
#     v.append(round(360 * 62500 / 4096 / i, 3))    


# for i in range(len(v)):
#     # print(f"{i+1}-> v = {v[i]}; N = {n[i]}")
#     print(f"{v[i]},")
# # print(n)