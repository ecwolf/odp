/* Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#include "config.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <odp_api.h>
#include <odph_debug.h>
#include <odp/helper/odph_api.h>
#include <odp/helper/ip.h>
#include <odp/helper/odph_iplookuptablev6.h>

typedef __int128 _uint128_t;
typedef unsigned __int128 uint128_t;

static void print_prefix_info_ipv6(
		const char *msg, _uint128_t ip, uint8_t cidr)
{
	int i = 0;
	uint8_t *ptr = (uint8_t *)(&ip);

	printf("%s IP prefix: ", msg);
	for (i = 15; i >= 0; i--) {
		if (i == 13 || i == 11 || i == 9 || i == 7 || i == 5 || i == 3 || i == 1)
			printf(":");
		printf("%02x", ptr[i]);
	}
	printf("/%d\n", cidr);
}
//"%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x"

/*
 * Basic sequence of operations for a single key:
 *	- put short prefix
 *	- put long prefix
 *	- get (hit long prefix)
 *	- remove long prefix
 *	- get (hit short prefix)
 */
static int test_ipv6_lookup_table(void)
{
	odph_iplookupv6_prefix_t prefix1, prefix2;
	odph_table_t table;
	int ret;
	uint64_t value1 = 1, value2 = 2, result = 0;
	_uint128_t lkp_ip = 0, lkp_ip2 = 0;
	printf("Table create\n");
	table = odph_iplookupv6_table_create(
			"prefix_test", 0, 0, sizeof(_uint128_t));
	if (table == NULL) {
		printf("IPv6 prefix lookup table creation failed\n");
		return -1;
	}

	printf("Parse 1\n");
	ret = odph_ipv6_addr_parse(&prefix1.ip, "2001:0db8:85a3:08d3:1319:8a2e:0370:7334");
	if (ret < 0) {
		printf("Failed to get IPv6 addr from str\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}
	prefix1.cidr = 64;
	print_prefix_info_ipv6("Parse 1", prefix1.ip, prefix1.cidr);
	/*ret = odph_ipv4_addr_parse(&prefix1.ip, "192.168.0.0");
	if (ret < 0) {
		printf("Failed to get IPv6 addr from str\n");
		odph_iplookup_table_destroy(table);
		return -1;
	}
	prefix1.cidr = 11;*/

	printf("Parse 2\n");
	ret = odph_ipv6_addr_parse(&prefix2.ip, "1319:8a2e:0370:7335:2001:0db8:ffff:08d3");
	if (ret < 0) {
		printf("Failed to get IPv6 addr from str\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}
	prefix2.cidr = 64;
	print_prefix_info_ipv6("Parse 1", prefix1.ip, prefix1.cidr);
	/*ret = odph_ipv4_addr_parse(&prefix2.ip, "192.168.0.0");
	if (ret < 0) {
		printf("Failed to get IP addr from str\n");
		odph_iplookup_table_destroy(table);
		return -1;
	}
	prefix2.cidr = 24;*/

	printf("Parse 3\n");
	ret = odph_ipv6_addr_parse(&lkp_ip, "2001:0db8:85a3:08d3:1319:8a2e:0370:7334");
	if (ret < 0) {
		printf("Failed to get IPv6 addr from str\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}
	print_prefix_info_ipv6("Parse lkp",lkp_ip ,128);

	printf("Parse 4\n");
	ret = odph_ipv6_addr_parse(&lkp_ip2, "1319:8a2e:0370:7335:2001:0db8:ffff:08d3");
	if (ret < 0) {
		printf("Failed to get IPv6 addr from str\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}

	/*ret = odph_ipv4_addr_parse(&lkp_ip, "192.168.0.1");
	if (ret < 0) {
		printf("Failed to get IP addr from str\n");
		odph_iplookup_table_destroy(table);
		return -1;
	}*/

	/* test with standard put/get/remove functions */
	printf("TABLE PUT\n");
	ret = odph_iplookupv6_table_put_value(table, &prefix1, &value1);
	printf("Add IP prefix: 2001:0db8:85a3:08d3:1319:8a2e:0370:7334  ----\n");
	print_prefix_info_ipv6("Add", prefix1.ip, prefix1.cidr);
	if (ret < 0) {
		printf("Failed to add IPv6 prefix\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}

	/*ret = odph_iplookup_table_put_value(table, &prefix1, &value1);
	print_prefix_info("Add", prefix1.ip, prefix1.cidr);
	if (ret < 0) {
		printf("Failed to add ip prefix\n");
		odph_iplookup_table_destroy(table);
		return -1;
	}*/
	printf("TABLE GET\n");
	ret = odph_iplookupv6_table_get_value(table, &lkp_ip, &result, 0);
	print_prefix_info_ipv6("Lkp", lkp_ip, 128);
	if (ret < 0 || result != 1) {
		printf("Failed to find IPv6 longest prefix 1\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}

	/*ret = odph_iplookup_table_get_value(table, &lkp_ip, &result, 0);
	print_prefix_info("Lkp", lkp_ip, 32);
	if (ret < 0 || result != 1) {
		printf("Failed to find longest prefix\n");
		odph_iplookup_table_destroy(table);
		return -1;
	}*/

	/* add a longer prefix */
	printf("TABLE PUT 2\n");
	ret = odph_iplookupv6_table_put_value(table, &prefix2, &value2);
	printf("Add IP prefix: 1319:8a2e:0370:7335:2001:0db8:ffff:08d3  ----\n");
	print_prefix_info_ipv6("Add", prefix2.ip, prefix2.cidr);
	if (ret < 0) {
		printf("Failed to add IPv6 prefix\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}

	/*ret = odph_iplookup_table_put_value(table, &prefix2, &value2);
	print_prefix_info("Add", prefix2.ip, prefix2.cidr);
	if (ret < 0) {
		printf("Failed to add ip prefix\n");
		odph_iplookup_table_destroy(table);
		return -1;
	}*/
	printf("TABLE GET 2\n");
	ret = odph_iplookupv6_table_get_value(table, &lkp_ip2, &result, 0);
	print_prefix_info_ipv6("Lkp", lkp_ip2, 128);
	if (ret < 0 || result != 2) {
		printf("Failed to find IPv6 longest prefix 2\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}

	/*ret = odph_iplookup_table_get_value(table, &lkp_ip, &result, 0);
	print_prefix_info("Lkp", lkp_ip, 32);
	if (ret < 0 || result != 2) {
		printf("Failed to find longest prefix\n");
		odph_iplookup_table_destroy(table);
		return -1;
	}*/

	printf("Finish ADD and GET\n");

	printf("TABLE REMOVE Value\n");
	ret = odph_iplookupv6_table_remove_value(table, &prefix2);
	print_prefix_info_ipv6("Del", prefix2.ip, prefix2.cidr);
	if (ret < 0) {
		printf("Failed to delete IPv6 prefix\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}

	/*ret = odph_iplookup_table_remove_value(table, &prefix2);
	print_prefix_info("Del", prefix2.ip, prefix2.cidr);
	if (ret < 0) {
		printf("Failed to delete ip prefix\n");
		odph_iplookup_table_destroy(table);
		return -1;
	}*/

	printf("TABLE GET\n");
	ret = odph_iplookupv6_table_get_value(table, &lkp_ip, &result, 0);
	print_prefix_info_ipv6("Lkp", lkp_ip, 128);
	if (ret < 0 || result != 1) {
		printf("Error: found IPv6 result after deleting\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}

	/*ret = odph_iplookup_table_get_value(table, &lkp_ip, &result, 0);
	print_prefix_info("Lkp", lkp_ip, 32);
	if (ret < 0 || result != 1) {
		printf("Error: found result ater deleting\n");
		odph_iplookup_table_destroy(table);
		return -1;
	}*/

	printf("TABLE REMOVE Value\n");
	ret = odph_iplookupv6_table_remove_value(table, &prefix1);
	print_prefix_info_ipv6("Del", prefix1.ip, prefix1.cidr);
	if (ret < 0) {
		printf("Failed to delete IPv6 prefix\n");
		odph_iplookupv6_table_destroy(table);
		return -1;
	}

	/*ret = odph_iplookup_table_remove_value(table, &prefix1);
	print_prefix_info("Del", prefix1.ip, prefix1.cidr);
	if (ret < 0) {
		printf("Failed to delete prefix\n");
		odph_iplookup_table_destroy(table);
		return -1;
	}*/

	printf("TABLE REMOVE destroy\n");
	odph_iplookupv6_table_destroy(table);
	return 0;
}

int main(int argc ODPH_UNUSED, char *argv[] ODPH_UNUSED)
{
	odp_instance_t instance;
	int ret = 0;

	ret = odp_init_global(&instance, NULL, NULL);
	if (ret != 0) {
		fprintf(stderr, "Error: ODP global init failed.\n");
		exit(EXIT_FAILURE);
	}

	ret = odp_init_local(instance, ODP_THREAD_WORKER);
	if (ret != 0) {
		fprintf(stderr, "Error: ODP local init failed.\n");
		exit(EXIT_FAILURE);
	}

	if (test_ipv6_lookup_table() < 0)
		printf("Test failed\n");
	else
		printf("All tests passed\n");

	if (odp_term_local()) {
		fprintf(stderr, "Error: ODP local term failed.\n");
		exit(EXIT_FAILURE);
	}

	if (odp_term_global(instance)) {
		fprintf(stderr, "Error: ODP global term failed.\n");
		exit(EXIT_FAILURE);
	}

	return ret;
}
