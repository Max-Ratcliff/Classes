#start tests for only 1 inputs
#test inputs (1, )
# Creates the example output
echo NOT ENOUGH INPUT > ex_out.txt

#test 1: 1
# Runs the program
./calc 1 > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *arent* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt

#test 2: 
# Runs the program
./calc > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *arent* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt
rm ex_out.txt
echo all tests passed
exit 0

