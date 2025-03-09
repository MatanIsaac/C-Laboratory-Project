MAIN:       add r3 ,  LIST 
LOOP:       prn #25
            mcro 
            inc r6
            mcroend
            dec K
            mcro a_mc abs
            cmp K , #-6
            mcroend
            mcro add
            bne &END
            mcroend
            lea STR , r6
            mov r3 ,K
            sub r1, r4
            bne END
            a_mc
            jmp &LOOP 
END:        stop 
STR:        .string "abcd" 
LIST:       .data 6, -9 
            .data -100
K:          .data 31