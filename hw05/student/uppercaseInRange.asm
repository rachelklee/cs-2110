;;=============================================================
;; CS 2110 - Spring 2024
;; Homework 5 - uppercaseInRange 
;;=============================================================
;; Name: Rachel Lee
;;=============================================================

;;  Suggested Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  String str = "touppERcase";
;;  int start = mem[START];
;;  int end = mem[END];
;;  int length = mem[LENGTH];
;;  if (end > length) {
;;      end = length;
;;  }
;;
;;  for (int x = start; x < end; x++) {
;;      if (str[x] >= 'a') {
;;          str[x] = str[x] - 32;
;;      }
;;  }


.orig x3000
    ;; YOUR CODE HERE
    
    AND R0, R0, #0                  ;; clear R1 for str
    LD R0, STRING                   ;; R1 = STRING -> variable str
    
    AND R1, R1, #0                  ;; clear R1 for start
    LD R1, START                    ;; R1 = mem[START] -> variable start
    
    AND R2, R2, #0                  ;; clear R2 for end
    LD R2, END                      ;; R2 = mem[END] -> variable end
    
    AND R3, R3, #0                  ;; clear R3 for length
    LD R3, LENGTH                   ;; R3 = mem[LENGTH] -> variable length

    AND R4, R4, #0                  ;; clear R4 for end - length
    NOT R4, R3                      ;; R4 = ~R3 = ~length
    ADD R4, R4, #1                  ;; R4 = R4 + 1 = ~length + 1 = -length
    ADD R4, R2, R4                  ;; R4 = R2 + R4 = end - length
    ADD R4, R4, #0                  ;; if (R4 > 0) then
        BRnz ELSE1
        
        ;;end = length
        AND R2, R2, #0              ;; clear R2 for end = length
        LD R2, LENGTH               ;; R2 = mem[LENGTH]
        
        BR ELSE1
    ELSE1 NOP                       ;; else
        
        AND R4, R4, #0              ;; clear R4 for x -> variable x
        LD R4, START                ;; R4 = mem[start]
        
        AND R3, R3, #0              ;; clear R5 for x - end
        NOT R3, R2                  ;; R5 = ~R2 = ~end
        ADD R3, R3, #1              ;; R5 = R5 + 1 = ~end + 1 = -end
        ADD R3, R4, R3              ;; R5 = R4 + R5 = x - end
        FOR1 ADD R3, R3, #0
            BRzp ENDF1              ;; R5 < 0
            ;; FOR body
            
            AND R5, R5, #0
            AND R6, R6, #0
            AND R7, R7, #0
            
            ADD R5, R0, R4
            LDR R6, R5, #0
            LDR R7, R5, #0
            
            ADD R7, R7, #-15
            ADD R7, R7, #-15
            ADD R7, R7, #-15
            ADD R7, R7, #-15
            ADD R7, R7, #-15
            ADD R7, R7, #-15
            ADD R7, R7, #-7
            BRn SKIP_SUB
            
            ADD R6, R6, #-15
            ADD R6, R6, #-15
            ADD R6, R6, #-2
            
            STR R6, R5, #0
        
        SKIP_SUB
            
            
            
;;      if (str[x] >= 'a') {
;;          str[x] = str[x] - 32;
;;      }
            
            ADD R4, R4, #1          ;; R4 = R4 + 1 -> ++x
            ADD R3, R3, #1
            BR FOR1
        ENDF1 NOP
        
;;  for (int x = start; x < end; x++) {

;;  }
        
    ENDIF1 NOP                      ;; endif

    HALT

;; Do not change these values!
STRING          .fill x5000
ASCII_A         .fill 97

;; You can change these numbers for debugging!
LENGTH          .fill 11
START           .fill 2
END             .fill 9

.end

.orig x5000                    ;;  Don't change the .orig statement
    .stringz "touppERcase"     ;;  You can change this string for debugging!
.end
