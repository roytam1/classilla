/* Nanojit me */

/* typedef unsigned long long int unsigned long long; */
/* borrowed from www.libsdl.org/tmp/SDL-1.2/src/timer/macos/FastTimes.c */

static asm unsigned long long rdtsc(void);
asm static unsigned long long rdtsc__(void) {
entry rdtsc
#if(0)
	machine 603 /* will this work? */
	@oopsie:
		mftbu	r4
		mftb	r5
		mftbu	r6
		cmpw	r4,r6
		bne @oopsie
#else
	machine 601
	@oopsie:
		mfrtcu	r4
		mfrtcl	r5
		mfrtcu	r6
		cmpw	r4,r6
		bne @oopsie
#endif
	stw	r4,0(r3)
	stw	r5,4(r3)
	blr
}

