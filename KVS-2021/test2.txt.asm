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

 strton PROTO : DWORD

.const
		newline byte 13, 10, 0
		LTRL1 byte 'hello', 0
		LTRL2 byte 'string ', 0
		LTRL3 byte 'ff', 0
.data
		temp sdword ?
		buffer byte 256 dup(0)
		mains dword ?
		mainr dword ?
		mainn dword ?
.code

;----------- func ------------
func PROC 
	
; --- save registers ---
push ebx
push edx
; ----------------------

push offset LTRL1
call outstr

; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret
func ENDP
;------------------------------


;----------- MAIN ------------
main PROC
mov mains, offset LTRL2
mov mainr, offset LTRL3

push mainr
push mains
push offset buffer
call concat
mov mainn, eax

push mainn
call outstr


call func

push 0
call ExitProcess
main ENDP
end main
