income_from_record:
#function to return numerical income from a specific record
#e.g. for record "Microsoft,34\r\n", income to return is 34(for which name is Microsoft)

#arguments:	a0 contains pointer to start of numerical income in record 

#function RETURNS income numerical value of the asci income in a0 (34 in our example)
	
# Start your coding from here!
lwu a0, 0(a0)
mv t1, a0 #save start of number
li t0, 13 #marks end of number (13 is CR in ASCII)
LoopToEndOFNumber:
	lb t2, 1(a0)
	beq t2, t0, exit_LoopToEndOFNumber
	addi a0, a0, 1
	j LoopToEndOFNumber
exit_LoopToEndOFNumber:
#a0 should now contain the pointer to the end of numerical income

stepBackwards:
	li t2, 0 #sum
	li t3, 1 #multiply by 10
	li t4, 10
sumIncomeFromRecord:

	lb t0, 0(a0)
	addi t0, t0, -48
	
	mul t0, t0, t3
	add t2, t2, t0
	
	mul t3, t3, t4 #increase t5 by factor of 10 to get the next digit

	
	beq t1, a0, exit_sumIncomeFromRecord #check if were at the start of the integer
	addi a0, a0, -1

	j sumIncomeFromRecord
		
exit_sumIncomeFromRecord:

	
	
	mv a0, t2 # income_from_record function
	#if no student code entered, a0 just returns 0 always :(
	
# End your  coding  here!
	ret
	
#######################end of income_from_record###############################################	
