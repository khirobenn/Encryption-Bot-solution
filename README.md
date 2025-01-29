# Encryption-Bot-solution
This is my solution for the Encryption Bot CTF on Hack the box.

1 - First of all we unzip the file we downloaded. As we can see, we have a directory that contains two files; "chell" and "flag.enc".
    If we use file command on the files we can see that "chall" is a 64-bit executable file, and "flag.enc" is just an ASCII text that contains a string of 36 charcacters.

    When we execute "chell", it asks us to put a string with a specific length that we don't know. So here, as we don't have the source code, we can think to decompile the binary file "chell", and that to know the length we have to put and other stuffs.
    We will use "GHIDRA" to decompile the binary file.

2 - Now that we decompiled the file, so we have the source code.

    - If we search in the functions one by one, we can see that the function named "FUN_0010163e" is like the main function, we will rename it to "main".
    So in the main function, after putting the string, we can see that there is a call of the function "FUN_001015dc" with the string as a parameter.
    
    - We are going to see this function that was called. We can see that he is putting the length of the string on a variable and he is comparing this variable to the value "0x1B" which is 27 in decimal.
    So the length of the string we can put is 27. If it's not 27 it exits.

    - After that he calls another function named "FUN_0010128a", we can see it does nothing. So we can ignore it.

    - He calls another function named "FUN_0010131d" with the string as a parameter. 
    We can see on the code that he is doing a while loop that stops if the variable 'uVar' equals 'sVar' (since uVar starts in 0).
    And for each character of the string he is calling the function "FUN_001011d9" with the character in the parameter and an array of 2000.

    - We are going to see what is he doing with each character.
    We go to the code of "FUN_001011d9", we can see that he is creating a file named "data.dat" and he is putting the binary value of the character passed in the file created.
    ! Note that : He is doing a for loop of 8 iterations beacause a character is coded in 8 bits.
    For example if we put "H......" we will have on our file "01001000.........".
    If we put "HT....." we will have on our file "'01001000''01010100'.........".
    
    So if for each character he is doing this we will have 27*8 = 216 characters of "0" and "1".

    - After doing this for each character, he is calling a function named "FUN_0010129f" that does nothing. We can ignore it.

    *- So now that we have our file "data.dat" of "0" and "1" characters, he is calling another function named "FUN_001014ba".
        As we can see, he is reading from the file "data.dat".
        He is doing a loop that starts from 1 to "0xd9" which is 217 in decimal. So he is doing '216 - 1 + 1 = 216' iterations (217 is not inclueded that why we stoped in 216).
    
    - We can understand by this loop that he is going to read each character of the file.
    - Now, we can see that if the character is '0', he is putting the value 0 in the array aiStack on the "index local_c - 1", else if the character is '1', he puts the value 1.
    - And after filling six values in the aiStack, he is doing another loop that calls the function "FUN_001013ab".
    - If we see the code of this function, we can undertsand that this is a function that returns 2^(parameter). So this is a function of power 2.
    - If we try to understand what happens each iteration, we can see that the loop he made is doing 6 iterations and we have two variables that are changing each iteration.
        there is 'local_18' which starts from 0 and stops at 5, and there is 'local_10' that starts at 'local_c' and stops at 'local_c - 5'.
        So first he is calculating 2 power local_18 then he is multiplying the result with aiStack[local_10].

        We will take an example to understand what is happening.

        Imagine we put "HT...." as a string so we have a "data.dat" file that contains "'01001000' '01010100'.........".
        So here, he is taking six by six characters so we can devide our characters like this : "'010010' '000101' '0100..' .........".
        and he is taking each character starting by right and multiply it with 2 power 'the index starting by right'.
        So he is converting the binary value of '010010' to decimal which is 18.

    After taking each 6 characters and convert it to the decimal, he is passing this decimal value to the function "FUN_001013e9".

        *- In this function he declared 8 variables with hex values.
            Let's take an example to understand what happens here:
            the variable local_198 contains "0x5958575655545352" (it is a hex value). And as we know a value in the memory is stored byte by byte starting by right.
            So our variable in the memory is like this (You can search for Little Indian representation to undertand more)
            [address 0] : 52 (the ascii value of R)
            [address 1] : 53 (the ascii value of S)
            [address 2] : 54 (the ascii value of T)
            [address 3] : 55 (the ascii value of U)
            [address 4] : 56 (the ascii value of V)
            [address 5] : 57 (the ascii value of W)
            [address 6] : 58 (the ascii value of X)
            [address 7] : 59 (the ascii value of Y)

            So as our variables are all declared one by one, we can understand that they ara side by side in the memory so we will have like an array in the memory
            for example the two variables local_198 and local_190 will be like this in the memory:
            
            local_198:
            [address 0] : 52 (the ascii value of R)
            [address 1] : 53 (the ascii value of S)
            [address 2] : 54 (the ascii value of T)
            [address 3] : 55 (the ascii value of U)
            [address 4] : 56 (the ascii value of V)
            [address 5] : 57 (the ascii value of W)
            [address 6] : 58 (the ascii value of X)
            [address 7] : 59 (the ascii value of Y)

            local_190:
            [address 8] : 5A (the ascii value of Z)
            [address 9] : 30 (the ascii value of '0')
            [address 10] : 31 (the ascii value of '1')
            [address 11] : 32 (the ascii value of '2')
            [address 12] : 33 (the ascii value of '3')
            [address 13] : 34 (the ascii value of '4')
            [address 14] : 35 (the ascii value of '5')
            [address 15] : 36 (the ascii value of '6')

        We can see that he is taking the address of the first byte of the variable local_198 and add to it the value we calculated in the the last function, after that he is taking the char of this address (it's means he is taking one byte only) and transform it to an integer and give it to the function putchar.
        
        For example if the number we calculated is 0 ; the address of the first byte of local_198 is 0, so 0 + 0 = 0 so we will have the integer of the address [0] -> 52, so we will print R.
        Another example, the number calulated is 18 then we will print the character '9' beacause the address[18] contains 39 in hex which is the ascii value of '9'. 

    
    If we do a little maths : 216/6 = 36, so if we put a string of 27 characters it will return a string of 36 characters in the end.

    
3 - After understanding all that, we can try to put a string which has 27 characters and starting with "HTB......". We can see that it returns a string of 36 characters which starts like the string we have in our file "flag.enc".
We can understand that the string we have in the file "flag.enc" is our flag crypted by the algorithm we saw.


# So now, we will do the opposite of the first algorithm, we will do an algorithm that takes a string of 36 characters and return a string of 27 characters.

You can check the source code of my algorithm to understand the opposite process.
