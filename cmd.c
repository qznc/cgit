/* cmd.c: the cgit command dispatcher
 *
 * Copyright (C) 2008 Lars Hjemli
 *
 * Licensed under GNU General Public License v2
 *   (see COPYING for full license text)
 */

#include "cgit.h"
#include "cmd.h"
#include "cache.h"
#include "ui-shared.h"
#include "ui-atom.h"
#include "ui-blob.h"
#include "ui-clone.h"
#include "ui-commit.h"
#include "ui-diff.h"
#include "ui-log.h"
#include "ui-patch.h"
#include "ui-plain.h"
#include "ui-refs.h"
#include "ui-repolist.h"
#include "ui-snapshot.h"
#include "ui-stats.h"
#include "ui-summary.h"
#include "ui-tag.h"
#include "ui-tree.h"

static void HEAD_fn(struct cgit_context *ctx)
{
	cgit_clone_head(ctx);
}

static void atom_fn(struct cgit_context *ctx)
{
	cgit_print_atom(ctx->qry.head, ctx->qry.path, ctx->cfg.max_atom_items, ctx->repo->enable_atom_diff);
}

static void about_fn(struct cgit_context *ctx)
{
	if (ctx->repo)
		cgit_print_repo_readme(ctx->qry.path);
	else
		cgit_print_site_readme();
}

static void blob_fn(struct cgit_context *ctx)
{
	cgit_print_blob(ctx->qry.sha1, ctx->qry.path, ctx->qry.head);
}

static void commit_fn(struct cgit_context *ctx)
{
	cgit_print_commit(ctx->qry.sha1, ctx->qry.path);
}

static void diff_fn(struct cgit_context *ctx)
{
	cgit_print_diff(ctx->qry.sha1, ctx->qry.sha2, ctx->qry.path);
}

static void info_fn(struct cgit_context *ctx)
{
	cgit_clone_info(ctx);
}

static void log_fn(struct cgit_context *ctx)
{
	cgit_print_log(ctx->qry.sha1, ctx->qry.ofs, ctx->cfg.max_commit_count,
		       ctx->qry.grep, ctx->qry.search, ctx->qry.path, 1,
		       ctx->repo->enable_commit_graph);
}

static void ls_cache_fn(struct cgit_context *ctx)
{
	ctx->page.mimetype = "text/plain";
	ctx->page.filename = "ls-cache.txt";
	cgit_print_http_headers(ctx);
	cache_ls(ctx->cfg.cache_root);
}

static void objects_fn(struct cgit_context *ctx)
{
	cgit_clone_objects(ctx);
}

static void repolist_fn(struct cgit_context *ctx)
{
	cgit_print_repolist();
}

static void patch_fn(struct cgit_context *ctx)
{
	cgit_print_patch(ctx->qry.sha1, ctx->qry.path);
}

static void plain_fn(struct cgit_context *ctx)
{
	cgit_print_plain(ctx);
}

static void refs_fn(struct cgit_context *ctx)
{
	cgit_print_refs();
}

static void snapshot_fn(struct cgit_context *ctx)
{
	cgit_print_snapshot(ctx->qry.head, ctx->qry.sha1, ctx->qry.path,
			    ctx->repo->snapshots, ctx->qry.nohead);
}

static void stats_fn(struct cgit_context *ctx)
{
	cgit_show_stats(ctx);
}

static void summary_fn(struct cgit_context *ctx)
{
	cgit_print_summary();
}

static void tag_fn(struct cgit_context *ctx)
{
	cgit_print_tag(ctx->qry.sha1);
}

static void tree_fn(struct cgit_context *ctx)
{
	cgit_print_tree(ctx->qry.sha1, ctx->qry.path);
}

#define def_cmd(name, want_repo, want_layout, want_vpath) \
	{#name, name##_fn, want_repo, want_layout, want_vpath}

struct cgit_cmd *cgit_get_cmd(struct cgit_context *ctx)
{
	static struct cgit_cmd cmds[] = {
		def_cmd(HEAD, 1, 0, 0),
		def_cmd(atom, 1, 0, 0),
		def_cmd(about, 0, 1, 0),
		def_cmd(blob, 1, 0, 0),
		def_cmd(commit, 1, 1, 1),
		def_cmd(diff, 1, 1, 1),
		def_cmd(info, 1, 0, 0),
		def_cmd(log, 1, 1, 1),
		def_cmd(ls_cache, 0, 0, 0),
		def_cmd(objects, 1, 0, 0),
		def_cmd(patch, 1, 0, 1),
		def_cmd(plain, 1, 0, 0),
		def_cmd(refs, 1, 1, 0),
		def_cmd(repolist, 0, 0, 0),
		def_cmd(snapshot, 1, 0, 0),
		def_cmd(stats, 1, 1, 1),
		def_cmd(summary, 1, 1, 0),
		def_cmd(tag, 1, 1, 0),
		def_cmd(tree, 1, 1, 1),
	};
	int i;

	if (ctx->qry.page == NULL) {
		if (ctx->repo)
			ctx->qry.page = "summary";
		else
			ctx->qry.page = "repolist";
	}

	for(i = 0; i < sizeof(cmds)/sizeof(*cmds); i++)
		if (!strcmp(ctx->qry.page, cmds[i].name))
			return &cmds[i];
	return NULL;
}
