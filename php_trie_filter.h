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
  | Author: liwei <liwei@anbutu.com>                                     |
  +----------------------------------------------------------------------+
*/

/* $Id: header 226204 2007-01-01 19:32:10Z iliaa $ */

#include <iconv.h>
#include <datrie/trie.h>

#ifndef PHP_TRIE_FILTER_H
#define PHP_TRIE_FILTER_H

extern zend_module_entry trie_filter_module_entry;
#define phpext_trie_filter_ptr &trie_filter_module_entry

#ifdef PHP_WIN32
#define PHP_TRIE_FILTER_API __declspec(dllexport)
#else
#define PHP_TRIE_FILTER_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define ALPHA_CHARSET	"UCS-4LE"
#define PHP_TRIE_FILTER_RES_NAME "Trie tree filter"

PHP_MINIT_FUNCTION(trie_filter);
PHP_MSHUTDOWN_FUNCTION(trie_filter);
PHP_RINIT_FUNCTION(trie_filter);
PHP_RSHUTDOWN_FUNCTION(trie_filter);
PHP_MINFO_FUNCTION(trie_filter);

PHP_FUNCTION(trie_filter_load);
PHP_FUNCTION(trie_filter_search);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(trie_filter)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(trie_filter)
*/

#ifdef ZTS
#define TRIE_FILTER_G(v) TSRMG(trie_filter_globals_id, zend_trie_filter_globals *, v)
#else
#define TRIE_FILTER_G(v) (trie_filter_globals.v)
#endif

#endif	/* PHP_TRIE_FILTER_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
