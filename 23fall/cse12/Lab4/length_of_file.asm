length_of_file:
#function to find length of data read from file
#arguments: a1=bufferAdress holding file data
#return file length in a0

#Start your coding here"
#s3 = file descriptor from main
li t0, 0 #counter
LoopLengthOfFile:
	lbu t1, (a1)
	beqz t1, exit_LoopLengthOfFile
	
	addi t0, t0, 1
	addi a1, a1, 1
	
	j LoopLengthOfFile
	
exit_LoopLengthOfFile:
close_file(s3)

mv a0, t0

#if no student code provided, this function just returns 0 in a0

#End your coding here
	
	ret
#######################end of length_of_file###############################################	
