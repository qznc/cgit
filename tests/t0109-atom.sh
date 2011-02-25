#!/bin/sh

. ./setup.sh

prepare_tests "Check content on atom feed page"

run_test 'fetch feed' 'cgit_url "foo/atom" >trash/tmp'


run_test 'find feed open tag' '
	grep -e "^<feed xmlns=.http://www\.w3\.org/2005/Atom.>" trash/tmp
'

run_test 'find title' 'grep -e "^<title>foo" trash/tmp'

run_test 'find entry title' 'grep -e "^<title>commit 5</title>" trash/tmp'

run_test 'find entry id' 'grep -e "^<id>406d4fbf364415255f2493a0e78e2a7f0d5db85b</id>" trash/tmp'

tests_done
