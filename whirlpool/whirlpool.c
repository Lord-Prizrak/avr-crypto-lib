/* whirlpool.c */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2006-2011 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "gf256mul.h"
#include "memxor.h"
#include "whirlpool.h"

#define DEBUG 0

#if DEBUG
#include "cli.h"
#endif

#if WHIRLPOOL_0
  #ifdef SBOX_PROG
    #undef SBOX_PROG
  #endif
  #define SBOX_PROG 0
#endif

#if SBOX_PROG
/*
u       0 	1 	2 	3 	4 	5 	6 	7 	8 	9 	A 	B 	C 	D 	E 	F
E(u)    1 	B 	9 	C 	D 	6 	F 	3 	E 	8 	7 	4 	A 	2 	5 	0
E -1(u) F 	0 	D 	7 	B 	E 	5 	A 	9 	2 	C 	1 	3 	4 	8 	6
*/
static const uint8_t eeinv_box[16] PROGMEM = {
	/*	0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
	0x1F, 0xB0, 0x9D, 0xC7, 0xDB, 0x6E, 0xF5, 0x3A,
	0xE9, 0x82, 0x7C, 0x41, 0xA3, 0x24, 0x58, 0x06
};

static const uint8_t r_box[16] PROGMEM = {
	0x77, 0xCC, 0xBB, 0xDD, 0xEE, 0x44, 0x99, 0xFF,
	0x66, 0x33, 0x88, 0xAA, 0x22, 0x55, 0x11, 0x00
};

static uint8_t whirlpool_sbox(uint8_t a){
	uint8_t b,c,d;
	b = pgm_read_byte(eeinv_box+(a&0x0f));
	d = pgm_read_byte(eeinv_box+(a>>4));
	c = ((d>>4)^b)&0x0f;
	b = (b&0x0f)|(d&0xf0);
	b ^= pgm_read_byte(r_box+c);
	c = pgm_read_byte(eeinv_box+(b&0x0f))&0x0f;
	c |= pgm_read_byte(eeinv_box+(b>>4))&0xf0;
	return c;
}

#else
#if WHIRLPOOL_0
static const  uint8_t sbox[256] PROGMEM = {
	0x68, 0xd0, 0xeb, 0x2b, 0x48, 0x9d, 0x6a, 0xe4, 0xe3, 0xa3, 0x56, 0x81, 0x7d, 0xf1, 0x85, 0x9e,
	0x2c, 0x8e, 0x78, 0xca, 0x17, 0xa9, 0x61, 0xd5, 0x5d, 0x0b, 0x8c, 0x3c, 0x77, 0x51, 0x22, 0x42,
	0x3f, 0x54, 0x41, 0x80, 0xcc, 0x86, 0xb3, 0x18, 0x2e, 0x57, 0x06, 0x62, 0xf4, 0x36, 0xd1, 0x6b,
	0x1b, 0x65, 0x75, 0x10, 0xda, 0x49, 0x26, 0xf9, 0xcb, 0x66, 0xe7, 0xba, 0xae, 0x50, 0x52, 0xab,
	0x05, 0xf0, 0x0d, 0x73, 0x3b, 0x04, 0x20, 0xfe, 0xdd, 0xf5, 0xb4, 0x5f, 0x0a, 0xb5, 0xc0, 0xa0,
	0x71, 0xa5, 0x2d, 0x60, 0x72, 0x93, 0x39, 0x08, 0x83, 0x21, 0x5c, 0x87, 0xb1, 0xe0, 0x00, 0xc3,
	0x12, 0x91, 0x8a, 0x02, 0x1c, 0xe6, 0x45, 0xc2, 0xc4, 0xfd, 0xbf, 0x44, 0xa1, 0x4c, 0x33, 0xc5,
	0x84, 0x23, 0x7c, 0xb0, 0x25, 0x15, 0x35, 0x69, 0xff, 0x94, 0x4d, 0x70, 0xa2, 0xaf, 0xcd, 0xd6,
	0x6c, 0xb7, 0xf8, 0x09, 0xf3, 0x67, 0xa4, 0xea, 0xec, 0xb6, 0xd4, 0xd2, 0x14, 0x1e, 0xe1, 0x24,
	0x38, 0xc6, 0xdb, 0x4b, 0x7a, 0x3a, 0xde, 0x5e, 0xdf, 0x95, 0xfc, 0xaa, 0xd7, 0xce, 0x07, 0x0f,
	0x3d, 0x58, 0x9a, 0x98, 0x9c, 0xf2, 0xa7, 0x11, 0x7e, 0x8b, 0x43, 0x03, 0xe2, 0xdc, 0xe5, 0xb2,
	0x4e, 0xc7, 0x6d, 0xe9, 0x27, 0x40, 0xd8, 0x37, 0x92, 0x8f, 0x01, 0x1d, 0x53, 0x3e, 0x59, 0xc1,
	0x4f, 0x32, 0x16, 0xfa, 0x74, 0xfb, 0x63, 0x9f, 0x34, 0x1a, 0x2a, 0x5a, 0x8d, 0xc9, 0xcf, 0xf6,
	0x90, 0x28, 0x88, 0x9b, 0x31, 0x0e, 0xbd, 0x4a, 0xe8, 0x96, 0xa6, 0x0c, 0xc8, 0x79, 0xbc, 0xbe,
	0xef, 0x6e, 0x46, 0x97, 0x5b, 0xed, 0x19, 0xd9, 0xac, 0x99, 0xa8, 0x29, 0x64, 0x1f, 0xad, 0x55,
	0x13, 0xbb, 0xf7, 0x6f, 0xb9, 0x47, 0x2f, 0xee, 0xb8, 0x7b, 0x89, 0x30, 0xd3, 0x7f, 0x76, 0x82
};
#else
static const uint8_t sbox[256] PROGMEM = {
	0x18, 0x23, 0xC6, 0xE8, 0x87, 0xB8, 0x01, 0x4F, 0x36, 0xA6, 0xD2, 0xF5, 0x79, 0x6F, 0x91, 0x52,
    0x60, 0xBC, 0x9B, 0x8E, 0xA3, 0x0C, 0x7B, 0x35, 0x1D, 0xE0, 0xD7, 0xC2, 0x2E, 0x4B, 0xFE, 0x57,
    0x15, 0x77, 0x37, 0xE5, 0x9F, 0xF0, 0x4A, 0xDA, 0x58, 0xC9, 0x29, 0x0A, 0xB1, 0xA0, 0x6B, 0x85,
	0xBD, 0x5D, 0x10, 0xF4, 0xCB, 0x3E, 0x05, 0x67, 0xE4, 0x27, 0x41, 0x8B, 0xA7, 0x7D, 0x95, 0xD8,
	0xFB, 0xEE, 0x7C, 0x66, 0xDD, 0x17, 0x47, 0x9E, 0xCA, 0x2D, 0xBF, 0x07, 0xAD, 0x5A, 0x83, 0x33,
	0x63, 0x02, 0xAA, 0x71, 0xC8, 0x19, 0x49, 0xD9, 0xF2, 0xE3, 0x5B, 0x88, 0x9A, 0x26, 0x32, 0xB0,
	0xE9, 0x0F, 0xD5, 0x80, 0xBE, 0xCD, 0x34, 0x48, 0xFF, 0x7A, 0x90, 0x5F, 0x20, 0x68, 0x1A, 0xAE,
	0xB4, 0x54, 0x93, 0x22, 0x64, 0xF1, 0x73, 0x12, 0x40, 0x08, 0xC3, 0xEC, 0xDB, 0xA1, 0x8D, 0x3D,
	0x97, 0x00, 0xCF, 0x2B, 0x76, 0x82, 0xD6, 0x1B, 0xB5, 0xAF, 0x6A, 0x50, 0x45, 0xF3, 0x30, 0xEF,
	0x3F, 0x55, 0xA2, 0xEA, 0x65, 0xBA, 0x2F, 0xC0, 0xDE, 0x1C, 0xFD, 0x4D, 0x92, 0x75, 0x06, 0x8A,
	0xB2, 0xE6, 0x0E, 0x1F, 0x62, 0xD4, 0xA8, 0x96, 0xF9, 0xC5, 0x25, 0x59, 0x84, 0x72, 0x39, 0x4C,
	0x5E, 0x78, 0x38, 0x8C, 0xD1, 0xA5, 0xE2, 0x61, 0xB3, 0x21, 0x9C, 0x1E, 0x43, 0xC7, 0xFC, 0x04,
	0x51, 0x99, 0x6D, 0x0D, 0xFA, 0xDF, 0x7E, 0x24, 0x3B, 0xAB, 0xCE, 0x11, 0x8F, 0x4E, 0xB7, 0xEB,
	0x3C, 0x81, 0x94, 0xF7, 0xB9, 0x13, 0x2C, 0xD3, 0xE7, 0x6E, 0xC4, 0x03, 0x56, 0x44, 0x7F, 0xA9,
	0x2A, 0xBB, 0xC1, 0x53, 0xDC, 0x0B, 0x9D, 0x6C, 0x31, 0x74, 0xF6, 0x46, 0xAC, 0x89, 0x14, 0xE1,
	0x16, 0x3A, 0x69, 0x09, 0x70, 0xB6, 0xD0, 0xED, 0xCC, 0x42, 0x98, 0xA4, 0x28, 0x5C, 0xF8, 0x86
};
#endif

#define whirlpool_sbox(a) pgm_read_byte(sbox+a)

#endif

static void gamma(uint8_t* a){
	uint8_t i;
	for(i=0; i<64; ++i){
		*a = whirlpool_sbox(*a);
		++a;
	}
}

static void pi(uint8_t* a){
	uint8_t b[8];
	uint8_t i,j;
	for(i=1; i<8; ++i){
		for(j=0; j<8; ++j){
			b[j] = a[i+8*((8+j-i)%8)];
		}
		for(j=0; j<8; ++j){
			a[j*8+i] = b[j];
		}
	}
}

#if WHIRLPOOL_0 || WHIRLPOOL_T
static const uint8_t theta_matrix[8] PROGMEM = {
	0x1, 0x1, 0x3, 0x1, 0x5, 0x8, 0x9, 0x5
};
#else
static const uint8_t theta_matrix[8] PROGMEM = {
	0x1, 0x1, 0x4, 0x1, 0x8, 0x5, 0x2, 0x9
};
#endif

#define POLYNOM 0x1D

static void theta(uint8_t* a){
	uint8_t b[8], c, accu;
	uint8_t i,j,k;
	for(i=0; i<8; ++i){
		for(j=0; j<8;++j){
			accu = 0;
			for(k=0; k<8; ++k){
				c = pgm_read_byte(theta_matrix+((8+j-k)%8));
				accu ^= gf256mul(a[8*i+k], c, POLYNOM);
			}
			b[j] = accu;
		}
		memcpy(a+8*i, b, 8);
	}
}

static void w_round(uint8_t* a, const uint8_t* k){
	gamma(a);
#if DEBUG
	cli_putstr_P(PSTR("\r\n pre-pi:"));
	cli_hexdump_block(a, 64, 4, 8);
#endif
	pi(a);
#if DEBUG
	cli_putstr_P(PSTR("\r\n post-pi & pre-theta:"));
	cli_hexdump_block(a, 64, 4, 8);
#endif
	theta(a);
#if DEBUG
	cli_putstr_P(PSTR("\r\n post-theta:"));
	cli_hexdump_block(a, 64, 4, 8);
#endif
	memxor(a, k, 64);
}

static void w_enc(uint8_t *a, const uint8_t* k){
#if DEBUG
	cli_putstr_P(PSTR("\r\n== w_enc ==\r\n w'_00:"));
	cli_hexdump_block(a, 64, 4, 8);
	cli_putstr_P(PSTR("\r\n k_00:"));
	cli_hexdump_block(k, 64, 4, 8);
#endif
	uint8_t rk[64], rc[64];
	uint8_t r;
	memxor(a, k, 64);
	memcpy(rk, k, 64);
	memset(rc+8, 0, 56);
	for(r=0; r<10; ++r){
#if SBOX_PROG
		uint8_t i;
		for(i=0; i<8; ++i){
			rc[i] = whirlpool_sbox(r*8+i);
		}
#else
		memcpy_P(rc, sbox+r*8, 8);
#endif
		w_round(rk, rc);
		w_round(a, rk);
#if DEBUG
		cli_putstr_P(PSTR("\r\n w'_"));
		cli_hexdump_byte(r+1);
		cli_putc(':');
		cli_hexdump_block(a, 64, 4, 8);
		cli_putstr_P(PSTR("\r\n k_"));
		cli_hexdump_byte(r+1);
		cli_putc(':');
		cli_hexdump_block(rk, 64, 4, 8);
#endif
	}
}

void whirlpool_init(whirlpool_ctx_t* ctx){
	memset(ctx->s, 0, 64);
	ctx->blocks = 0;
}

void whirlpool_nextBlock(whirlpool_ctx_t* ctx, const void* block){
	uint8_t state[64];
	ctx->blocks += 1;
	memcpy(state, block, 64);
	w_enc(state, (uint8_t*)(ctx->s));
	memxor(ctx->s, state, 64);
	memxor((ctx->s), block, 64);
}

void whirlpool_lastBlock(whirlpool_ctx_t* ctx, const void* block, uint16_t length_b){
	while(length_b>=512){
		whirlpool_nextBlock(ctx, block);
		block = (uint8_t*)block + 64;
		length_b -= 512;
	}
	uint8_t buffer[64];
	uint8_t i=8;
	uint64_t length;
	length = ctx->blocks*512+length_b;
	memset(buffer, 0, 64);
	memcpy(buffer, block, (length_b+7)/8);
	buffer[length_b/8] |= 0x80>>(length_b&7);
	if(length_b>255){
		whirlpool_nextBlock(ctx, buffer);
		memset(buffer, 0, 56);
	}
	do{
		buffer[56+(--i)] = length&0xff;
		length >>= 8;
	}while(i);
	whirlpool_nextBlock(ctx, buffer);
}

void whirlpool_ctx2hash(void* dest, const whirlpool_ctx_t* ctx){
	memcpy(dest, (ctx->s), 64);
}
