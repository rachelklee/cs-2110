;;=============================================================
;;  CS 2110 - Spring 2024
;;  Homework 6 - DFS
;;=============================================================
;;  Name: Rachel Lee
;;============================================================

;;  In this file, you must implement the 'SET_VISITED', 'IS_VISITED', and 'DFS' subroutines.


.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments (address of node 1, target node 5)
    ADD R6, R6, -1
    AND R1, R1, 0
    ADD R1, R1, 5
    STR R1, R6, 0
    ADD R6, R6, -1
    LD R1, STARTING_NODE_ADDRESS
    STR R1, R6, 0
    JSR DFS
    LDR R0, R6, 0
    ADD R6, R6, 3
    HALT

    STACK_PTR .fill xF000
    STARTING_NODE_ADDRESS .fill x6110
    VISITED_VECTOR_ADDR .fill x4199 ;; stores the address of the visited vector.

;;  SET_VISITED Pseudocode

;; Parameter: The address of the node
;; Updates the visited vector to mark the given node as visited

;;  SET_VISITED(addr node) {
;;      visited = mem[mem[VISITED_VECTOR_ADDR]];
;;      data = mem[node];
;;      mask = 1;
;;      while (data > 0) {
;;          mask = mask + mask;
;;          data--;
;;      }
;;      mem[mem[VISITED_VECTOR_ADDR]] = (visited | mask); //Hint: Use DeMorgan's Law!
;;  }

SET_VISITED ;; Do not change this label! Treat this as like the name of the function in a function header
;; Code your implementation for the SET_VISITED subroutine here!

    ;; BUILDUP
    	ADD R6, R6, #-4 ;; push stack pointer, allocate for return value, return address, old frame pointer, first local var
    	STR R7, R6, #2 ;; mem[R6 + 2] = R7, push return address (R7) to stack
    	STR R5, R6, #1 ;; mem[R6 + 1] = R5, push old frame pointer (R5) to stack
    	ADD R5, R6, #0 ;; R5 = R6 + 0, set new frame pointer to new stack pointer
    	;; allocate for local variables
    	ADD R6, R6, #-7 ;; X to (-4 - num LVs or -5 if no LVs)
    
    	STR R0, R6, #0 ;; mem[R6 + 0] = R0, save R0
    	STR R1, R6, #1 ;; mem[R6 + 1] = R1, save R1
    	STR R2, R6, #2 ;; mem[R6 + 2] = R2, save R2
    	STR R3, R6, #3 ;; mem[R6 + 3] = R3, save R3
    	STR R4, R6, #4 ;; mem[R6 + 4] = R4, save R4 
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;		    SUBROUTINE BODY
    	;; visited, 1ST LV | R5, #0
    	;; data, 2ND LV    | R5, #-1
    	;; mask, 3RD LV    | R5, #-2
    
    	;; node, 1ST ARG   | R5, #4

    	;; RETURN VAL SPOT | R5, #3
    	
;;      visited = mem[mem[VISITED_VECTOR_ADDR]];
            LDI R0, VISITED_VECTOR_ADDR ;; R0 = mem[mem[VISITED_VECTOR_ADDR]]
            STR R0, R5, #0 ;; mem[R5 + 0] = R0, mem[visited] = mem[mem[VISITED_VECTOR_ADDR]]

;;      data = mem[node];
            LDR R1, R5, #4 ;; R1 = mem[R5 + 4] = node
            LDR R1, R1, #0 ;; R1 = mem[node]
            STR R1, R5, #-1 ;; mem[R5 - 1] = R1, mem[data] = mem[node]

