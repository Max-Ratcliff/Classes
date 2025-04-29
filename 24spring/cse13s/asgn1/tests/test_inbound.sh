#start tests for inbound integers
#test inputs (512 2, -512 2, 512 -512, 0 0)
#test 1: 512 2
# Creates the example output
echo 514 > ex_out.txt
# Runs the program
./calc 512 2 > out.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *arent* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output "
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt
rm ex_out.txt

#test 2: 2 512
# Creates the example output
echo 514 > ex_out.txt
# Runs the program
./calc 2 512 > out.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *arent* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output "
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt
rm ex_out.txt


#test 3: 512 512
# Creates the example output
echo 1024 > ex_out.txt
# Runs the program
./calc 512 512 > out.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *arent* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output "
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt
rm ex_out.txt

#test 4: -512 -512
# Creates the example output
echo -1024 > ex_out.txt
# Runs the program
./calc -512 -512 > out.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *arent* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output "
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt
rm ex_out.txt



#test 5: 512 -512
# Creates the example output
echo 0 > ex_out.txt
# Runs the program
./calc 512 -512 > out.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *arent* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output "
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt
rm ex_out.txt


#test 6: 0 0
# Creates the example output
echo 0 > ex_out.txt
# Runs the program
./calc 0 0 > out.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *arent* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output "
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt
rm ex_out.txt


#test 7: +10 +10
# Creates the example output
echo 20 > ex_out.txt
# Runs the program
./calc +10 +10 > out.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Ensures differences *arent* found
diff out.txt ex_out.txt
if [ $? -ne 0 ]; then
    echo "wrong output "
    rm out.txt
    rm ex_out.txt
    exit 1
fi

# Cleans up files created
rm out.txt
rm ex_out.txt

echo all tests passed
exit 0
