minIncome:
#finds the total income from the file
#arguments:	a0 contains the file record pointer array location (0x10040000 in our example) But your code MUST handle any address value
#		a1:the number of records in the file
#return value a0:the total income (add up all the record incomes)

	#if empty file, return 0 for both a0, a1
	bnez a1, minIncome_fileNotEmpty # minIncome function
	li a0, 0
	ret

 minIncome_fileNotEmpty:
 	mv a6, ra
	li t6, -1 #min num
	li a5, 0 #min register
	addi a0, a0, 4
	mv t5, a0
	
minIncomeLoop:
	
	jal income_from_record   ## CALL income_from_record ##########################
	bltu t6, a0 skipdacode2
		mv t6, a0
		addi a5, t5, -4
	skipdacode2:
	addi a1,a1,-1  # count down
	addi t5,t5,8   # go to next income pointer (every other one)
	mv a0, t5
	bgtz a1, minIncomeLoop
	
	
	mv a0, a5
	mv ra, a6
	

	ret
#######################end of minIncome###############################################
