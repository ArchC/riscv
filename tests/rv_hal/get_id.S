#define return	a0

.global	get_id

get_id:
	csrr return, mhartid
	ret
