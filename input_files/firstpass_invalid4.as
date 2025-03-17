.extern
MAIN:       add r3 ,  LIST $3
LOOP:       prn #48 xcv
            mcro a_mc
            cmp K       ,  #-6
            bne &END
            mcroend
doit :      add #3, r3 
            run_it STR
            lea STR ,r6
            inc r6
            mov r3 , K
            sub r1 ,r4
            bne END 65
            a_mc
            dec K
.entry 
            jmp &LOOP 
END:        stop asd
add:        .string "abcd" 
            .string  
LIST:       .data 6, -9 
            .data 
K:          .data 31