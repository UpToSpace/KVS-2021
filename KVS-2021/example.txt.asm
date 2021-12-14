.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "D:\KVS-2021\Debug\StaticLib.lib"
ExitProcess PROTO:DWORD 
.stack 4096


 outnum PROTO : DWORD

 outstr PROTO : DWORD

 concat PROTO : DWORD, : DWORD, : DWORD

 lenght PROTO : DWORD, : DWORD

.const
		newline byte 13, 10, 0
		LTRL1 sdword 1
		LTRL2 sdword 54
		LTRL3 byte 'Just', 0
		LTRL4 byte 'string', 0
		LTRL5 sdword 3
		LTRL6 sdword 52
		LTRL7 byte ' ', 0
		LTRL8 sdword 2
.data
		temp sdword ?
		buffer byte 256 dup(0)
		minres sdword 0
		standk sdword 0
		standstr dword ?
		mainx sdword 0
		mainy sdword 0
		mainstrx dword ?
		mainstry dword ?
		maint sdword 0
		mainab sdword 0
.code

;----------- min ------------
min PROC,
	minx : sdword, miny : sdword  
; --- save registers ---
push ebx
push edx
; ----------------------
; --- restore registers ---
pop edx
pop ebx
; -------------------------
mov eax, minres
ret
min ENDP
;------------------------------


;----------- stand ------------
stand PROC,
	standa : dword, standb : dword  
; --- save registers ---
push ebx
push edx
; ----------------------

push standa
push offset buffer
call lenght
push eax
push LTRL1
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov standk, ebx


push standk
call outnum

push offset newline
call outstr


push standb
push standa
push offset buffer
call concat
mov standstr, eax

push standstr
call outstr

push offset newline
call outstr

; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret
stand ENDP
;------------------------------


;----------- MAIN ------------
main PROC
push LTRL1

pop ebx
mov mainx, ebx

push LTRL2

pop ebx
mov mainy, ebx

mov mainstrx, offset LTRL3
mov mainstry, offset LTRL4

push mainy
push mainx
call min
push eax

pop ebx
mov maint, ebx


push maint
call outnum

push offset newline
call outstr

push LTRL5

pop ebx
mov mainab, ebx

mov edx, mainab
cmp edx, LTRL6

jnz cycle1
jmp cyclenext1
cycle1:

push mainab
call outnum


push offset LTRL7
call outstr

push mainab
push LTRL8
pop ebx
pop eax
sub eax, ebx
push eax
push LTRL8
pop ebx
pop eax
imul eax, ebx
push eax

pop ebx
mov mainab, ebx

mov edx, mainab
cmp edx, LTRL6

jnz cycle1
cyclenext1:

push offset LTRL7
call outstr


push mainab
call outnum

push offset newline
call outstr


push mainstry
push mainstrx
call stand

push 0
call ExitProcess
main ENDP
end main
