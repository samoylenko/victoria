bits 32

extern _screen

global _printx
global _printy
global _ink
global _text

global escpressed_
global blur_
global printl_
global length_

_printx      dd 0                 ; short printx = 0
_printy      dd 0                 ; short printy = 0
_ink         db 0xff              ; char ink = 0xff

font         incbin "victoria.fnt"
_text        incbin "victoria.txt"
             db 0

escpressed_:                      ; char escpressed(void)
       in    al, 0x60
       dec   al
       ret

blur_:                            ; void blur(void *, void *)
       mov   esi, eax
       add   esi, 321
       mov   edi, edx
       add   edi, 321
       mov   ecx, 198
.blur1:
       push  ecx
       mov   ecx, 318
.blur2:
       mov   ebx, 0
       mov   eax, 0
       mov   bl, [esi - 321]
       add   ax, bx
       mov   bl, [esi - 320]
       add   ax, bx
       mov   bl, [esi - 319]
       add   ax, bx
       mov   bl, [esi - 1]
       add   ax, bx
       mov   bl, [esi]
       add   ax, bx
       mov   bl, [esi + 1]
       add   ax, bx
       mov   bl, [esi + 319]
       add   ax, bx
       mov   bl, [esi + 320]
       add   ax, bx
       mov   bl, [esi + 321]
       add   ax, bx
       mov   bl, 10
       div   bl
       stosb
       inc   esi
       loop  .blur2
       add   edi, 2
       add   esi, 2
       pop   ecx
       loop  .blur1
       ret

printl_:                          ; void printl(char)
       mov   bl, [font + 0x2000 + eax]
       shl   eax, 5
       mov   esi, font
       add   esi, eax
       mov   eax, 320
       mul   word [_printy]
       mov   edi, [_screen]
       add   edi, [_printx]
       add   edi, eax
       mov   ecx, 16
.printl1:
       push  cx
       lodsw
       xchg  ah, al
       mov   cl, 16
       mov   bh, [_ink]
.printl2:
       rcl   ax, 1
       jnc   .printl3
       mov   [edi], bh
.printl3:
       inc   edi
       loop  .printl2
       add   edi, 304
       pop   cx
       loop  .printl1
       mov   bh, 0
       inc   bl
       inc   bl
       add   [_printx], bx
       ret

length_:                          ; char length(char *)
       mov   esi, eax
       mov   ah, 0xff
.len1:
       inc   ah
       lodsb
       cmp   al, 0x0d
       jnz   .len1
       mov   al, ah
       ret
