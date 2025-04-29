
.macro print_str(%str)
        # destroys a7 and a0
	li a7, 4  # print_str macro
	la a0, %str
	ecall
.end_macro

.macro print_character(%charRegister)
	li a7, 11 # print_character macro
	addi a0, %charRegister, 0
	ecall
.end_macro

.macro  crlf
        # destroys a7,a0
        .eqv CR 13
        .eqv LF 10
        li a1, CR 
        print_character(a1)
        li a1, LF
        print_character(a1)
.end_macro

.macro print_Int(%IntRegister)
	li a7, 1  # print_Int macro
	addi a0, %IntRegister, 0
	ecall
.end_macro

.macro print_int_hex(%IntRegister)
	li a7, 34  # printIntHex macro
	addi a0, %IntRegister, 0
	ecall
.end_macro

.macro 	file_open_for_read(%str)
	la a0, %str # file_open_for_read macro
	li a1, 0
	li a7, 1024
	ecall
.end_macro

.macro fileRead(%file_descriptor_register, %file_buffer_address)
#macro reads upto first 10,000 characters from file
	addi a0, %file_descriptor_register, 0  # fileRead macro
	li a1, %file_buffer_address
	li a2, 10000
	li a7, 63
	ecall
.end_macro 

.macro print_file_contents(%ptr_register)
	li a7, 4  # print_file_contents macro
	addi a0, %ptr_register, 0
	ecall
	#entire file content is essentially stored as a string
.end_macro
	
.macro close_file(%file_descriptor_register)
	li a7, 57  # close_file macro
	addi a0, %file_descriptor_register, 0
	ecall
.end_macro

.macro exit
	li a7, 10  # exit macro
	ecall
	# the end no need to restore anything.
.end_macro

.macro pushw(%reg)
	addi sp, sp, -4
	sw  %reg, 0(sp)
.end_macro

.macro popw(%reg)
       lwu %reg, 0(sp)
       addi sp, sp, 4
.end_macro

.macro print_record(%prompt)
	# %prompt address of prompt to print
	# a0: pointer of a record company name
        # leaf function, nothing needs to be preserved.
	# uses: a2,a3,a4,  returns nothing.
	nop # print_record #####################
	mv   a2, a0
	print_str(%prompt) # affects a0, and a7
	addi a3, a2, 4
	lwu a2, 0(a2)    # get address of company name
	lwu a3, 0(a3)    # get address of income
	addi a3, a3, -1  # a3 contains address of last char in stock name

charPrintLoop:	
	beq a2, a3, charPrintLoopExit
	lbu a4, 0(a2)  # get char
	#loading byte at address in a2 into a4
	print_character(a4) 
	# increment char index
	addi a2, a2, 1	
	j charPrintLoop		
charPrintLoopExit:
	print_str(newline)
.end_macro

