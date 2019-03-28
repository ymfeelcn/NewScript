#pragma once
//#ifndef _UNISTD_H
//#define _UNISTD_H


#include "stdafx.h"

#include <io.h>
#include <process.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
//#include <unistd.h>

#define SMALL_CHUNK 256

static char* randomkey() {
	char tmp[9] = { 0 };

	ZeroMemory(tmp, 9);
	tmp[8] = '\0';
	int i;
	char x = 0;
	srand((unsigned)time(NULL));
	for (i = 0; i < 8; i++) {
		tmp[i] = rand() & 0xff;
		x ^= tmp[i];
	}
	if (x == 0) {
		tmp[0] |= 1;	// avoid 0
	}
	return tmp;
}

static char* tohex(char* str, int len) {
	static char hex[] = "0123456789abcdef";
	size_t sz = len;
	const uint8_t * text = (const uint8_t *)str;
	char tmp[SMALL_CHUNK];
	char *buffer = tmp;
// 	if (sz > SMALL_CHUNK / 2) {
// 		buffer = (char *)lua_newuserdata(L, sz * 2);
// 	}
	for (int i = 0; i < sz; i++)
	{
		buffer[i * 2] = hex[text[i] >> 4];
		buffer[i * 2 + 1] = hex[text[i] & 0xf];
	}
	char strtext[16] = { 0 };
	ZeroMemory(strtext, 16);
	memcpy(strtext, buffer, 16);
	return strtext;
}

//#endif /* _UNISTD_H */