PIO_PER equ 0x400E0E00
PIO_OER equ 0x400E0E10
PIO_CODR equ 0x400E0E34
PIO_SODR equ 0x400E0E30
PIO_IFER equ 0x400E0E20
PIO_PUER equ 0x400E0E64
PIO_ISR equ 0x400E0E4C
PIO_PDSR equ 0x400E0E3C

	area blinkCode, code, readonly
	export __main
	entry
			
__main
	bl enable_pio
	bl enable_input

loop
	bl led_off
	ldr r4, =PIO_ISR
	ldr r5, [r4]
	mov r4, #2_1000
	cmp r4, r5
	beq blink
	b loop
	
blink
	bl led_left_on
	bl led_middle_on
	bl led_right_on
	
	bl delay
	bl led_right_off
	
	bl delay
	bl led_right_on
	bl led_middle_off
	
	bl delay
	bl led_right_off
	
	bl delay
	bl led_right_on
	bl led_middle_on
	bl led_left_off
	
	bl delay
	bl led_right_off
	
	bl delay
	bl led_right_on
	bl led_middle_off
	
	bl delay
	bl led_right_off
	
	bl delay
	
	b blink
	
led_left_on
	mov r8, #2_001
	
	ldr r9, =PIO_SODR
	str r8, [r9]
	
	bx lr
	
led_left_off
	mov r8, #2_001
	
	ldr r9, =PIO_CODR
	str r8, [r9]
	
	bx lr
	
led_middle_on
	mov r8, #2_010
	
	ldr r9, =PIO_SODR
	str r8, [r9]
	
	bx lr
	
led_middle_off
	mov r8, #2_010
	
	ldr r9, =PIO_CODR
	str r8, [r9]
	
	bx lr
	
led_right_on
	mov r8, #2_100
	
	ldr r9, =PIO_SODR
	str r8, [r9]
	
	bx lr
	
led_right_off
	mov r8, #2_100
	
	ldr r9, =PIO_CODR
	str r8, [r9]
	
	bx lr
	

enable_pio
	mov r4, #2_111
		
	ldr r5, =PIO_PER
	str r4, [r5]
	
	ldr r5, =PIO_OER
	str r4, [r5]
	
	bx lr

enable_input
	mov r4, #2_11000
	
	ldr r5, =PIO_IFER
	str r4, [r5]
	
	bx lr

led_on
	mov r4, #2_111
	ldr r5, =PIO_SODR
	str r4, [r5]
	bx lr

led_off
	mov r4, #2_111
	ldr r5, =PIO_CODR
	str r4, [r5]
	bx lr

delay
	mov r4, #0
	ldr r5, =0x0000A000

delay_incrementor
	ldr r6, =PIO_ISR
	ldr r7, [r6]
	mov r6, #2_10000
	cmp r6, r7
	beq loop
	add r4, r4, #1
	cmp r4, r5
	bne delay_incrementor
	bx lr

	end