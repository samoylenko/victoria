bits 32

global memlock_
global memunlock_
global copymem_

memlock_:                         ; int memlock(void *, short)
       movzx ecx, ax
       mov   ebx, eax
       shr   ebx, 16
       mov   eax, 0x600
       movzx edi, dx
       mov   esi, edx
       shr   esi, 16
       int   0x31
       mov   eax, 1
       jnc   .ml1
       mov   eax, 0
.ml1:
       ret

memunlock_:                       ; int memunlock(void *, short)
       movzx ecx, ax
       mov   ebx, eax
       shr   ebx, 16
       mov   eax, 0x601
       movzx edi, dx
       mov   esi, edx
       shr   esi, 16
       int   0x31
       mov   eax, 1
       jnc   .mul1
       mov   eax, 0
.mul1:
       ret

copymem_:                         ; copymem(void *, void *, short)
       mov esi,eax
       mov edi,edx
       mov ecx,ebx
.cm:
       dec ecx
       mov eax,[esi + ecx * 4]
       mov [edi + ecx * 4],eax
       jnz .cm
       ret
