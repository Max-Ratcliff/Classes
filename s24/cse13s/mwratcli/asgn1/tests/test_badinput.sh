#start tests for non-integer inputs
#test inputs (A 1, 1 A, BB BB, 5.5 3, 3 5.5, 5.5 5.5, - 4, 4 -, - -, 1+1 1, 1 1+1, 1+1 1+1)
# Creates the example output
echo BAD INPUT > ex_out.txt

#test 1: A 1
# Runs the program
./calc A 1 > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt


#test 2: 1 A
# Runs the program
./calc 1 A > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt


#test 3: BB BB
# Runs the program
./calc BB BB > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt


#test 4: 5.5 3
# Runs the program
./calc 5.5 3 > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt


#test 5: 3 5.5
# Runs the program
./calc 3 5.5 > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt


#test 6: 5.5 5.5
# Runs the program
./calc 5.5 5.5 > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

#test 7: - 4
# Runs the program
./calc - 4 > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

#test 8: 4 -
# Runs the program
./calc 4 - > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi


#test 9: - -
# Runs the program
./calc - - > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

#test 10: 1+1 1
# Runs the program
./calc 1+1 1 > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi

#test 11: 1 1+1
# Runs the program
./calc 1 1+1 > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output"
    rm out.txt
    rm ex_out.txt
    exit 1
fi


#test 12: 1+1 1+1
# Runs the program
./calc 1+1 1+1 > out.txt 

# Ensures exit code is not Zero
if [ $? -eq 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *are* found
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
