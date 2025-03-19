start:
	.global main
main:
	inc r0
	inc r1
	add r0, r1
	add r1, r30
	rjmp main
