# Author: Isaac Assegai
# Date  : 6/1/2015
# Pop the .init section back off the registers.
###############################################
.section .init
	/* gcc will nicely put the contents of crtend.o's .init section here. */
	popl %ebp
	ret

.section .fini
	/* gcc will nicely put the contents of crtend.o's .fini section here. */
	popl %ebp
	ret
