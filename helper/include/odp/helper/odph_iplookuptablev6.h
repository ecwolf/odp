/* Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:    BSD-3-Clause
 */

/**
 * @file
 *
 * ODP IP Lookup Table
 *
 * This is an implementation of the IP lookup table. The key of
 * this table is IPv6 address (128 bits), and the value can be
 * defined by user. This table uses the 16,8,8 ip lookup (longest
 * prefix matching) algorithm.
 */

#ifndef ODPH_IPLOOKUPV6_TABLE_H_
#define ODPH_IPLOOKUPV6_TABLE_H_

#include <odp/helper/table.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef __int128 _uint128_t;
typedef unsigned __int128 uint128_t;

/**
 * @addtogroup odph_iplookuptablev6 ODPH IP LOOKUP TABLE
 * @{
 */

/**
 * IP Lookup Prefix
 */
typedef struct {
	_uint128_t ip;  /**< IPv6 address */
	uint8_t cidr; /**< CIDR value for prefix matching */
} odph_iplookupv6_prefix_t;

/**
 * Create an IP lookup table
 *
 * @param name Name of the table to be created
 * @param ODP_IGNORED_1 Unused
 * @param ODP_IGNORED_2 Unused
 * @param value_size Byte size of each entry in the table
 *
 * @return Handle of the created ip lookup table
 * @retval NULL If table create failed
 */
odph_table_t odph_iplookupv6_table_create(const char *name,
					uint32_t ODP_IGNORED_1,
					uint32_t ODP_IGNORED_2,
					uint32_t value_size);

/**
 * Lookup an IP lookup table by name
 *
 * @param name Name of the table to be located
 *
 * @return Handle of the located ip lookup table
 * @retval NULL No table matching supplied name found
 */
odph_table_t odph_iplookupv6_table_lookup(const char *name);

/**
 * Destroy an IP lookup table
 *
 * @param table Handle of the ip lookup table to be destroyed
 *
 * @retval 0 Success
 * @retval < 0 Failure
 */
int odph_iplookupv6_table_destroy(odph_table_t table);

/**
 * Insert a key/value pair into an ip lookup table
 *
 * @param table Table into which value is to be stored
 * @param key   Address of an odph_iplookup_prefix_t to be used as key
 * @param value Value to be associated with specified key
 *
 * @retval >= 0 Success
 * @retval < 0  Failure
 */
int odph_iplookupv6_table_put_value(odph_table_t table, void *key, void *value);

/**
 * Retrieve a value from an iplookup table
 *
 * @param table Table from which value is to be retrieved
 * @param key   Address of an odph_iplookup_prefix_t to be used as key
 * @param[out] buffer Address of buffer to receive resulting value
 * @param buffer_size Size of supplied buffer
 *
 * @retval 0 Success
 * @retval 1 Success
 * @retval < 0 Failure
 */
int odph_iplookupv6_table_get_value(odph_table_t table, void *key,
				  void *buffer, uint32_t buffer_size);

/**
 * Remove a value from an iplookup table
 *
 * @param table Table from which value is to be removed
 * @param key   Address of odph_iplookup_prefix_t to be used as key
 *
 * @retval >= 0 Success
 * @retval < 0  Failure
 *
 */
int odph_iplookupv6_table_remove_value(odph_table_t table, void *key);

extern odph_table_ops_t odph_iplookupv6_table_ops; /**< @internal */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ODPH_IPLOOKUPV6_TABLE_H_ */
