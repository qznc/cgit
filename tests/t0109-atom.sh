#!/bin/sh

. ./setup.sh

prepare_tests "Check content on atom feed page"

run_test 'fetch feed' 'cgit_url "foo/atom" >trash/tmp'

run_test 'find feed open tag' 'grep -e "^<feed xmlns=.http://www\.w3\.org/2005/Atom.>" trash/tmp'

run_test 'find title' 'grep -e "^<title>foo" trash/tmp'

run_test 'find entry title' 'grep -e "^<title>commit 5</title>" trash/tmp'

run_test 'find entry diffstat' 'grep -e "^<div class=.diffstat-header.>" trash/tmp'

run_test 'find entry diff' 'grep -e "<table summary=.diff. class=.diff.>" trash/tmp'

run_test 'find diff content 1' 'grep -e "diff --git a/file-1 b/file-1" trash/tmp'

run_test 'find diff content 2' 'grep -e "@@ -0,0 +1 @@" trash/tmp'

run_test 'find diff content 3' 'grep -e "+1<" trash/tmp'

tests_done
