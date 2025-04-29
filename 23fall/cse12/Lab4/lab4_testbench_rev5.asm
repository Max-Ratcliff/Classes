
# address of file buffer: 0xffff0000 (MMIO)
# address of file record pointers(for location of starting points 
#    of stock name and income): 0x10040000 (heap)

# URGENT: make sure your csv file follows the Windows csv format:
# Windows uses a carriage return (\r) followed by line feed: (\n)
#   versus Linux or Mac: which ends their lines with a line feed.
# So lines should be like this:
#   "Facebook,56\r\nApple,100"

.include "macros_rev2.asm"
# macros currently can only affect the state of: a0,a1,a2,a3,a4,a7

.data
	filePath: .asciz "data.csv" 
	fileBeginPrompt: .asciz "Printing file contents...\n========================\n"
	fileEndPrompt: .asciz "========================\n"
	sizeFilePrompt: .asciz "Size of file data (in bytes): " 
	totalPrompt: .asciz "Total income garnered from all stocks: $"
	maxPrompt: .asciz "Stock name with maximum income:"
	minPrompt: .asciz "Stock name with minimum income:"
	newline: .asciz " \n" 	
	income_recordPrompt: .asciz "income records: "
.text
        #####################################
        #   Open and Read .CSV file contents
        #####################################
	li s4, 0x10040000  # ******** constant location for array of pointers to data.  
	#open file as read
	file_open_for_read(filePath)
	# a0 has file descriptor (identifier #).
	mv s3, a0   # save file descriptor, just in case.
	fileRead(a0, 0x0ffff0000) # read from file
	# returns buffer address 0x0fff0000 in a1
	mv s2, a1 # save the 0x0ffff0000 in s2
        # copying register to register is much more efficient
	# than an li of a large number, 1 instruction vs. many.

	# print entire file, i.e. text stored at a1.
	print_str(fileBeginPrompt) # affects a0,a7
	print_file_contents(a1)
	print_str(fileEndPrompt)

	# find length of file (size of file data:)
	print_str(sizeFilePrompt)
	mv a1, s2 # 0x0ffff0000 is param for use by length_of_file function
	jal length_of_file # CALL student length_of_file
	mv s3, a0  # resulting length of file.
	print_Int(a0) # print returned length of file, no need to save it.
	print_str(newline)
	mv a0, s3 # length of file parameter and
	mv a1, s2 # 0x0ffff0000 param for allocate_file_record_pointers ###########1
	jal allocate_file_record_pointers
	mv s1, a0  # s1 = no. of records preserved in s1.

	# test income_record
	print_str(income_recordPrompt)  # income_record 
	li s7,0x20  # space
	mv s5,s1 # start record down counter in t1
	mv s6,s4 # 0x10040000  # start initial record pointer
	addi s6,s6,4           # initial income pointer
next_record:
	# lwu a0, 0(t2) # get location of income 
	mv a0, s6
	jal income_from_record   ## CALL income_from_record ##########################
	print_Int(a0)  # print read value
	print_character(s7)  # print a space
	addi s5,s5,-1  # count down
	addi s6,s6,8   # go to next income pointer (every other one)
	bgtz s5, next_record
	crlf

	# find total income
	print_str(totalPrompt)  # total income prompt
	mv a0, s4 # 0x10040000
	mv a1, s1   # no. of records in file
	jal totalIncome # CALL student totalIncome
	print_Int(a0)	# print returned total income
	print_str(newline)
	
	# Find stock with max income
	mv a0,s4  # 0x10040000
	mv a1, s1  # no. of records in file
	jal maxIncome # CALL student maxIncome
	# returns in a0 address of pointer record with max income stock
	print_record(maxPrompt)

	# Find stock with min income
	mv a0,s4  # 0x10040000
	mv a1, s1  #no. of records in file
	jal minIncome  # CALL student minIncome
	# returns in a0 address of pointer record with min income stock
	print_record(minPrompt)

	exit
	
#######################end of main###############################################
.include "allocate_file_record_pointers.asm"
.include "income_from_record.asm"
.include "length_of_file.asm"
.include "totalIncome.asm"
.include "maxIncome.asm"
.include "minIncome.asm"