;;      mask = 1;
            AND R2, R2, #0 ;; clear R2
            ADD R2, R2, #1 ;; R2 = R2 + 1 = 1
            STR R2, R5, #-2 ;; mem[R5 - 2] = R2, mem[mask] = 1

            WHILE1 NOP
            ;; data > 0
            LDR R0, R5, #-1 ;; R0 = mem[R5 - 1] = data
            BRnz ENDWHILE1
            	;; mask = mask + mask;
            	LDR R1, R5, #-2 ;; R1 = mem[R5 - 2] = mask
            	ADD R1, R1, R1 ;; R1 = R1 + R1 = mask + mask
            	STR R1, R5, #-2 ;; mask[R5 - 2] = mask + mask, mask = mask + mask
            	
            	;; data--;
            	LDR R0, R5, #-1 ;; R0 = mem[R5 - 1] = mem[data]
            	ADD R0, R0, #-1 ;; R0 = R0 - 1 = data - 1
            	STR R0, R5, #-1 ;; mem[R5 - 1] = R0, data = data - 1
            	
            BR WHILE1
            ENDWHILE1 NOP

;;      mem[mem[VISITED_VECTOR_ADDR]] = (visited | mask); //Hint: Use DeMorgan's Law!
            ;; visited | mask = ~((~visited) and (~mask))
        LDR R0, R5, #0 ;; R0 = visited
        NOT R0, R0 ;; R0 = ~R0 = ~visited
        
        LDR R1, R5, #-2 ;; R1 = mask
        NOT R1, R1 ;; R1 = ~R1 = ~mask
        
        AND R0, R0, R1 ;; R0 = R0 & R1 = ~visited & ~mask
        NOT R0, R0 ;; R0 = ~R0 = ~(~visited & ~mask)
        
        STI R0, VISITED_VECTOR_ADDR ;; mem[mem[VISITED_VECTOR_ADDR]] = R0 = ~(~visited & ~mask)

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    	
    	BR TEAR1
    
    
    ;; TEARDOWN
    
    TEAR1 NOP
    	LDR R0, R6, #0 ;; R4 = mem[R6 + 0], restore R0
    	LDR R1, R6, #1 ;; R3 = mem[R6 + 1], restore R1
    	LDR R2, R6, #2 ;; R2 = mem[R6 + 2], restore R2
    	LDR R3, R6, #3 ;; R1 = mem[R6 + 3], restore R3
    	LDR R4, R6, #4 ;; R0 = mem[R6 + 4], restore R4
    	ADD R6, R5, #0 ;; pop saved regs and LVs
    
    	LDR R5, R6, #1 ;; R5 = mem[R6 + 1], restore old frame pointer
    	LDR R7, R6, #2 ;; R7 = mem[R6 + 2], restore return address 
    	ADD R6, R6, #3 ;; pop all but return value

    RET


;;  IS_VISITED Pseudocode

;; Parameter: The address of the node
;; Returns: 1 if the node has been visited, 0 if it has not been visited

;;  IS_VISITED(addr node) {
;;       visited = mem[mem[VISITED_VECTOR_ADDR]];
;;       data = mem[node];
;;       mask = 1;
;;       while (data > 0) {
;;           mask = mask + mask;
;;           data--;
;;       }
;;       return (visited & mask) != 0;
;;   }

;;  SET_VISITED(addr node) {
;;      visited = mem[mem[VISITED_VECTOR_ADDR]];
;;      data = mem[node];
;;      mask = 1;
;;      while (data > 0) {
;;          mask = mask + mask;
;;          data--;
;;      }
;;      mem[mem[VISITED_VECTOR_ADDR]] = (visited | mask); //Hint: Use DeMorgan's Law!
;;  }

