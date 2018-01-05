/* Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#include "config.h"

#include <odp/helper/ip.h>

#include <stdio.h>
#include <string.h>

/** An unsigned 128-bit (16-byte) scalar type */
typedef __int128 _uint128_t;
typedef unsigned __int128 uint128_t;

int odph_ipv4_addr_parse(uint32_t *ip_addr, const char *str)
{
	unsigned byte[ODPH_IPV4ADDR_LEN];
	int i;

	memset(byte, 0, sizeof(byte));

	if (sscanf(str, "%u.%u.%u.%u",
		   &byte[0], &byte[1], &byte[2], &byte[3]) != ODPH_IPV4ADDR_LEN)
		return -1;

	for (i = 0; i < ODPH_IPV4ADDR_LEN; i++)
		if (byte[i] > 255)
			return -1;

	*ip_addr = byte[0] << 24 | byte[1] << 16 | byte[2] << 8 | byte[3];

	return 0;
}

int odph_ipv6_addr_parse(_uint128_t *ip_addr, const char *str)
{
	unsigned byte[ODPH_IPV6ADDR_LEN];
	int i;
	_uint128_t p_ip1, p_ip2, p_ip3, p_ip4;

	memset(byte, 0, sizeof(byte));

	if (sscanf(str, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
		   &byte[0], &byte[1], &byte[2], &byte[3], &byte[4], &byte[5], &byte[6], &byte[7], &byte[8], &byte[9],
		   &byte[10], &byte[11], &byte[12], &byte[13], &byte[14], &byte[15]) != ODPH_IPV6ADDR_LEN)
		return -1;

	for (i = 0; i < ODPH_IPV6ADDR_LEN; i++)
		if (byte[i] > 255)
			return -1;

	p_ip1 = byte[0] << 24 | byte[1] << 16 | byte[2] << 8 | byte[3];

	p_ip2 = byte[4] << 24 | byte[5] << 16 | byte[6] << 8 | byte[7];

	p_ip3 = byte[8] << 24 | byte[9] << 16 | byte[10] << 8 | byte[11];

	p_ip4 = byte[12] << 24 | byte[13] << 16 | byte[14] << 8 | byte[15];

	*ip_addr = p_ip1 << 96 | p_ip2 << 64 |  p_ip3 << 32 | p_ip4;

	return 0;
}
