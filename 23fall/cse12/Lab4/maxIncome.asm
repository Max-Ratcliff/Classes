maxIncome:
#finds the total income from the file
#arguments:	a0 contains the file record pointer array location (0x10040000 in our example) But your code MUST handle any address value
#		a1:the number of records in the file
#return value a0: heap memory pointer to actual  location of the record stock name in the file buffer

	#if empty file, return 0 for both a0, a1
	bnez a1, maxIncome_fileNotEmpty # maxIncome Function
	li a0, 0
	ret

 maxIncome_fileNotEmpty:
	
	# Start your coding from here!
	mv a6, ra
	li t6, 0 #max num
	li a5, 0 #max register
	addi a0, a0, 4
	mv t5, a0
	
maxIncomeLoop:
	
	jal income_from_record   ## CALL income_from_record ##########################
	bgt t6, a0 skipdacode
		mv t6, a0
		addi a5, t5, -4
	skipdacode:
	addi a1,a1,-1  # count down
	addi t5,t5,8   # go to next income pointer (every other one)
	mv a0, t5
	bgtz a1, maxIncomeLoop
	
	
	mv a0, a5
	mv ra, a6
	#if no student code entered, a0 just returns 0x10040010 always :(
	
# End your  coding  here!
	
	ret
#######################end of maxIncome###############################################
