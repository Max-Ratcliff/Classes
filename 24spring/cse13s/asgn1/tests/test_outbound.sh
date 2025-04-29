#test for varying inputs:

#start tests for out of bounds integers
#TOO BIG tests:
# Creates the example output
echo TOO BIG > ex_out.txt

#test big inputs (513 2, 2 513, 513 513)
#test 1
# Runs the program
./calc 513 2 > out.txt 

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
    echo "not printing TOO BIG"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt

#test 2
# Runs the program
./calc 2 513 > out.txt 

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
    echo "not printing TOO BIG"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt


#test 3
# Runs the program
./calc 513 513 > out.txt 

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
    echo "not printing TOO BIG"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt


#test small inputs (-513 2, 2 -513, -513 -513)
#test 4
# Runs the program
./calc -513 2 > out.txt 

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
    echo "not printing TOO BIG"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt

#test 5
# Runs the program
./calc 2 -513 > out.txt 

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
    echo "not printing TOO BIG"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt


#test 6
# Runs the program
./calc -513 -513 > out.txt 

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
    echo "not printing TOO BIG"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt
rm ex_out.txt
#end out of bounds integer tests

echo all tests passed

exit 0
