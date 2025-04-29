# Create a file with test input.
echo 3 > /tmp/lcr_input
echo testing >> /tmp/lcr_input

# Select the right binary to use as a reference.
lcr_ref=lcr_arm
if [ $(uname -m) = "x86_64" ]; then
    lcr_ref=lcr_x86
fi

# Run the reference binary with the test input and redirect the output and
# errors to separate files.
./$lcr_ref < /tmp/lcr_input > /tmp/ref_out 2> /tmp/ref_err
ref_returncode=$?

# Run your binary with the test input and redirect the output and errors to
# a separate file.
./lcr < /tmp/lcr_input > /tmp/my_out 2> /tmp/my_err
my_returncode=$?

# Compare what was printed to stdout.
diff /tmp/ref_out /tmp/my_out
if [ $? -ne 0 ]; then
    echo "standard out doesn't match!"
    exit 1
fi

# Compare what was printed to stderr.
diff /tmp/ref_err /tmp/my_err
if [ $?  -ne 0 ]; then
    echo "standard out doesn't match!"
    exit 1
fi

# Compare the return codes.
if [ $ref_returncode -ne $my_returncode ] ; then 
    echo "Return codes don't match!"
    exit 1
fi

# Everything matches as expected!
echo "Working Correctly"
exit 0