IS_VISITED ;; Do not change this label! Treat this as like the name of the function in a function header
;; Code your implementation for the IS_VISITED subroutine here!
    
    ;; BUILDUP
    	ADD R6, R6, #-4 ;; push stack pointer, allocate for return value, return address, old frame pointer, first local var
    	STR R7, R6, #2 ;; mem[R6 + 2] = R7, push return address (R7) to stack
    	STR R5, R6, #1 ;; mem[R6 + 1] = R5, push old frame pointer (R5) to stack
    	ADD R5, R6, #0 ;; R5 = R6 + 0, set new frame pointer to new stack pointer
    	;; allocate for local variables
    	ADD R6, R6, #-7 ;; X to (-4 - num LVs or -5 if no LVs)
    
    	STR R0, R6, #0 ;; mem[R6 + 0] = R0, save R0
    	STR R1, R6, #1 ;; mem[R6 + 1] = R1, save R1
    	STR R2, R6, #2 ;; mem[R6 + 2] = R2, save R2
    	STR R3, R6, #3 ;; mem[R6 + 3] = R3, save R3
    	STR R4, R6, #4 ;; mem[R6 + 4] = R4, save R4 
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;		    SUBROUTINE BODY
    	;; visited, 1ST LV | R5, #0
    	;; data, 2ND LV    | R5, #-1
    	;; mask, 3RD LV    | R5, #-2
    
    	;; node, 1ST ARG   | R5, #4

    	;; RETURN VAL SPOT | R5, #3
    	
;;      visited = mem[mem[VISITED_VECTOR_ADDR]];
            LDI R0, VISITED_VECTOR_ADDR ;; R0 = mem[mem[VISITED_VECTOR_ADDR]]
            STR R0, R5, #0 ;; mem[R5 + 0] = R0, mem[visited] = mem[mem[VISITED_VECTOR_ADDR]]

;;      data = mem[node];
            LDR R1, R5, #4 ;; R1 = mem[R5 + 4] = node
            LDR R1, R1, #0 ;; R1 = mem[node]
            STR R1, R5, #-1 ;; mem[R5 - 1] = R1, mem[data] = mem[node]

