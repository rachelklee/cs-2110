;;=============================================================
;; CS 2110 - Spring 2024
;; Homework 5 - Fibonacci
;;=============================================================
;; Name: Rachel Lee
;;=============================================================


;; Suggested Pseudocode (see PDF for explanation)
;;
;; n = mem[N];
;; resAddr = mem[RESULT]
;; 
;; if (n == 1) { break if n - 1 is pos or neg (not 0)
;;     mem[resAddr] = 0;
;; } else if (n > 1) {
;;     mem[resAddr] = 0;
;;     mem[resAddr + 1] = 1;
;;     for (i = 2; i < n; i++) {
;;         x = mem[resAddr + i - 1];
;;         y = mem[resAddr + i - 2];
;;         mem[resAddr + i] = x + y;
;;     }
;; }

   ;; Suggested Pseudocode (see PDF for explanation)
;;
;; n = mem[N];
;; resAddr = mem[RESULT]
;; 
;; if (n == 1) { break if n - 1 is pos or neg (not 0)
;;     mem[resAddr] = 0;
;; } else if (n > 1) {
;;     mem[resAddr] = 0;
;;     mem[resAddr + 1] = 1;
;;     for (i = 2; i < n; i++) {
;;         x = mem[resAddr + i - 1];
;;         y = mem[resAddr + i - 2];
;;         mem[resAddr + i] = x + y;
;;     }
;; }

.orig x3000
    ;; YOUR CODE HERE

    ;; n = mem[N];
    LD R0, N ;; R0 = mem[N] = n
    
    ;; resAddr = mem[RESULT]    
    LD R1, RESULT ;; R1 = mem[RESULT]
    ST R1, RESADDR ;; RESADDR = R1 = mem[result] 
    
    ;; if condition
    ;; if (n - 1 == 0)
    ADD R2, R0, #-1 ;; R2 = R0 - 1 = n - 1
    BRnp ELSEIF
        ;; if body
        ;; mem[resAddr] = 0
        AND R3, R3, #0 ;; R3 = 0
        LD R4, RESADDR ;; R4 = mem[resAddr]
        ;;STR R4, R3, #0 ;; !!!!
        STR R3, R4, #0 ;; mem[R4 + 0] = R3, mem[RESADDR] = 0
    BR ENDIF
    
    ELSEIF NOP
    ;; elseif condition
    ;; else if (n - 1 > 0)
    AND R2, R2, #0 ;; R2 = 0
    ADD R2, R0, #-1 ;; R2 = R0 - 1 = n - 1
    BRnz ENDIF
        ;; elseif body
        ;; mem[resAddr] = 0;
        ;; mem[resAddr + 1] = 1
        AND R4, R4, #0 ;; R4 = 0
        LD R3, RESADDR ;; R3 = mem[RESADDR]
        STR R4, R3, #0 ;; mem[R3 + 0] = 0, mem[RESADDR] = 0
        ADD R4, R4, #1 ;; R4 = R4 + 1 = 1
        STR R4, R3, #1 ;; mem[R3 + 1 ] = R4, mem[RESADDR + 1] = 1

        ;; for initialization
        ;; for (i = 2; i < n; i++) -> i = 2
        AND R2, R2, #0 ;; R2 = 0
        ADD R2, R2, #2 ;; R2 = 2
        FOR1 NOP
        ;; for condition
        ;; for (i = 2; i < n; i++) -> i - n < 0
        NOT R3, R0 ;; R3 = ~R0 = ~n
        ADD R3, R3, #1 ;; R3 = R3 + 1 = ~n + 1 = -n
        ADD R3, R2, R3 ;; R3 = R2 + R3 = i - n
        BRzp ENDFOR    
            ;; for body
            ;; x = mem[resAddr + i - 1];
            ;; y = mem[resAddr + i - 2];
            ;; mem[resAddr + i] = x + y;
            AND R4, R4, #0 ;; R4 = 0
            LD R5, RESADDR ;; R5 = mem[RESADDR]
            ADD R5, R5, R2 ;; resAddr + i ;; R5 = R4 + R2 = RESADDR + i
            LDR R4, R5, #-1 ;; R4 = mem[R5 - 1] = mem[RESADDR + i - 1]
            LEA R6, X ;; R6 = address of X
            STR R4, R6, #0 ;; mem[R6 + 0] = R4, X = mem[RESADDR + i - 1]
            LDR R4, R5, #-2 ;; R4 = mem[R5 - 2] = mem[RESADDR + i - 2]
            LEA R6, Y ;; R6 = address of Y
            STR R4, R6, #0 ;; mem[R6 + 0] = R4, Y = mem[RESADDR + i - 2]
            LD R6, X ;; R6 = mem[X] = x
            LD R7, Y ;; R7 = mem[Y] = y
            ADD R6, R6, R7 ;; R6 = R6 + R7 = mem[X] + mem[Y] = x + y
            STR R6, R5, #0 ;; mem[R5 + 0] = R6, mem[RESADDR + i] = x + y
            
            ;; increment condition
            ;; for (i = 2; i < n; i++) -> i = i + 1
            ADD R2, R2, #1 ;; R2 = R2 + 1 = i + 1
            LEA R7, N ;; R7 = address of N
            STR R2, R7, #0 ;; mem[R7 + 0] = R2, mem[N] = i + 1

        BR FOR1
        ENDFOR NOP
            
    ENDIF NOP
    
    HALT
    
    ;; Do not rename or remove any existing labels
;; You may change the value of N for debugging
N       .fill 3
RESULT  .fill x4000
RESADDR .blkw 1
X       .blkw 1
Y       .blkw 1

.end

.orig x4000
.blkw 100
.end
