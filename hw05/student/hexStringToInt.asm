;;=============================================================
;; CS 2110 - Spring 2024
;; Homework 5 - hexStringToInt
;;=============================================================
;; Name: 
;;=============================================================

;;  Suggested Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  String hexString = "F1ED"; -> given
;;  int length = mem[LENGTH]; -> given
;;  int value = 0; -> filled
;;  int i = 0; -> filled
;;  while (i < length) {
;;      int leftShifts = 4;
;;      while (leftShifts > 0) {
;;          value += value;
;;          leftShifts--;
;;      }
;;      if (hexString[i] >= 65) {
;;          value += hexString[i] - 55;
;;      } else {
;;          value += hexString[i] - 48;
;;      }
;;      i++;
;;  }
;;  mem[mem[RESULTADDR]] = value;

.orig x3000
    ;; YOUR CODE HERE
    
;;  String hexString = "F1ED"; -> given
;;  int length = mem[LENGTH]; -> given
;;  int value = 0; -> filled
;;  int i = 0; -> filled

WHILE1 NOP
;; while condition
;; i - length < 0
LD R0, I ;; R0 = mem[I] = i
LD R1, LENGTH ;; R1 = mem[LENGTH] = length
NOT R1, R1 ;; R1 = ~R1 = ~length
ADD R1, R1, #1 ;; R1 = R1 + 1 = ~length + 1 = -length
ADD R0, R0, R1 ;; R0 = R0 + R1 = i - length
BRzp ENDWHILE
    ;; while body
    ;;      int leftShifts = 4;
    WHILE2 NOP
    ;; while condition
    ;; leftShifts > 0
    LD R1, LEFTSHIFTS ;; R1 = mem[LEFTSHIFTS] = leftShifts
    BRnz ENDWHILE2
        
        ;; while body
        ;;          value += value; -> value = value = value
        LD R2, VALUE ;; R2 = mem[VALUE] = value
        ADD R2, R2, R2 ;; R2 = R2 + R2 = value + value
        ST R2, VALUE ;; mem[VALUE] = R2
        
        ;; increment while
        ;;          leftShifts--;
        LD R1, LEFTSHIFTS
        ADD R1, R1, #-1
        ST R1, LEFTSHIFTS
        BR WHILE2
    ENDWHILE2 NOP
    
    ;; if condition
    ;; hexString[i] - 65 >= 0
    LD R1, HEXSTRING ;; R1 = rtAddr of hexString
    LD R2, I ;; R2 = I = offset
    ADD R1, R1, R2 ;; R1 = R1 + R2 = rtAddr + offset
    LDR R2, R1, #0 ;; R2 = mem[R1 + 0] = mem[rtAddr + offset] = hexString[i]
    ST R2, CHARDIG
    LD R3, NEG65
    ADD R3, R2, R3 ;; R3 = R2 + R3 = CHARDIG + NEG65 = hexString[i] - 65
    BRn ELSE
        ;; if body
        ;; value += hexString[i] - 55;
        LD R5, CHARDIG
        LD R3, VALUE ;; R3 = mem[VALUE] = value
        LD R4, NEG55 ;; R4 = mem[NEG55] = -55
        ADD R4, R5, R4 ;; R4 = R5 + R4 = hexString[i] - 55
        ADD R3, R3, R4 ;; R3 = R3 + R4 = value + hexString[i] - 55
        ST R3, VALUE ;; mem[VALUE] = R3
        BR ENDIF
    ELSE NOP
        ;; else body
        ;; value += hexString[i] - 48;
        LD R5, CHARDIG
        LD R3, VALUE ;; R3 = mem[VALUE] = value
        LD R4, NEG48 ;; R4 = mem[NEG48] = -48
        ADD R4, R5, R4 ;; R4 = R5 + R4 = hexString[i] - 48
        ADD R3, R3, R4 ;; R3 = R3 + R4 = value + hexString[i] - 48
        ST R3, VALUE ;; mem[VALUE] = R3
    ENDIF
    ;; increment while
    ;; i = i + 1
    LD R5, I
    ADD R5, R5, #1 
    ST R5, I
    BR WHILE1
ENDWHILE NOP

;;  mem[mem[RESULTADDR]] = value;
    LD R6, VALUE
    STI R6, RESULTADDR ;; mem[mem[RESULTADDR]] = R6
    
    
    HALT
    
;; Do not change these values!
ASCIIDIG        .fill 48
ASCIICHAR       .fill 55
SIXTYFIVE       .fill 65
HEXSTRING       .fill x5000
LENGTH          .fill 4
RESULTADDR      .fill x4000

;; new vars
VALUE           .fill 0
I               .fill 0
LEFTSHIFTS      .fill 4
NEG55           .fill -55
NEG48           .fill -48
NEG65           .fill -65
CHARDIG         .blkw 1

.end

.orig x4000                    ;;Don't change the .orig statement
    ANSWER .blkw 1             ;;Do not change this value
.end


.orig x5000                    ;;  Don't change the .orig statement
    .stringz "F1ED"            ;;  You can change this string for debugging! Hex characters will be uppercase.
.end
