; adding LIST to r3 register - MISSING OPERAND
MAIN:       add r3,                                                                         LIST 
LOOP:       prn #48
            mcroa_mc xyz
            cmp K , #-6
            bne &END
            mcroend
            mcro data 
            inc r1
            add r2, r1
            mcroend
            lea STR, r6
            inc r6
            mov r3 ,K
            sub r1, r4
            bne END
            a_mc
            mcro thisisaveryverylongandincorrectmacroname
            dec K
            mcroend
            jmp &LOOP 
END:        stop 

STR:        .string "abcd" 

LIST:       .data 6, -9 
            .data -100
K:          .data 31