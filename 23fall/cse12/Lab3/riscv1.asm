.data

main:
  # Prompt the user to enter the height of the triangle
  li a0, SYS_WRITE
  li a1, STDOUT_FD
  la a2, height_prompt
  li a3, height_prompt_length
  ecall

  # Read the user input for the triangle height
  li a0, SYS_READ
  li a1, STDIN_FD
  la a2, input_buffer
  li a3, input_buffer_size
  ecall

  # Validate the user input
  la a0, input_buffer
  addi a1, a0, input_buffer_size
  call validate_input

  # Generate the triangle pattern
  la a0, input_buffer
  call generate_triangle

  # Exit the program
  li a0, SYS_EXIT
  ecall

height_prompt:
  .string "Enter the height of the triangle: "
height_prompt_length:
  .size height_prompt

input_buffer:
  .space 4

input_buffer_size:
  .size input_buffer

error_message:
  .string "Invalid input. Please enter a positive integer.\n"
error_message_length:
  .size error_message

validate_input:
  # Check if the input is a positive integer
  # Convert the input string to an integer and store it in register t0
  la a0, input_buffer
  addi a1, a0, input_buffer_size
  call parse_integer

  # Check if t0 is less than or equal to 0
  slt t1, t0, 1
  bne t1, x0, print_error_message

  # If the input is valid, return to the main function
  jr ra

print_error_message:
  # Print the error message
  li a0, SYS_WRITE
  li a1, STDOUT_FD
  la a2, error_message
  li a3, error_message_length
  ecall

  # Return to the main function to prompt for input again
  jr ra

generate_triangle:
  # Load the height of the triangle from register t0 into register t1
  mv t1, t0

  # Generate the triangle pattern row by row
  loop:
    # Print the blank spaces for the current row
    li t2, 0
    loop2:
      li a0, SYS_WRITE
      li a1, STDOUT_FD
      la a2, blank_space_character
      li a3, blank_space_character_length
      ecall

      addi t2, t2, 1
      bne t2, t1, loop2

    # Print a single star
    li a0, SYS_WRITE
    li a1, STDOUT_FD
    la a2, star_character
    li a3, star_character_length
    ecall

    # Print the blank spaces for the remaining part of the row
    li t2, t0
    addi t2, t2, -2
    loop3:
      li a0, SYS_WRITE
      li a1, STDOUT_FD
      la a2, blank_space_character
      li a3, blank_space_character_length
      ecall

      addi t2, t2, -1
      bne t2, -1, loop3

    # Print a newline character
    li a0, SYS_WRITE
    li a1, STDOUT_FD
    la a2, newline_character
    li a3, newline_character_length
    ecall

    # Decrement the height of the triangle
    addi t1, t1, -1
    bne t1, 0, loop

  # Return to the main function
  jr ra

star_character:
  .byte 0x2a

star_character_length:
  .size star_character

blank_space_character:
  .byte 0x20

blank_space_character_length:
  .size blank_space_character

newline_character:
  .asciz  "\n"
