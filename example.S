; Fibonacci sequence program by qubane

; address for var A
CA
MOVC MR

; write 0
WT

; address for var B
LDA 1
MOVC MR

; write 1
WT

; loop start
; load address for var A
CA
MOVC MR

; move var A to BR
RD
MOV BR

; load address for var B
CA
LDA 1
MOVC MR

; read var B
RD

; A + B -> B
ADD
WT

; move A + B to BR
MOV BR

; load address for var A
CA
; clear carry flag
AND
MOVC MR

; read var A
RD

; compute -(A - B)
SUB

MOV BR

CA
SUB

; write B - A to var A
WT

; load jump address
CA
; reset carry flag
and
LDA 1
LDA 2
MOVC PC
