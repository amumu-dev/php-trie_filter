<?php

$trie = trie_filter_load("/tmp/trie.tri");
if (!$trie)
{
    die("failed to load trie filter\n");
}

var_dump(trie_filter_search($trie, "google"));
var_dump(trie_filter_search($trie, "baidu"));
var_dump(trie_filter_search($trie, "seo"));
var_dump(trie_filter_search($trie, "aqq游戏"));
var_dump(trie_filter_search($trie, "李炜"));
var_dump(trie_filter_search($trie, "唉声唉声唉声唉声唉声唉声叹气"));
var_dump(trie_filter_search($trie, "php"));
