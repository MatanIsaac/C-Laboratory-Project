MAIN:       sub r2, LIST
            mcro b_mc
            cmp K, #-7
            bne &END
            mcroend
            lea STR, r6
            mcro c_mc
            add #3, K
            mcroend
            dec r6
            mov r3, K
            sub r1, r4
            bne END
            b_mc
            inc K
            c_mc
            jmp &LOOP
END:        stop
STR:        .string "abcd"
LIST:       .data 6, -9
            .data -100
K:          .data 31