;;=============================================================
;;  CS 2110 - Spring 2024
;;  Homework 6 - GCD
;;=============================================================
;;  Name: Rachel Lee
;;============================================================

;;  In this file, you must implement the 'MOD' and 'GCD' subroutines.

.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments A and B
    ADD R6, R6, -2
    LD R1, A
    STR R1, R6, 0
    LD R1, B
    STR R1, R6, 1 
    JSR GCD
    LDR R0, R6, 0
    ADD R6, R6, 3
    HALT

    STACK_PTR   .fill xF000
    ;; You can change these numbers for debugging!
    A           .fill 10
    B           .fill 4


;;  MOD Pseudocode (see PDF for explanation and examples)   
;;  
;;  MOD(int a, int b) {
;;      while (a >= b) {
;;          a -= b;
;;      }
;;      return a;
;;  }

MOD ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the MOD subroutine here!
    
;; CALLEE
;; BUILDUP
	ADD R6, R6, #-4 ;; push stack pointer, allocate for return value, return address, old frame pointer, first local var
	STR R7, R6, #2 ;; mem[R6 + 2] = R7, push return address (R7) to stack
	STR R5, R6, #1 ;; mem[R6 + 1] = R5, push old frame pointer (R5) to stack
	ADD R5, R6, #0 ;; R5 = R6 + 0, set new frame pointer to new stack pointer
	;; allocate for local variables
	ADD R6, R6, #-5 ;; X to (-4 - num LVs or -5 if no LVs)

	STR R0, R6, #0 ;; mem[R6 + 0] = R0, save R0
	STR R1, R6, #1 ;; mem[R6 + 1] = R1, save R1
	STR R2, R6, #2 ;; mem[R6 + 2] = R2, save R2
	STR R3, R6, #3 ;; mem[R6 + 3] = R3, save R3
	STR R4, R6, #4 ;; mem[R6 + 4] = R4, save R4 
	
	; Your return value is addressed as FP, 3
	; Local variables are addressed as FP, 0; FP, -1; FP, -2; etc.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;		    SUBROUTINE BODY
	;; 1ST LV | R5, #0
	;; 2ND LV | R5, #-1
	;; NTH LV | R5, 1 - N

	;; a, 1ST ARG | R5, #4
	;; b, 2ND ARG | R5, #5

	;; RETURN VAL SPOT | R5, #3

	;; begin body 	
	
