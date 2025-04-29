module read_initFile
    ! Module with subroutines that are able to read in different data types from a 
    ! text file. The format of each line of the text file should be as follows:
    !   # comments
    !   var_name1 3.2 # comments
    !   var_name2 3 # comments
    !   var_name3 a_string # comments
    ! for real, integer, and character datatypes respectively

    use utility, only : fp, maxStrLen

    implicit none

contains

    function read_initFileInt(file_name, var_name) result(var_value)
        ! function:     read_initFileInt
        ! Author:       Sean Riedel
        ! purpose:      To read in a variable from a .init file
        ! 
        ! Inputs:       - file_name (character) name of file to read
        !               - var_name (character) name of variable as written in .init file
        !               
        ! Outputs:      - var_value (integer) value of variable with name var_name
        ! ------------------------------------------------------------
        implicit none
        character(len=*), intent(in) :: file_name, var_name
        integer :: var_value
        ! local variables
        character(len=100) :: var_value_char
        
        call read_value_string(file_name, var_name, var_value_char)
        read(var_value_char,*) var_value
    
    end function read_initFileInt

    function read_initFileReal(file_name, var_name) result(var_value)
        ! function:     read_initFileReal
        ! Author:       Sean Riedel
        ! purpose:      To read in a real type variable from a text file
        ! 
        ! Inputs:       - file_name (character) name of file to read
        !               - var_name (character) name of variable as written in .init file
        !               
        ! Outputs:      - var_value (double) value of variable with name var_name
        ! ------------------------------------------------------------
        implicit none
        character(len=*), intent(in) :: file_name, var_name
        real(kind=fp) :: var_value
        ! local variables
        character(len=100) :: var_value_char
        
        call read_value_string(file_name, var_name, var_value_char)
        read(var_value_char,*) var_value

    end function read_initFileReal
    
    function read_initFileChar(file_name, var_name) result(var_value)
        ! function:   read_initFileChar
        ! Author:       Sean Riedel
        ! purpose:      To read in a variable from a .init file
        ! 
        ! Inputs:       - file_name (character) name of file to read
        !               - var_name (character) name of variable as written in .init file
        !               
        ! Outputs:      - var_value (character) value of variable with name var_name
        ! ------------------------------------------------------------
        implicit none
        character(len=*), intent(in) :: file_name, var_name
        character(len=maxStrLen) :: var_value
        ! local variables
        character(len=maxStrLen) :: var_value_char
        
        call read_value_string(file_name, var_name, var_value_char)
        read(var_value_char,*) var_value
    
    end function read_initFileChar

    function read_initFileLogical(file_name, var_name) result(var_value)
        ! function:   read_initFileLogical
        ! Author:       Sean Riedel
        ! purpose:      To read in a variable from a .init file
        ! 
        ! Inputs:       - file_name (character) name of file to read
        !               - var_name (character) name of variable as written in .init file
        !               
        ! Outputs:      - var_value (logical) value of variable with name var_name
        ! ------------------------------------------------------------
        implicit none
        character(len=*), intent(in) :: file_name, var_name
        logical :: var_value
        ! local variables
        character(len=100) :: var_value_char
        
        call read_value_string(file_name, var_name, var_value_char)
        read(var_value_char,*) var_value
    
    end function read_initFileLogical

    subroutine read_value_string(file_name, var_name, var_value_char)
        ! subroutine:   read_value_string
        ! Author:       Sean Riedel
        ! purpose:      to return the string that contains the relevant variable from 
        !               an init file
        ! 
        ! Inputs:       - file_name (character) name of file to read
        !               - var_name (character) name of variable as written in .init file
        !               
        ! Outputs:      - var_value_char (character) string containing the value of the
        !                 variable specified by var_name
        ! ------------------------------------------------------------
        implicit none
        character(len=*), intent(in) :: var_name, file_name
        character(len=*), intent(out) :: var_value_char
        ! local variables
        integer :: input_status, open_status
        character(len=maxStrLen) :: dummy_var_name, line

        open(unit=10, file=file_name, status='old', IOSTAT=open_status, FORM='formatted', ACTION='read')
        do
            read(10, FMT = 100, IOSTAT = input_status) line ! read the current line of the file into the variable line
            if (line(1:1)/='#') then ! if first character is not a comment character
                if (index(line,'#') > 0) then ! if there is another comment character somewhere else
                    line = trim(line(1:index(line,"#")-1)) ! redefine line to exclude the portion after the #
                end if
                dummy_var_name = trim(line(1:index(line,' ')-1))
                if (dummy_var_name==var_name) then 
                    var_value_char = line(index(line,' ')+1:)
                    exit
                end if
            end if
            ! if the parameter wasn't read in
            if (input_status<0) then
                print *, "====================================================================================="
                print *, "UNSUCCESFUL INPUT FILE READ." 
                print *, "The parameter            '", trim(var_name), "'"
                print *, "was not found inside of  '", trim(file_name), "'"
                print *, "---------------------------------------------------------------------------------"
                print *, "Make sure the parameter is present in the input"
                print *, "file and is spelled correctly."
                print *, "====================================================================================="
                print *, " "
                stop
            end if
        end do


        close(10)
        100 format(A)
        
    end subroutine read_value_string
        
end module read_initFile