;;      mask = 1;
            AND R2, R2, #0 ;; clear R2
            ADD R2, R2, #1 ;; R2 = R2 + 1 = 1
            STR R2, R5, #-2 ;; mem[R5 - 2] = R2, mem[mask] = 1

            WHILE2 NOP
            ;; data > 0
            LDR R0, R5, #-1 ;; R0 = mem[R5 - 1] = data
            BRnz ENDWHILE2
            	;; mask = mask + mask;
            	LDR R1, R5, #-2 ;; R1 = mem[R5 - 2] = mask
            	ADD R1, R1, R1 ;; R1 = R1 + R1 = mask + mask
            	STR R1, R5, #-2 ;; mask[R5 - 2] = mask + mask, mask = mask + mask
            	
            	;; data--;
            	LDR R0, R5, #-1 ;; R0 = mem[R5 - 1] = mem[data]
            	ADD R0, R0, #-1 ;; R0 = R0 - 1 = data - 1
            	STR R0, R5, #-1 ;; mem[R5 - 1] = R0, data = data - 1
            	
            BR WHILE2
            ENDWHILE2 NOP

            ;; return (visited & mask) != 0;
                        
            ;; if (visited & mask != 0) {
            LDR R0, R5, #0 ;; R0 = visited
            LDR R1, R5, #-2 ;; R1 = mask
            AND R0, R0, R1 ;; R0 = R0 & R1 = visited & mask
            BRz ELSE2
            	;; return 1;
            	AND R1, R1, #0 ;; clear R1
            	ADD R1, R1, #1 ;; R1 = R1 + 1 = 1
            	STR R1, R5, #3 ;; return R1
            BR ENDIF2
            ELSE2 NOP
            	;; return 0;
            	AND R1, R1, #0 ;; clear R1
            	STR R1, R5, #3 ;; return R1
            ENDIF2 NOP

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    	
    	BR TEAR2
    
    
    ;; TEARDOWN
    
    TEAR2 NOP
    	LDR R0, R6, #0 ;; R4 = mem[R6 + 0], restore R0
    	LDR R1, R6, #1 ;; R3 = mem[R6 + 1], restore R1
    	LDR R2, R6, #2 ;; R2 = mem[R6 + 2], restore R2
    	LDR R3, R6, #3 ;; R1 = mem[R6 + 3], restore R3
    	LDR R4, R6, #4 ;; R0 = mem[R6 + 4], restore R4
    	ADD R6, R5, #0 ;; pop saved regs and LVs
    
    	LDR R5, R6, #1 ;; R5 = mem[R6 + 1], restore old frame pointer
    	LDR R7, R6, #2 ;; R7 = mem[R6 + 2], restore return address 
    	ADD R6, R6, #3 ;; pop all but return value

    RET


;;  DFS Pseudocode (see PDF for explanation and examples)

;; Parameters: The address of the starting (or current) node, the data of the target node
;; Returns: the address of the node (if the node is found), 0 if the node is not found

;;  DFS(addr node, int target) {
;;        SET_VISITED(node);
;;        if (mem[node] == target) {
;;           return node;
;;        }
;;        result = 0;
;;        for (i = node + 1; mem[i] != 0 && result == 0; i++) {
;;            if (! IS_VISITED(mem[i])) {
;;                result = DFS(mem[i], target);
;;            }
;;        }       
;;        return result;
;;  }

DFS ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the DFS subroutine here!
    
    ;; BUILDUP
    	ADD R6, R6, #-4 ;; push stack pointer, allocate for return value, return address, old frame pointer, first local var
    	STR R7, R6, #2 ;; mem[R6 + 2] = R7, push return address (R7) to stack
    	STR R5, R6, #1 ;; mem[R6 + 1] = R5, push old frame pointer (R5) to stack
    	ADD R5, R6, #0 ;; R5 = R6 + 0, set new frame pointer to new stack pointer
    	;; allocate for local variables
    	ADD R6, R6, #-6 ;; X to (-4 - num LVs or -5 if no LVs)
    
    	STR R0, R6, #0 ;; mem[R6 + 0] = R0, save R0
    	STR R1, R6, #1 ;; mem[R6 + 1] = R1, save R1
    	STR R2, R6, #2 ;; mem[R6 + 2] = R2, save R2
    	STR R3, R6, #3 ;; mem[R6 + 3] = R3, save R3
    	STR R4, R6, #4 ;; mem[R6 + 4] = R4, save R4 
    
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;		    SUBROUTINE BODY
    	;; result, 1ST LV | R5, #0
    	;; i, 2ND LV | R5, #-1
    
    	;; node, 1ST ARG | R5, #4
    	;; target, 2ND ARG | R5, #5

    	;; RETURN VAL SPOT | R5, #3
    	
;;  DFS(addr node, int target) {

;;        SET_VISITED(node);
        LDR R0, R5, #4 ;; R0 = node
        
        ADD R6, R6, #-1 ;; move stack up once for arg node
        STR R0, R6, #0 ;; load node to top of stack
        
        JSR SET_VISITED ;; call subrouting SET_VISITED
        
        ADD R6, R6, #2
        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        ;###############################
        
        ;; if (mem[node] - target == 0) 
        LDR R1, R0, #0 ;; R1 = mem[node]
        LDR R2, R5, #5 ;; R2 = target
        
        NOT R3, R2 ;; R2 = ~target
        ADD R3, R3, #1 ;; R2 = ~target + 1 = - target
        
        ADD R3, R1, R3 ;; R2 = mem[node] - target
        BRnp ELSE3
            ;; return node
            LDR R0, R5, #4 ;; R0 = node
            STR R0, R5, #3 ;; return R0
            BR TEAR3
            
        ELSE3 NOP
        
;;        result = 0;
            AND R0, R0, #0 ;; clear R0
            STR R0, R5, #0 ;; result = R0
            
;;        for (i = node + 1; mem[i] != 0 && result == 0; i++) {
;;            if (! IS_VISITED(mem[i])) {
;;                result = DFS(mem[i], target);
;;            }
;;        }   

            LDR R0, R5, #4 ;; R0 = node
            
            FOR3
            ADD R0, R0, #1 ;; R0 = node + 1 = i
            LDR R1, R0, #0 ;; R1 = mem[i]
            BRz ENDFOR3
            LDR R2, R5, #0 ;; R2 = result
            BRnp ENDFOR3
            
            ADD R6, R6, #-1
            STR R1, R6, #0 ;; push mem[i] to stack
            JSR IS_VISITED
            
            LDR R3, R6, #0
            ADD R6, R6, #2 
            
            ADD R3, R3, #0
            BRnp FOR3
            
            ;; result = DFS(mem[i], target);
            
            ADD R6, R6, #-2
            STR R1, R6, #0
            LDR R2, R5, #5
            STR R2, R6, #1
            JSR DFS
            
            LDR R4, R6, #0 ;; R4 = output
            STR R4, R5, #0 ;; result = R4
            
            ADD R6, R6, #3
            BR FOR3
            ENDFOR3

;;        return result;
            LDR R0, R5, #0 ;; R0 = result
            STR R0, R5, #3 ;; return R0

    	BR TEAR3
    
    
    ;; TEARDOWN
    
    TEAR3 NOP
    	LDR R0, R6, #0 ;; R4 = mem[R6 + 0], restore R0
    	LDR R1, R6, #1 ;; R3 = mem[R6 + 1], restore R1
    	LDR R2, R6, #2 ;; R2 = mem[R6 + 2], restore R2
    	LDR R3, R6, #3 ;; R1 = mem[R6 + 3], restore R3
    	LDR R4, R6, #4 ;; R0 = mem[R6 + 4], restore R4
    	ADD R6, R5, #0 ;; pop saved regs and LVs
    
    	LDR R5, R6, #1 ;; R5 = mem[R6 + 1], restore old frame pointer
    	LDR R7, R6, #2 ;; R7 = mem[R6 + 2], restore return address 
    	ADD R6, R6, #3 ;; pop all but return value

    
    RET
.end

;; Assuming the graphs starting node (1) is at address x6100, here's how the graph (see below and in the PDF) is represented in memory
;;
;;         0      3
;;          \   / | \
;;            4   1 - 2 
;;             \ /    |
;;              5  -  6
;;

.orig x4199
    .fill 0 ;; visited set will be at address x4199, initialized to 0
.end

.orig x6110         ;; node 1 itself lives here at x6110
    .fill 1         ;; node.data (1)
    .fill x6120     ;; node 2 lives at this address
    .fill x6130     ;; node 3 lives at this address
    .fill x6150     ;; node 5 lives at this address   
    .fill 0
.end

.orig x6120	        ;; node 2 itself lives here at x6120
    .fill 2         ;; node.data (2)
    .fill x6110     ;; node 1 lives at this address
    .fill x6130     ;; node 3 lives at this address
    .fill x6160     ;; node 6 lives at this address
    .fill 0
.end

.orig x6130	        ;; node 3 itself lives here at x6130
    .fill 3         ;; node.data (3)
    .fill x6110     ;; node 1 lives at this address
    .fill x6120     ;; node 2 lives at this address
    .fill x6140     ;; node 4 lives at this address
    .fill 0
.end

.orig x6140	        ;; node 4 itself lives here at x6140
    .fill 4         ;; node.data (4)
    .fill x6100     ;; node 0 lives at this address
    .fill x6130     ;; node 3 lives at this address
    .fill x6150     ;; node 5 lives at this address
    .fill 0
.end

.orig x6100         ;; node 0 itself lives here at x6000
    .fill 0         ;; node.data (0)
    .fill x6140     ;; node 4 lives at this address
    .fill 0
.end

.orig x6150	        ;; node 5 itself lives here at x6150
    .fill 5         ;; node.data (5)
    .fill x6110     ;; node 1 lives at this address
    .fill x6140     ;; node 4 lives at this address
    .fill x6160     ;; node 6 lives at this address
    .fill 0
.end

.orig x6160	        ;; node 6 itself lives here at x6160
    .fill 6         ;; node.data (6)
    .fill x6120     ;; node 2 lives at this address
    .fill x6150     ;; node 5 lives at this address
    .fill 0
.end