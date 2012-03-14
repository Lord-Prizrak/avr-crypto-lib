/* rsa_key_blob.c */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)

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

#include <stdlib.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "cli.h"
#include "rsa.h"
#include "bigint.h"

#define RSA_KEYSIZE 1024

#if RSA_KEYSIZE == 2048
#define N_LEN_B (32*8)
#define E_LEN_B  (3)
#define D_LEN_B (32*8)
#define ALL_LEN_B (N_LEN_B+E_LEN_B+D_LEN_B)

const uint8_t rsa_key_blob[] PROGMEM = {
		/* n */
		0xDA, 0xBC, 0x4A, 0x42, 0x02, 0x6F, 0x7F, 0xC2,
		0xEB, 0x5A, 0x73, 0x2A, 0x84, 0x25, 0xB5, 0x56,
		0xC7, 0x99, 0x25, 0x0A, 0x50, 0x6F, 0x00, 0xAE,
		0x84, 0x5C, 0x19, 0x30, 0xC6, 0xDE, 0xAF, 0xC6,
		0x30, 0xC8, 0xD0, 0x67, 0x96, 0x14, 0xF1, 0x5F,
		0xFF, 0x0C, 0x49, 0xC1, 0x4A, 0xA6, 0x91, 0x52,
		0x83, 0xAE, 0x46, 0xA6, 0x2B, 0xCC, 0x38, 0x90,
		0xBD, 0xE3, 0xB0, 0x10, 0x30, 0x9F, 0x02, 0x69,
		0xCF, 0x34, 0x3E, 0x9C, 0x38, 0x82, 0x2A, 0x21,
		0xB9, 0x98, 0x01, 0x9F, 0x60, 0x97, 0x7B, 0xDD,
		0xDF, 0xB4, 0x5B, 0x78, 0x9B, 0x51, 0x7C, 0xEF,
		0x9E, 0x48, 0xBB, 0xEF, 0x27, 0xD5, 0xA1, 0x38,
		0x10, 0x30, 0x60, 0x7D, 0x1A, 0x50, 0x56, 0x64,
		0xD7, 0x45, 0x47, 0x1B, 0x71, 0x75, 0xBB, 0xA1,
		0xEC, 0x3A, 0xC3, 0xAB, 0x6B, 0x15, 0x84, 0x09,
		0xFD, 0x87, 0x38, 0xE7, 0x28, 0x41, 0x47, 0x84,
		0xB1, 0x87, 0xDE, 0xA8, 0x19, 0xFF, 0xD6, 0xBF,
		0xA4, 0xF5, 0x8B, 0xFF, 0xF7, 0xB5, 0x5F, 0x88,
		0xAE, 0x15, 0xD8, 0x26, 0x31, 0xDE, 0x94, 0xDB,
		0xDC, 0x72, 0xEB, 0x9A, 0xFD, 0xD2, 0xE3, 0xBB,
		0x05, 0x98, 0x76, 0xAD, 0x0D, 0x0A, 0x21, 0xDF,
		0xB4, 0xAA, 0x9B, 0x49, 0xE8, 0x6B, 0xEC, 0x7C,
		0x8B, 0x87, 0x21, 0xEF, 0x6E, 0xD9, 0x64, 0xAB,
		0x17, 0x52, 0xDD, 0xC9, 0x13, 0x4B, 0x0C, 0xDF,
		0x3F, 0x87, 0x35, 0x1F, 0x2A, 0x9B, 0x88, 0xDD,
		0x92, 0x06, 0x83, 0x51, 0x4B, 0x5E, 0x8E, 0xDB,
		0xD2, 0x7F, 0xC1, 0x1B, 0x39, 0x9E, 0xFB, 0x47,
		0x71, 0x75, 0xF2, 0x5B, 0xCC, 0xF9, 0xAA, 0x4D,
		0x5C, 0x9F, 0x34, 0xFF, 0x52, 0x6F, 0x6A, 0xAB,
		0x84, 0x82, 0xAA, 0x62, 0x58, 0x59, 0x7D, 0x78,
		0xA0, 0x37, 0x26, 0x4C, 0x8A, 0x8F, 0xF0, 0x92,
		0xE9, 0xF7, 0x05, 0xAD, 0x77, 0x61, 0x32, 0x4D,
		/* e */
		0x01, 0x00, 0x01,
		/* d */
		0x12, 0x98, 0xD7, 0xA7, 0x6D, 0x15, 0x95, 0xC3,
		0x38, 0xC7, 0x63, 0xEB, 0x45, 0x78, 0xFF, 0x45,
		0xF5, 0x8A, 0x6B, 0xBF, 0xC8, 0xBE, 0xE7, 0x4B,
		0xEF, 0x2C, 0xF5, 0x67, 0x54, 0x41, 0x32, 0xA8,
		0xA3, 0x15, 0x2F, 0xC2, 0xDE, 0xDB, 0xC0, 0xF9,
		0xEC, 0x71, 0x5B, 0x03, 0xF8, 0x18, 0xD9, 0x5A,
		0x2E, 0x9F, 0xEE, 0x99, 0xCA, 0x37, 0x7E, 0xC7,
		0xAF, 0x9D, 0x60, 0x26, 0xC0, 0x3B, 0x84, 0xB5,
		0x02, 0x4C, 0xEA, 0xD5, 0xD5, 0x68, 0x1B, 0xB4,
		0xEA, 0x56, 0xA6, 0x2C, 0xAA, 0xC5, 0xB6, 0x5E,
		0x52, 0xDA, 0xBE, 0xBC, 0x2E, 0x58, 0x6B, 0x4A,
		0xE4, 0x26, 0x42, 0xF4, 0x3E, 0x6A, 0xC7, 0x5E,
		0x40, 0xE2, 0x1C, 0x12, 0x06, 0xDE, 0xB5, 0xB9,
		0xB5, 0x23, 0xCA, 0xBB, 0xE6, 0x95, 0x9F, 0xF2,
		0x50, 0x67, 0x5E, 0xA3, 0x80, 0x63, 0x3F, 0xEA,
		0xA9, 0x87, 0x89, 0xAA, 0xFD, 0x9D, 0x2F, 0x3B,
		0x4C, 0xA4, 0xC4, 0xB0, 0xC5, 0x25, 0xEC, 0xD1,
		0x5C, 0xC3, 0x2C, 0x40, 0x1E, 0x28, 0x42, 0x1E,
		0xC7, 0x26, 0xAB, 0x9E, 0x5A, 0xB9, 0x22, 0xB9,
		0x44, 0x14, 0xC8, 0x1D, 0x51, 0x64, 0x4F, 0xE3,
		0xA0, 0x04, 0x48, 0x04, 0x55, 0x4B, 0xCE, 0x8B,
		0x11, 0x00, 0xCB, 0x85, 0x93, 0x8B, 0x06, 0xCE,
		0x33, 0x0D, 0xA6, 0xB8, 0xBA, 0x5F, 0xC0, 0x3C,
		0x5A, 0xB0, 0x0B, 0x0C, 0x68, 0xE2, 0x4F, 0x4D,
		0xCC, 0x94, 0x54, 0x85, 0xCD, 0x8D, 0x46, 0x79,
		0xD7, 0x2C, 0xD9, 0x67, 0x27, 0xED, 0x1C, 0x22,
		0x6C, 0x90, 0xC2, 0xB1, 0x23, 0xD2, 0x38, 0x59,
		0x79, 0x12, 0x54, 0xAD, 0xEE, 0xFA, 0xE3, 0x31,
		0x87, 0x51, 0x88, 0x15, 0xB9, 0x24, 0x73, 0x5A,
		0x32, 0xA4, 0xD3, 0xCE, 0xEB, 0xC5, 0xAE, 0x9C,
		0x8E, 0xD6, 0x64, 0x00, 0x9E, 0x04, 0x5D, 0x28,
		0xED, 0x99, 0x90, 0xF9, 0xA0, 0xBA, 0x55, 0x81
};
#endif /* RSA_KEYSIZE == 2048 */

