"""
    0x4033E9 에 있는
    암호화 디코딩
"""
addr = 0x401000
for i in range(0x23DD) :
    data = get_wide_byte(addr+i)
    data ^= 0x9A
    ida_bytes.patch_byte(addr+i, data)
    
# 위 암호화 하는 주소 지우기
addr = 0x4033DD
for i in range(0x14) :
    ida_bytes.patch_byte(addr+i, 0x0)
   
# browse func 401994 ~ 401717까지 브레이크 및 코드 변조 있는지 체크
key = 0x5C1EE5D5
checksum = 0x401994
res = 0x401915
for i in range(0, 0x184) :
    for j in range(0, 0x27D) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data) # 0x401915에 데이터 입력
addr = 0x402657
ida_bytes.patch_dword(addr, 0xFFFFF147) # call 주소구하기 = 목적지 - 현재주소 - 명령어크기(5)

# browse button dec, enc delete
start_addr = 0x401718
end_addr = 0x4017A1

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x401919
end_addr = 0x4019A4

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
print("*** 401718_fucn End ***")


# 40269B_func DEC
key = 0x5E082AD5
checksum = 0x4029FD
res = 0x40297E
for i in range(0, 0x26B) :
    for j in range(0, 0x363) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
addr = 0x402667
ida_bytes.patch_byte(addr, 0xBA)
# IsDebuggerPresent del
addr = 0x4027EE
ida_bytes.patch_dword(addr, 0x90909090)
ida_bytes.patch_dword(addr+4, 0x90909090)
ida_bytes.patch_dword(addr+8, 0x90909090)
ida_bytes.patch_word(addr+12, 0x9090)

# 40269B enc, dec delete
start_addr = 0x40269B
end_addr = 0x402724

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x402982
end_addr = 0x402A10

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
print("40269B func End")


# 401F96_func
key = 0x5B1068ED
checksum = 0x402158
res = 0x4020D9
for i in range(0, 0x0CB) :
    for j in range(0, 0x1C3) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
addr = 0x402741
ida_bytes.patch_dword(addr, 0xFFFFF8DB)
# 암호화 주소 함수 있어서 바꿔준다.
addr = 0x402088
ida_bytes.patch_byte(addr, 0xE9)
ida_bytes.patch_dword(addr+1, 0x0000004C)

# 401F96 enc, dec delete
start_addr = 0x401F96
end_addr = 0x40201F

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1

start_addr = 0x4020DD
end_addr = 0x402167

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
print("401F96 func End")

# browse func 401994 ~ 401717까지 브레이크 및 코드 변조 있는지 체크
key = 0x5D3C0845
checksum = 0x401CD3
res = 0x401C54
for i in range(0, 0x234) :
    for j in range(0, 0x32C) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data) # 0x401915에 데이터 입력
addr = 0x40275F
ida_bytes.patch_dword(addr, 0xFFFFF2CF)

# 4019A8 dec, enc delete
start_addr = 0x4019A8
end_addr = 0x401A31

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x401C58
end_addr = 0x401CE2

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
print("4019A8 func End")

# 402CE1_func
key = 0x5BA133E9
checksum = 0x402F14
res = 0x402E95
for i in range(0, 0x13C) :
    for j in range(0, 0x234) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data) 
addr = 0x4027CD
ida_bytes.patch_word(addr, 0x59A)

# 402CE1 dec, enc delete
start_addr = 0x402CE1
end_addr = 0x402D6A

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x402E99
end_addr = 0x402F27

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
print("402CE1 func End")

# 402BAE_func
key = 0x5A9B74A1
checksum = 0x402CCD
res = 0x402C4E
for i in range(0, 0x28) :
    for j in range(0, 0x120) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
addr = 0x402D78
ida_bytes.patch_dword(addr, 0xFFFFFEB9)
addr = 0x402E09
ida_bytes.patch_dword(addr, 0xFFFFFE28)

# 402BAE dec, enc delete
start_addr = 0x402BAE
end_addr = 0x402C34

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x402C52
end_addr = 0x402CDC

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
print("402BAE func End")

# 402BAE_func
key = 0x5A9B74A1
checksum = 0x402CCD
res = 0x402C4E
for i in range(0, 0x28) :
    for j in range(0, 0x120) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
addr = 0x402D78
ida_bytes.patch_dword(addr, 0xFFFFFEB9)
addr = 0x402E09
ida_bytes.patch_dword(addr, 0xFFFFFE28)

# 402BAE dec, enc delete
start_addr = 0x402BAE
end_addr = 0x402C34

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x402C52
end_addr = 0x402CDC

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
print("402BAE func End")

# 402F28_func
key = 0x5A9B6319
checksum = 0x403047
res = 0x402FC8
for i in range(0, 0x28) :
    for j in range(0, 0x120) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
