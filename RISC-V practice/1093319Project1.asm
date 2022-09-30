.globl	main
.data
	input:	.string "Input the number:\n"
   	output:	.string "output:\n"
.text

main:
	addi s2,x0,100#比大小用的數字
	addi s3,x0,20#s3=20
	addi s4,x0,10#s4=10
	add s5,x0,x0#final ans
	addi s9,x0,5#輔助計算用的
	addi s6,x0,1
	la a0,input#這邊開始是input
	li a7,4
	ecall
	li a7,5
	ecall
	add s7,a0,x0#這邊表s7=input
	jal ra,fun0
	la a0,output
	li a7,4
	ecall
	mv a0,s5
	li a7,1
	ecall
	j Exit
Exit:
	li a7,10
	ecall	
	
fun0:
	bge s7,s2,Exit#input>99 exit
	beq s7,s6,fun5#input==1
	beq s7,x0,fun4#input==0
	beq s7,s4,fun3#input==10
	beq s7,s3,fun2#input==20
	bge s7,s3,fun1#20<input
	bge s7,s4,fun2#10<input
	bge s7,s6,fun3#1<input
	blt s7,x0,fun6#else
	
fun1:#p > 20
	addi sp,sp -4#留出一個空間
	sw ra,0(sp)
	add t3,s7,x0
	slli t3,t3,1#2*p
	add s5,s5,t3
	div s7,s7,s9#p/5
	jal ra,fun0#go back to the fun0 => recursive
	lw ra,0(sp)
	addi sp,sp,4#把空間還回去
	jalr x0,0(ra)
	
fun2:#10 < p && p <= 20
	addi sp,sp,-8#給出兩個位置 因為傳入的值在這邊要用到兩次
	sw s7,0(sp)
	sw ra,4(sp)
	addi s7,s7,-2#p-2
	jal ra,fun0#go back to the fun0 => recursive
	lw s7,0(sp)#因為前面已經對s7中的值做過更改 所以要把它更新回來
	addi sp,sp,4#因為用不到了
	addi s7,s7,-3#p-3
	jal ra,fun0#go back to the fun0 => recursive
	lw ra,0(sp)
	addi sp,sp,4
	jalr x0,0(ra)
	
fun3:#1 < p && p <= 10
	addi sp,sp,-8
	sw s7,0(sp)
	sw ra,4(sp)
	addi s7,s7,-1#p-1
	jal ra,fun0#go back to the fun0 => recursive
	lw s7,0(sp)
	addi sp,sp,4
	addi s7,s7,-2#p-2
	jal ra,fun0#go back to the fun0 => recursive
	lw ra,0(sp)
	addi sp,sp,4
	jalr x0,0(ra)
	
fun4:#p == 0
	addi s5,s5,1
	jalr x0,0(ra)
	
fun5:#p == 1
	addi s5,s5,5
	jalr x0,0(ra)

fun6:#else
	addi s5,s5,-1
	jalr x0,0(ra)
	
	
	
	
	