#if RSA_KEYSIZE == 1024

#define N_LEN_B (16*8)
#define E_LEN_B  (3)
#define D_LEN_B (16*8)
#define ALL_LEN_B (N_LEN_B+E_LEN_B+D_LEN_B)

const uint8_t rsa_key_blob[] PROGMEM = {
		/* n */
		0xC8, 0xE2, 0x5C, 0xE3, 0x38, 0xE2, 0x05, 0x03,
		0xBA, 0xFD, 0x33, 0x9A, 0xC4, 0x71, 0x5A, 0xD9,
		0x04, 0x8D, 0xF0, 0xDD, 0x03, 0x14, 0xCF, 0x73,
		0xCD, 0xE0, 0xBA, 0xFB, 0x21, 0xFF, 0x62, 0xF6,
		0x45, 0x17, 0x39, 0xBC, 0xDC, 0x9D, 0x06, 0x9F,
		0xC8, 0xED, 0xA0, 0xFE, 0xC4, 0xD2, 0x96, 0xE8,
		0x51, 0x40, 0x31, 0x1A, 0x05, 0x0B, 0xF6, 0x7B,
		0x8E, 0x9A, 0x96, 0x67, 0x3B, 0xDE, 0xA3, 0x7D,
		0x3C, 0x0E, 0x46, 0x00, 0x1F, 0x94, 0x25, 0xA5,
		0xC4, 0x71, 0x26, 0x5C, 0xE9, 0xC4, 0x43, 0xF3,
		0xF5, 0x83, 0x55, 0x97, 0x1C, 0x10, 0x20, 0x0E,
		0x4D, 0x2D, 0xF1, 0xF0, 0x72, 0x5D, 0x84, 0x01,
		0x28, 0x4D, 0x16, 0xAA, 0x95, 0xDF, 0x52, 0xBB,
		0x2E, 0x34, 0xC1, 0x77, 0x9F, 0x18, 0x30, 0x97,
		0x0A, 0x86, 0x70, 0xC6, 0xEE, 0xE2, 0x84, 0xD6,
		0x85, 0x11, 0x14, 0xAC, 0x70, 0x7A, 0x29, 0xB9,
		/* e */
		0x01, 0x00, 0x01,
		/* d */
		0x2E, 0xD2, 0xF4, 0xAB, 0xD2, 0x1A, 0x7C, 0xE8,
		0xBD, 0xB1, 0x3F, 0x52, 0x17, 0xF0, 0x04, 0x29,
		0x09, 0xB2, 0xBC, 0x2E, 0x08, 0x05, 0xD5, 0xC5,
		0x3C, 0x7B, 0xE1, 0x99, 0x9F, 0xF7, 0x13, 0x6E,
		0x2B, 0xF4, 0x99, 0xF4, 0xB7, 0xC7, 0xB2, 0x3F,
		0x94, 0x6A, 0x2F, 0x58, 0xB3, 0x30, 0x13, 0x14,
		0xDB, 0xBB, 0x86, 0xB6, 0x75, 0xEE, 0xAF, 0xE9,
		0x64, 0xF8, 0x0B, 0x8E, 0xCC, 0x99, 0xBC, 0xA9,
		0x8E, 0x57, 0xBA, 0x07, 0xBD, 0x0A, 0xDD, 0xD9,
		0x12, 0xA5, 0xFF, 0x08, 0x4F, 0x67, 0x64, 0xF7,
		0x04, 0xD5, 0xC0, 0xBC, 0x98, 0x2F, 0xF0, 0x8A,
		0x3A, 0x3D, 0xFF, 0xB2, 0xF7, 0x12, 0xA8, 0x71,
		0x4C, 0xF8, 0x21, 0x3B, 0xD4, 0x9C, 0x78, 0x93,
		0x5A, 0xC1, 0x25, 0xB4, 0x5F, 0x95, 0x59, 0xD4,
		0x8D, 0x7D, 0xF4, 0x07, 0x3C, 0xB5, 0x8A, 0x4C,
		0xFF, 0x94, 0x68, 0xCD, 0x1A, 0x12, 0xC5, 0xC1
};

