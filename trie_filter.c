/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 226204 2007-01-01 19:32:10Z iliaa $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_trie_filter.h"

#include <iconv.h>
#include <datrie/trie.h>

/* If you declare any globals in php_trie_filter.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(trie_filter)
*/

/* True global resources - no need for thread safety here */
static int le_trie_filter;

/* {{{ trie_filter_functions[]
 *
 * Every user visible function must have an entry in trie_filter_functions[].
 */
zend_function_entry trie_filter_functions[] = {
	PHP_FE(confirm_trie_filter_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(trie_filter_load, NULL)
	PHP_FE(trie_filter_search, NULL)
	{NULL, NULL, NULL}	/* Must be the last line in trie_filter_functions[] */
};
/* }}} */

/* {{{ trie_filter_module_entry
 */
zend_module_entry trie_filter_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"trie_filter",
	trie_filter_functions,
	PHP_MINIT(trie_filter),
	PHP_MSHUTDOWN(trie_filter),
	PHP_RINIT(trie_filter),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(trie_filter),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(trie_filter),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TRIE_FILTER
ZEND_GET_MODULE(trie_filter)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("trie_filter.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_trie_filter_globals, trie_filter_globals)
    STD_PHP_INI_ENTRY("trie_filter.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_trie_filter_globals, trie_filter_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_trie_filter_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_trie_filter_init_globals(zend_trie_filter_globals *trie_filter_globals)
{
	trie_filter_globals->global_value = 0;
	trie_filter_globals->global_string = NULL;
}
*/
/* }}} */

static void php_trie_filter_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	Trie *trie = (Trie *)rsrc->ptr;
	trie_free(trie);
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(trie_filter)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	le_trie_filter = zend_register_list_destructors_ex(
			php_trie_filter_dtor, 
			NULL, PHP_TRIE_FILTER_RES_NAME, module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(trie_filter)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(trie_filter)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(trie_filter)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(trie_filter)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "trie_filter support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_trie_filter_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_trie_filter_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "trie_filter", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

PHP_FUNCTION(trie_filter_load)
{
	Trie *trie;
	char *path;
	int path_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", 
				&path, &path_len) == FAILURE) {
		RETURN_NULL();
	}

	trie = trie_new_from_file(path);
	if (!trie) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, 
				"Unable to load %s", path);
		RETURN_FALSE;
	}

	ZEND_REGISTER_RESOURCE(return_value, trie, le_trie_filter);
}

static size_t conv_to_alpha(iconv_t handle, const char *in, AlphaChar *out, size_t out_size)
{
	char *in_p = (char *) in;
	char *out_p = (char *) out;
	size_t in_left = strlen(in);
	size_t out_left = out_size * sizeof(AlphaChar);
	size_t res;
	const unsigned char *byte_p;

	res = iconv(handle, (char **)&in_p, &in_left, 
			&out_p, &out_left);
	if (res < 0)
		return res;

	/* convert UCS-4LE to AlphaChar string */
	res = 0;
	for (byte_p = (const unsigned char *) out;
			res < out_size && byte_p + 3 < (unsigned char*) out_p;
			byte_p += 4)
	{
		out[res++] = byte_p[0]
			| (byte_p[1] << 8)
			| (byte_p[2] << 16)
			| (byte_p[3] << 24);
	}
	if (res < out_size) {
		out[res] = 0;
	}

	return res;
}

static int trie_search(Trie *trie, const AlphaChar *text)
{
	TrieState *s;
	const AlphaChar *p;
	const AlphaChar *start;

	start = text;
	while (*start)
	{
		s = trie_root(trie);
		p = start;

		/* skip root */
		if (!trie_state_is_walkable(s, *p)) {
			start++;
			continue;
		}
		else
			trie_state_walk(s, *p++);

		while (trie_state_is_walkable(s, *p) && !trie_state_is_terminal(s))
			trie_state_walk(s, *p++);

		if (trie_state_is_terminal(s)) {
			return start - text;
		}

		start++;
	}

	return -1;
}

PHP_FUNCTION(trie_filter_search)
{
	Trie *trie;
	zval *trie_resource;
	char *text;
	int text_len;
	int conv_ret;

	iconv_t iconv_handle;
	AlphaChar *alpha_text;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", 
				&trie_resource, &text, &text_len) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(trie, Trie *, &trie_resource, -1, 
			PHP_TRIE_FILTER_RES_NAME, le_trie_filter);

	iconv_handle = iconv_open("UCS-4LE", "utf-8");
	if (!iconv_handle) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to open iconv");
		RETURN_FALSE;
	}

	alpha_text = emalloc(sizeof(AlphaChar) * text_len);
	if (!alpha_text) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to alloc memory");
		RETURN_FALSE;
	}

	conv_ret = conv_to_alpha(iconv_handle, text, alpha_text, text_len);
	if (conv_ret < 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to convert text to alphabet");
		RETURN_FALSE;
	}

	RETURN_LONG(trie_search(trie, alpha_text));
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
