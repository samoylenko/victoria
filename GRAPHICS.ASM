bits 32

global _screen

global setvmode_
global waitretrace_
global putpixel_
global horline_
global vertline_
global setpalette_
global clearscreen_

_screen      dd 0xa0000           ; char *screen = (char *)0xa0000

setvmode_:                        ; void setvmode(short)
       int   0x10
       ret

waitretrace_:                     ; void waitretrace(void)
       mov   dx, 0x3da
.waitforretrace:
       in    al, dx
       test  al, 8
       jz    .waitforretrace
       ret

putpixel_:                        ; void putpixel(short, short, char)
       mov   edi, [_screen]
       shl   edx, 6
       add   edi, edx
       shl   edx, 2
       add   edi, edx
       mov   [edi + eax], bl
       ret

horline_:                         ; void horline(short, short, short, char)
       cmp   eax, edx
       jc    .hl1
       xchg  eax, edx
.hl1:
       mov   edi, [_screen]
       shl   ebx, 6
       add   edi, ebx
       shl   ebx, 2
       add   edi, ebx
       inc   ebx
.hl2:
       mov   [edi + eax], cl
       inc   eax
       cmp   eax, edx
       jc    .hl2
       ret

vertline_:                        ; void vertline(short, short, short, char)
       cmp   edx, ebx
       jc    .vl1
       xchg  edx, ebx
.vl1:
       mov   edi, [_screen]
       add   edi, eax
       mov   eax, edx
       shl   edx, 6
       add   edi, edx
       shl   edx, 2
       add   edi, edx
       inc   ebx
.vl2:
       mov   [edi], cl
       add   edi, 320
       inc   eax
       cmp   eax, ebx
       jc    .vl2
       ret

setpalette_:                      ; void setpalette(void *)
       mov   esi, eax
       mov   dx, 0x3c8
       mov   al, 0
       out   dx, al
       inc   dx
       mov   ecx, 0x300
       cld
       rep   outsb
       ret

clearscreen_:                     ; void clearscreen(void)
       mov   edi, [_screen]
       mov   ecx, 0x3e80
       mov   eax, 0
       cld
       rep   stosd
       ret
