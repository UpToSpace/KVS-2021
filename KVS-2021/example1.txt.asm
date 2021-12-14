.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "D:\KVS-2021\Debug\StaticLib.lib
ExitProcess PROTO:DWORD 
.stack 4096


 outnum PROTO : DWORD

 outstr PROTO : DWORD

 concat PROTO : DWORD, : DWORD, : DWORD

 lenght PROTO : DWORD, : DWORD

 strton PROTO : DWORD, : DWORD

.const
		newline byte 13, 10, 0
		LTRL1 sdword 16
		LTRL2 sdword 2
.data
		temp sdword ?
		buffer byte 256 dup(0)
		mainx sdword 0
		mainy sdword 0
		mainz sdword 0
.code

;----------- MAIN ------------
main PROC
push LTRL1

pop ebx
mov mainx, ebx

push LTRL2

pop ebx
mov mainy, ebx

push mainx
push mainy
	pop eax
	pop ebx
	or eax, ebx
	push eax

pop ebx
mov mainz, ebx


push mainz
call outnum

push 0
call ExitProcess
main ENDP
end main
