MAIN:       sub r3, LIST
            lea STR, r6
            dec r6
            mov r3, K
            sub r1, r4
            bne END
            cmp K, #-7
            bne &END
            add K
            add K, 3
            jmp &LOOP
END:        stop
STR:        .string "abcd"
LIST:       .data 6, -9
            .data -100
K:          .data 31
