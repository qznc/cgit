/* ui-repolist.c: functions for generating the repolist page
 *
 * Copyright (C) 2006 Lars Hjemli
 *
 * Licensed under GNU General Public License v2
 *   (see COPYING for full license text)
 */

#include "cgit.h"

void cgit_print_repolist(struct cacheitem *item)
{
	struct repoinfo *repo;
	int i;
	char *last_group = NULL;

	cgit_print_docstart(cgit_root_title, item);
	cgit_print_pageheader(cgit_root_title, 0);

	html("<table class='list nowrap'>");
	if (cgit_index_header) {
		html("<tr class='nohover'><td colspan='4' class='include-block'>");
		html_include(cgit_index_header);
		html("</td></tr>");
	}
	html("<tr class='nohover'>"
	     "<th class='left'>Name</th>"
	     "<th class='left'>Description</th>"
	     "<th class='left'>Owner</th>"
	     "<th>Links</th></tr>\n");

	for (i=0; i<cgit_repolist.count; i++) {
		repo = &cgit_repolist.repos[i];
		if ((last_group == NULL && repo->group != NULL) ||
		    (last_group != NULL && repo->group == NULL) ||
		    (last_group != NULL && repo->group!= NULL &&
		     strcmp(repo->group, last_group))) {
			html("<tr class='nohover'><td colspan='4' class='repogroup'>");
			html_txt(repo->group);
			html("</td></tr>");
			last_group = repo->group;
		}
		htmlf("<tr><td class='%s'>",
		      repo->group ? "sublevel-repo" : "toplevel-repo");
		html_link_open(cgit_repourl(repo->url), repo->desc, NULL);
		html_txt(repo->name);
		html_link_close();
		html("</td><td>");
		html_ntxt(cgit_max_repodesc_len, repo->desc);
		html("</td><td>");
		html_txt(repo->owner);
		html("</td><td>");
		html_link_open(cgit_repourl(repo->url),
			       "Summary", "button");
		html("S</a>");
		html_link_open(cgit_pageurl(repo->name, "log", NULL),
			       "Log", "button");
		html("L</a>");
		html_link_open(cgit_pageurl(repo->name, "tree", NULL),
			       "Files", "button");
		html("F</a>");
		html("</td></tr>\n");
	}
	html("</table>");
	cgit_print_docend();
}
