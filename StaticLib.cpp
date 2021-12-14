#include "pch.h"
#include "framework.h"

#include "stdafx.h"
#include <iostream>
#include <stdio.h>

extern "C"
{
	int __stdcall strton(char* source) // строка в число
	{
		int n = 0;
		while (*source >= '0' && *source <= '9') {
			n *= 10;
			n += *source++;
			n -= '0';
		}
		return n;
	}

	int __stdcall lenght(char* buffer, char* str) // длина строки
	{
		if (str == nullptr)
			return 0;
		int len = 0;
		for (int i = 0; i < 256; i++)
			if (str[i] == '\0')
			{
				len = i; break;
			}
		return len;
	}

	char* __stdcall concat(char* buffer, char* str1, char* str2) // конкатенация строк
	{
		int i = NULL, len1 = NULL, len2 = NULL;
		for (int j = 0; str1[j] != '\0'; j++)
		{
			if (i == 255)
				break;
			buffer[i++] = str1[j];
		}
		for (int j = 0; str2[j] != '\0'; j++)
		{
			if (i == 255)
				break;
			buffer[i++] = str2[j];
		}
		buffer[i] = '\0';
		return buffer;
	}

	int __stdcall outnum(int value) // вывод в стандартный поток числа
	{
		std::cout << value;
		return 0;
	}

	int __stdcall outstr(char* ptr) // вывод в стандартный поток строки
	{
		if (ptr == nullptr)
		{
			std::cout << std::endl;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
		return 0;
	}
}