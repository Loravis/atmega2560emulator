start:
	.global main
main:
	inc r0
	inc r1
	add r0, r1
	adc r0, r0
	rjmp main
