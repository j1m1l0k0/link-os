	.file	"main.c"
	.section	".data"
	.align 2
	.type	buffer, @object
	.size	buffer, 18
buffer:
	.string	"string in buffer\n"
	.section	".text"
	.align 2
	.globl test
	.type	test, @function
test:
	stwu 1,-32(1)
	stw 31,28(1)
	mr 31,1
	stw 3,8(31)
	li 0, 4
	sc
	li 0,0
	mr 3,0
	lwz 11,0(1)
	lwz 31,-4(11)
	mr 1,11
	blr
	.size	test, .-test
	.globl buf
	.section	".data"
	.align 2
	.type	buf, @object
	.size	buf, 9
buf:
	.string	"buf test"
	.section	".text"
	.align 2
	.globl main
	.type	main, @function
main:
	stwu 1,-16(1)
	mflr 0
	stw 31,12(1)
	stw 0,20(1)
	mr 31,1
	li 3,48
	bl test
	li 0,0
	mr 3,0
	lwz 11,0(1)
	lwz 0,4(11)
	mtlr 0
	lwz 31,-4(11)
	mr 1,11
	blr
	.size	main, .-main
	.ident	"GCC: (GNU) 3.4.6"
