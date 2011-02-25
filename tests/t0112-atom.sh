#!/bin/sh

test_description='Check content on atom feed'
. ./setup.sh

test_expect_success 'fetch feed' 'cgit_url "foo/atom/" >tmp'

test_expect_success 'find feed open tag' 'grep -e "^<feed xmlns=.http://www\.w3\.org/2005/Atom.>" tmp'

test_expect_success 'find title' 'grep -e "^<title>foo" tmp'

test_expect_success 'find entry title' 'grep -e "^<title>commit 5</title>" tmp'

test_expect_success 'find entry diffstat' 'grep -e "^<div class=.diffstat-header.>" tmp'

test_expect_success 'find entry diff' 'grep -e "<table summary=.diff. class=.diff.>" tmp'

test_expect_success 'find diff content 1' 'grep -e "diff --git a/file-1 b/file-1" tmp'

test_expect_success 'find diff content 2' 'grep -e "@@ -0,0 +1 @@" tmp'

test_expect_success 'find diff content 3' 'grep -e "+1<" tmp'

test_done
