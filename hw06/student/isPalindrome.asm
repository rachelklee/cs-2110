;;=============================================================
;;  CS 2110 - Spring 2024
;;  Homework 6 - isPalindrome
;;=============================================================
;;  Name: Rachel Lee
;;============================================================

;;  In this file, you must implement the 'isPalindrome' subroutine.
 

.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments (word addr and len)
    ADD R6, R6, -2
    LEA R0, STRING
    LD R1, LENGTH
    STR R0, R6, 0
    STR R1, R6, 1
    JSR IS_PALINDROME
    LDR R0, R6, 0
    ADD R6, R6, 3
    HALT
    STACK_PTR .fill xF000
    LENGTH .fill 7                 ;; Change this to be length of STRING
    STRING .stringz "racecar"	       ;; You can change this string for debugging!


;;  IS_PALINDROME **RECURSIVE** Pseudocode
;;
;;  isPalindrome(word (addr), len) { 
;;      if (len == 0 || len == 1) {
;;          return 1;
;;      } else {
;;          if (word[0] == word[len - 1]) {
;;              return IS_PALINDROME(word + 1, len - 2);
;;          } else { 
;;              return 0;
;;          }
;;      }
;;  }
;;
IS_PALINDROME ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the isPalindrome subroutine here!
    ;; NOTE: Your implementation MUST be done recursively
    
;;      if (len == 0 || len == 1) {
;;          return 1;
;;      } else {
;;          if (word[0] == word[len - 1]) {
;;              return IS_PALINDROME(word + 1, len - 2);
;;          } else { 
;;              return 0;
;;          }
;;      }

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

	;; word(addr), 1ST ARG | R5, #4
	;; len, 2ND ARG        | R5, #5

	;; RETURN VAL SPOT     | R5, #3

	;; begin body
	
;;      if (len == 0 || len == 1) {
;;          return 1;
;;      } else {
;;          if (word[0] == word[len - 1]) {
;;              return IS_PALINDROME(word + 1, len - 2);
;;          } else { 
;;              return 0;
;;          }
;;      }

        ;; if (len == 0)
        LDR R2, R5, #5 ;; R2 = len
        BRz THEN1
        
        ;; if (len - 1 == 0)
        LDR R2, R5, #5 ;; R2 = len
        ADD R2, R2, #-1 ;; R2 = len - 1
        BRz THEN1
        
        	;; if (word[0] - word[len - 1] == 0)
        	
        	;; get word[0]
        	LDR R1, R5, #4 ;; R1 = word (addr)
        	LDR R3, R1, #0 ;; R3 = word[0] 
        	
        	;; get word[len - 1]
        	LDR R2, R5, #5 ;; R2 = len
        	AND R4, R4, #0 ;; clear R4
        	ADD R4, R1, R2 ;; R4 = word (addr) + len
        	ADD R4, R4, #-1 ;; R4 = word (addr) + len - 1
        	LDR R4, R4, #0 ;; R4 = word[len - 1]
        	
        	NOT R4, R4 ;; R4 = ~word[len - 1]
        	ADD R4, R4, #1 ;; R4 = - word[len - 1]
        	ADD R4, R3, R4 ;; R4 = word[0] - word[len - 1]
        	
            BRnp ELSE2
            	;; return IS_PALINDROME(word + 1, len - 2);
            	
            	LDR R1, R5, #4 ;; R1 = word
            	ADD R1, R1, #1 ;; R1 = word + 1
            	
            	LDR R2, R5, #5 ;; R2 = len
            	ADD R2, R2, #-2 ;; R2 = len - 2
            	
                ;; CALLER
                ;; BUILDUP
                ; push arguements in reverse
                	;; last arg
                	ADD R6, R6, #-1
                	STR R2, R6, #0
                
                	;; second to last arg
                	ADD R6, R6, #-1
                	STR R1, R6, 0
                
                	;; keep adding until complete
                	JSR IS_PALINDROME ;; call subroutine
                
                ;; TEARDOWN
                	LDR R0, R6, #0 ;; pop return value to R0
                	ADD R6, R6, #1 ;; move stack pointer
                	;; ST R0, RETURNVALUE ;; save return value
                	STR R0, R5, #3 ;; store R0 (return value) to return stack
                
                	ADD R6, R6, #2 ;; pop 2 arguments (A and B)

            BR ENDIF2
            
            ELSE2 NOP
            	;; return 0;
            	AND R0, R0, #0 ;; clear R0
            	STR R0, R5, #3 ;; store R0 (0) to return stack
            	
            ENDIF2 NOP

        	BR ENDIF1
        
        THEN1 NOP
        	;; return 1
        	AND R0, R0, #0 ;; clear R0
        	ADD R0, R0, #1 ;; R0 = 1
        	STR R0, R5, #3 ;; store R0 (1) to return stack
        ENDIF1 NOP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	

	; Store return value in register (for this example, using R1)
	; LDR R0, R5, #offset ;; FILL OFFSET
	; STR R0, R5, #3 ; save return alue on stack	

	BR TEAR


;; TEARDOWN

TEAR NOP
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