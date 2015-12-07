#include <unistd.h>
struct commitinfo {
	const git_oid *id;

	char oid[GIT_OID_HEXSZ + 1];
	char parentoid[GIT_OID_HEXSZ + 1];

	const git_signature *author;
	const char *summary;
	const char *msg;

	git_diff_stats *stats;
	git_diff       *diff;
	git_commit     *commit;
	git_commit     *parent;
	git_tree       *commit_tree;
	git_tree       *parent_tree;

	size_t addcount;
	size_t delcount;
	size_t filecount;
};

void
commitinfo_free(struct commitinfo *ci)
{
	if (!ci)
		return;

	/* TODO: print error ? */
	git_diff_stats_free(ci->stats);
	git_diff_free(ci->diff);
	git_commit_free(ci->commit);
}

struct commitinfo *
commitinfo_getbyoid(const git_oid *id)
{
	struct commitinfo *ci;
	int error;

	if (!(ci = calloc(1, sizeof(struct commitinfo))))
		err(1, "calloc");

	ci->id = id;
	if (git_commit_lookup(&(ci->commit), repo, id))
		goto err;

	/* TODO: show tags when commit has it */
	git_oid_tostr(ci->oid, sizeof(ci->oid), git_commit_id(ci->commit));
	git_oid_tostr(ci->parentoid, sizeof(ci->parentoid), git_commit_parent_id(ci->commit, 0));

	ci->author = git_commit_author(ci->commit);
	ci->summary = git_commit_summary(ci->commit);
	ci->msg = git_commit_message(ci->commit);

	if ((error = git_commit_tree(&(ci->commit_tree), ci->commit)))
		goto err; /* TODO: handle error */
	if (!(error = git_commit_parent(&(ci->parent), ci->commit, 0))) {
		if ((error = git_commit_tree(&(ci->parent_tree), ci->parent)))
			goto err;
	} else {
		ci->parent = NULL;
		ci->parent_tree = NULL;
	}

	if ((error = git_diff_tree_to_tree(&(ci->diff), repo, ci->parent_tree, ci->commit_tree, NULL)))
		goto err;
	if (git_diff_get_stats(&(ci->stats), ci->diff))
		goto err;

	ci->addcount = git_diff_stats_insertions(ci->stats);
	ci->delcount = git_diff_stats_deletions(ci->stats);
	ci->filecount = git_diff_stats_files_changed(ci->stats);

	/* TODO: show tag when commit has it */

	return ci;

err:
	commitinfo_free(ci);
	free(ci);

	return NULL;
}

	fputs("</head>\n<body>\n<center>\n", fp);
	fprintf(fp, "<h1><img src=\"%slogo.png\" alt=\"\" width=\"32\" height=\"32\" /> %s <span class=\"desc\">%s</span></h1>\n",
	fputs("\n</center>\n<hr/>\n<pre>", fp);
writeblobhtml(FILE *fp, const git_blob *blob)
	xmlencode(fp, git_blob_rawcontent(blob), (size_t)git_blob_rawsize(blob));
}
void
printcommit(FILE *fp, struct commitinfo *ci)
{
		relpath, ci->oid, ci->oid);
	if (ci->parentoid[0])
			relpath, ci->parentoid, ci->parentoid);
#if 0
#endif
	if (ci->author) {
		xmlencode(fp, ci->author->name, strlen(ci->author->name));
		xmlencode(fp, ci->author->email, strlen(ci->author->email));
		xmlencode(fp, ci->author->email, strlen(ci->author->email));
		printtime(fp, &(ci->author->when));
	if (ci->msg)
		xmlencode(fp, ci->msg, strlen(ci->msg));

printshowfile(struct commitinfo *ci)
	const git_diff_delta *delta;
	const git_diff_hunk *hunk;
	const git_diff_line *line;
	git_patch *patch;
	git_buf statsbuf;
	size_t ndeltas, nhunks, nhunklines;
	size_t i, j, k;
	char path[PATH_MAX];
	snprintf(path, sizeof(path), "commit/%s.html", ci->oid);
	/* check if file exists if so skip it */
	if (!access(path, F_OK))
		return;
	fp = efopen(path, "w+b");
	printcommit(fp, ci);
	memset(&statsbuf, 0, sizeof(statsbuf));
	if (ci->stats) {
		if (!git_diff_stats_to_buf(&statsbuf, ci->stats,
			if (statsbuf.ptr && statsbuf.ptr[0]) {
				fprintf(fp, "<b>Diffstat:</b>\n");
				fputs(statsbuf.ptr, fp);
			}

	ndeltas = git_diff_num_deltas(ci->diff);
		if (git_patch_from_diff(&patch, ci->diff, i)) {
	git_buf_free(&statsbuf);
	struct commitinfo *ci;
	int ret = 0;
		if (!(ci = commitinfo_getbyoid(&id)))
			break;
		if (ci->summary) {
			fprintf(fp, "<a href=\"%scommit/%s.html\">", relpath, ci->oid);
			xmlencode(fp, ci->summary, strlen(ci->summary));
		if (ci->author)
			xmlencode(fp, ci->author->name, strlen(ci->author->name));

		if (ci->author)
			printtime(fp, &(ci->author->when));
		fprintf(fp, "%zu", ci->filecount);
		fprintf(fp, "+%zu", ci->addcount);
		fprintf(fp, "-%zu", ci->delcount);
		printshowfile(ci);
		commitinfo_free(ci);

	return ret;
printcommitatom(FILE *fp, struct commitinfo *ci)
	fprintf(fp, "<id>%s</id>\n", ci->oid);
	if (ci->author) {
		printtimez(fp, &(ci->author->when));
	if (ci->summary) {
		xmlencode(fp, ci->summary, strlen(ci->summary));
	fprintf(fp, "commit %s\n", ci->oid);
	if (ci->parentoid[0])
		fprintf(fp, "parent %s\n", ci->parentoid);
#if 0
#endif
	if (ci->author) {
		xmlencode(fp, ci->author->name, strlen(ci->author->name));
		xmlencode(fp, ci->author->email, strlen(ci->author->email));
		printtime(fp, &(ci->author->when));
	if (ci->msg)
		xmlencode(fp, ci->msg, strlen(ci->msg));
	if (ci->author) {
		xmlencode(fp, ci->author->name, strlen(ci->author->name));
		xmlencode(fp, ci->author->email, strlen(ci->author->email));
	struct commitinfo *ci;
		if (!(ci = commitinfo_getbyoid(&id)))
			break;
		printcommitatom(fp, ci);
		commitinfo_free(ci);
	git_index *index;
	fputs("<table><thead>\n"
	      "<tr><td>Mode</td><td>Name</td><td align=\"right\">Size</td></tr>\n"
	      "</thead><tbody>\n", fp);
	git_repository_index(&index, repo);