addr = 0x402801
ida_bytes.patch_dword(addr, 0x7AA)

# 402F28 dec, enc delete
start_addr = 0x402F28
end_addr = 0x402FAE

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x402FCC
end_addr = 0x403056

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
print("402F28 func End")# 402F28_func
key = 0x5A9B6319
checksum = 0x403047
res = 0x402FC8
for i in range(0, 0x28) :
    for j in range(0, 0x120) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
addr = 0x402801
ida_bytes.patch_dword(addr, 0x7AA)

# 402F28 dec, enc delete
start_addr = 0x402F28
end_addr = 0x402FAE

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x402FCC
end_addr = 0x403056

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
print("402F28 func End")

# 40305B_func
key = 0x5AD47107
checksum = 0x4031D3
res = 0x403154
for i in range(0, 0x81) :
    for j in range(0, 0x179) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
# 콜 주소가 바뀌었으니 변경 해줍니다.
addr = 0x40280D
ida_bytes.patch_dword(addr, 0x8D1)

addr = 0x402816
ida_bytes.patch_dword(addr, 0x8C8)

addr = 0x40281F
ida_bytes.patch_dword(addr, 0x8BF)

# 40305B dec, enc delete
start_addr = 0x40305B
end_addr = 0x4030E1

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1

start_addr = 0x403158
end_addr = 0x4031E3

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
  
print("40305B func End")

# 402A66_func 
key = 0x5AA57891
checksum = 0x402B9A
res = 0x402B1B
for i in range(0, 0x3D) :
    for j in range(0, 0x135) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
addr = 0x402835
ida_bytes.patch_dword(addr, 0x2B4)

# 402A66 dec, enc delete
start_addr = 0x402A66
end_addr = 0x402AEC

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x402B1F
end_addr = 0x402BAA

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
print("402A66 func End")

# 4031E7_func
key = 0x5AD05827
checksum = 0x403362
res = 0x4032E3
for i in range(0, 0x84) :
    for j in range(0, 0x17C) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
# call 주소가 변경되니 변경된 주소로 바꿔줍시다.
addr = 0x40284C
ida_bytes.patch_dword(addr, 0xA1E)

addr = 0x40285D
ida_bytes.patch_dword(addr, 0xA0D)

addr = 0x40286E
ida_bytes.patch_dword(addr, 0x9FC)

# 4031E7 dec, enc delete 
start_addr = 0x4031E7
end_addr = 0x40326D

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x4032E9
end_addr = 0x403372

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
print("4031E7 func End")

# 401CE7_func
key = 0x5AC0AE57
checksum = 0x401E47
res = 0x401DC8
for i in range(0, 0x69) :
    for j in range(0, 0x161) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
addr = 0x402890
ida_bytes.patch_dword(addr, 0xFFFFF4DA)

# 401CE7 dec, enc delete

start_addr = 0x401CE7
end_addr = 0x401D6D

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x401DCC
end_addr = 0x401E57

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
print("401CE7 func End")

# 401E5B_func
key = 0x5A9F2F87
checksum = 0x401F82
res = 0x401F03
for i in range(0, 0x30) :
    for j in range(0, 0x128) : # 코드 변조됬으면 다른값이 나와서 브레이크걸면 안된다
        data = ida_bytes.get_wide_byte(checksum-j)
        key ^= data
        key -= data
    data = get_wide_dword(res-i)
    data -= key
    data ^= key
    ida_bytes.patch_dword(res-i, data)
    
# 401E5B dec,enc delete

start_addr = 0x401E5B
end_addr = 0x401EE1

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
start_addr = 0x401F07
end_addr = 0x401F92

while start_addr <= end_addr :
    ida_bytes.patch_byte(start_addr, 0x0)
    start_addr += 1
    
print("401E5B func End")

print("*** xchg    eax, ds ***")
print("*** 바이트 찾기 시작 ***")
addr = ida_ida.inf_get_min_ea() # 시작 주소
find_str = "xchg    eax, d"
cnt = 1
while(addr < ida_ida.inf_get_max_ea()):
    addr = idc.find_text(addr, SEARCH_DOWN | SEARCH_NEXT,0,0, find_str)
    if addr != idc.BADADDR :
        print("%d addr\tasm" % cnt)
        print(hex(addr), idc.GetDisasm(addr))
        ida_bytes.patch_byte(addr, 0xE9) # jmp
        ida_bytes.patch_byte(addr+1, 0x10) # 0으로 채운자리 넘어가는 jmp 문
        for i in range(2,21) :
            ida_bytes.patch_byte(addr+i, 0x0) # 주소의 바이트를 0으로 변경
        cnt+= 1
print("*** 바이트 패치 끝 ***")