#endif /* RSA_KEYSIZE */

void load_rsa_key_blob(rsa_ctx_t* ctx){
	if(ctx->modulus.wordv){
		free(ctx->modulus.wordv);
	}
	ctx->modulus.wordv = malloc(ALL_LEN_B);
	if(ctx->modulus.wordv==NULL){
		cli_putstr_P(PSTR("\r\nERROR: OUT OF MEMORY!!!"));
		return;
	}
	ctx->modulus.info = ctx->privexp.info = ctx->pubexp.info = 0;
	memcpy_P(ctx->modulus.wordv, rsa_key_blob, ALL_LEN_B);
	ctx->modulus.length_B=N_LEN_B;
	ctx->pubexp.wordv = ctx->modulus.wordv+N_LEN_B;
	ctx->pubexp.length_B = E_LEN_B;
	ctx->privexp.wordv = ctx->pubexp.wordv+E_LEN_B;
	ctx->privexp.length_B = D_LEN_B;

	bigint_changeendianess(&(ctx->modulus));
	bigint_changeendianess(&(ctx->pubexp));
	bigint_changeendianess(&(ctx->privexp));

	bigint_adjust(&(ctx->modulus));
	bigint_adjust(&(ctx->pubexp));
	bigint_adjust(&(ctx->privexp));
}
