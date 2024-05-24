.section .text

.gdtr:
.word 0
.long 0

.global setGdt
setGdt:
    movl 4(%esp), %eax
    movl %eax, (.gdtr + 2)
    mov 8(%esp), %ax
    dec %ax
    mov %ax, (.gdtr)
    lgdt (.gdtr)
    ret

.global reloadSegments
reloadSegments:
    ljmp $0x08, $.reload
.reload:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret