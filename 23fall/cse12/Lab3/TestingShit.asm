.text
 	li a0, 5
        li a1, 10
        jal ra, func
        addi a2, a0, 3
        jal zero end
    func: 
        blt a0, a1, label
        addi a3, a0, 3
        jal zero return
    label: addi a3, a1, 2
    return:
    end:
    	li a7, 10
	ecall