;;  MOD(int a, int b) {
;;      
;;          a -= a - b
;;      }
;;      return a;
;;  }

        WHILE1 NOP ;; while (a - b >= 0) {
        LDR R1, R5, #4 ;; R1 = a
        LDR R2, R5, #5 ;; R2 = b
        NOT R0, R2 ;; R0 = ~b
        ADD R0, R0, #1 ;; R0 = -b
        ADD R0, R1, R0 ;; R0 = a - b
        BRn ENDWHILE1 ;; break while loop
        	;; a -= a - b
        	LDR R1, R5, #4 ;; R1 = a
            LDR R2, R5, #5 ;; R2 = b
            NOT R0, R2 ;; R0 = ~b
            ADD R0, R0, #1 ;; R0 = -b
            ADD R0, R1, R0 ;; R0 = a - b
        	STR R0, R5, #4
        BR WHILE1 ;; break to check condition
        ENDWHILE1 NOP ;; end of while1
        
        ;; return a
        LDR R1, R5, #4 ;; R1 = a
        STR R1, R5, #3 ;; stack(RV) = a

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	

	; Store return value in register (for this example, using R1)
	; LDR R0, R5, #offset ;; FILL OFFSET
	; STR R0, R5, #3 ; save return alue on stack	

	BR TEAR1

;; TEARDOWN

TEAR1 NOP
	LDR R0, R6, #0 ;; R4 = mem[R6 + 0], restore R0
	LDR R1, R6, #1 ;; R3 = mem[R6 + 1], restore R1
	LDR R2, R6, #2 ;; R2 = mem[R6 + 2], restore R2
	LDR R3, R6, #3 ;; R1 = mem[R6 + 3], restore R3
	LDR R4, R6, #4 ;; R0 = mem[R6 + 4], restore R4

	ADD R6, R5, #0 ;; pop saved regs and LVs, restore stack pointer

	LDR R5, R6, #1 ;; R5 = mem[R6 + 1], restore old frame pointer
	LDR R7, R6, #2 ;; R7 = mem[R6 + 2], restore return address 
	ADD R6, R6, #3 ;; pop all but return value, Pop RA, FP, 1st local var (change valeue if more than one LV)
RET

;;  GCD Pseudocode (see PDF for explanation and examples)
;;
;;  GCD(int a, int b) {
;;      if (b == 0) {
;;          return a;
;;      }
;;        
;;      while (b != 0) {
;;          int temp = b;
;;          b = MOD(a, b);
;;          a = temp;
;;      }
;;      return a;
;;  }

GCD ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the GCD subroutine here!

;; CALLEE
;; BUILDUP
	ADD R6, R6, #-4 ;; push stack pointer, allocate for return value, return address, old frame pointer, first local var
	STR R7, R6, #2 ;; mem[R6 + 2] = R7, push return address (R7) to stack
	STR R5, R6, #1 ;; mem[R6 + 1] = R5, push old frame pointer (R5) to stack
	ADD R5, R6, #0 ;; R5 = R6 + 0, set new frame pointer to new stack pointer
	;; allocate for local variables
	ADD R6, R6, #-5 ;; X to (-4 - num LVs or -5 if no LVs)

	STR R0, R6, #0 ;; mem[R6 + 0] = R0, save R0
	STR R1, R6, #1 ;; mem[R6 + 1] = R1, save R1
	STR R2, R6, #2 ;; mem[R6 + 2] = R2, save R2
	STR R3, R6, #3 ;; mem[R6 + 3] = R3, save R3
	STR R4, R6, #4 ;; mem[R6 + 4] = R4, save R4 
	
	; Your return value is addressed as FP, 3
	; Local variables are addressed as FP, 0; FP, -1; FP, -2; etc.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;		    SUBROUTINE BODY
	;; 1ST LV | R5, #0
	;; 2ND LV | R5, #-1
	;; NTH LV | R5, 1 - N

	;; 1ST ARG | R5, #4
	;; 2ND ARG | R5, #5
	;; NTH ARG | R5, N + 3

	;; RETURN VAL SPOT | R5, #3

	;; begin body 	

;;      if (b == 0) {
;;          return a;
;;      }

    ;; if (b == 0)
    LDR R2, R5, #5 ;; R2 = b
    BRnp ENDIF2
    	;; return a
    	LDR R1, R5, #4 ;; R1 = a
    	STR R1, R5, #3 ;; stack (RV) = a
    	
    	BR ENDIF2
    ENDIF2 NOP

;;        
;;      while (b != 0) {
;;          int temp = b;
;;          b = MOD(a, b);
;;          a = temp;
;;      }
    
    WHILE2 NOP
    ;; while (b != 0)
    LDR R2, R5, #5 ;; R2 = b
    BRz ENDWHILE2
    
    ;; temp = b
    AND R0, R0, #0 ;; clear R0 for temp
    LDR R0, R5, #5 ;; R0 = b
    
    ;; b = MOD(a, b)
    LDR R1, R5, #5 ;; R1 = b
    ADD R6, R6, #-1
    STR R1, R6, #0
    
    LDR R1, R5, #4 ;; R1 = a
    ADD R6, R6, #-1
    STR R1, R6, #0
    
    JSR MOD ;; jump to MOD
    
    LDR R2, R6, #0 ;; R2 = MOD(a,b)
    ADD R6, R6, #3
    STR R2, R5, #5 ;; b = R2
    
    ;; a = temp
    STR R0, R5, #4
    
    BR WHILE2
    ENDWHILE2 NOP


;;      return a;

    LDR R1, R5, #4 ;; R1 = a
    STR R1, R5, #3
            


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	BR TEAR2

;; TEARDOWN

TEAR2 NOP
	LDR R0, R6, #0 ;; R4 = mem[R6 + 0], restore R0
	LDR R1, R6, #1 ;; R3 = mem[R6 + 1], restore R1
	LDR R2, R6, #2 ;; R2 = mem[R6 + 2], restore R2
	LDR R3, R6, #3 ;; R1 = mem[R6 + 3], restore R3
	LDR R4, R6, #4 ;; R0 = mem[R6 + 4], restore R4

	ADD R6, R5, #0 ;; pop saved regs and LVs, restore stack pointer

	LDR R5, R6, #1 ;; R5 = mem[R6 + 1], restore old frame pointer
	LDR R7, R6, #2 ;; R7 = mem[R6 + 2], restore return address 
	ADD R6, R6, #3 ;; pop all but return value, Pop RA, FP, 1st local var (change valeue if more than one LV)
RET

.end