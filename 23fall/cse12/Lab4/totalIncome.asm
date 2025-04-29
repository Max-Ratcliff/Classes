totalIncome:
#finds the total income from the file
#arguments:	a0 contains the file record pointer array location (0x10040000 in our example) But your code MUST handle any address value
#		a1:the number of records in the file
#return value a0:the total income (add up all the record incomes)

	#if empty file, return 0 for  a0
	bnez a1, totalIncome_fileNotEmpty # totalIncome function
	li a0, 0
	ret


totalIncome_fileNotEmpty:
	
	# Start your coding from here!
	mv a6, ra #save ra
	li t6, 0 #sum
	
	addi a0, a0, 4
	mv t5, a0
totalIncomeLoop:
	
	jal income_from_record   ## CALL income_from_record ##########################
	add t6, t6, a0
	addi a1,a1,-1  # count down
	addi t5,t5,8   # go to next income pointer (every other one)
	mv a0, t5
	bgtz a1, totalIncomeLoop
	
	
	mv a0, t6 #return the sum
	mv ra, a6 #restore ra
	#if no student code entered, a0 just returns 0 always :(
	
# End your  coding  here!
	
	ret
#######################end of nameOfMaxIncome_totalIncome###############################################
