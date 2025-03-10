; LIST,  STR
MAIN:       sub r2, 
            mcro b_mc
            cmp K, #-7
            bne &END
            mcroend
            lea , r6        
            prn #3, 
            dec r6
            mov r3  K
            sub r1, r4
            bne END
            b_mc
            inc K
            jmp &LOOP
END:        stop
STR:        .string "abcd"
LIST:       .data 6, -9
            .data -100
K:          .data 31