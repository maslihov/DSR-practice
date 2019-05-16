/* $NetBSD: rm.c,v 1.52 2012/06/13 07:35:37 dholland Exp $ */

/*-
 * Copyright (c) 1990, 1993, 1994, 2003
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "rm.h"
#include <sys/cdefs.h>


#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <fts.h>
#include <grp.h>
#include <locale.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int dflag, eval, fflag, iflag, Pflag, stdin_ok, vflag, Wflag;
static sig_atomic_t pinfo;

static void	rm_file(char **);
static void	rm_tree(char **);

/*
 * For the sake of the `-f' flag, check whether an error number indicates the
 * failure of an operation due to an non-existent file, either per se (ENOENT)
 * or because its filename argument was illegal (ENAMETOOLONG, ENOTDIR).
 */
#define NONEXISTENT(x) \
    ((x) == ENOENT || (x) == ENAMETOOLONG || (x) == ENOTDIR)



int rm_main(int argc, char **argv, int fl_r)
{
	int rflag;
	Pflag = 0;
	dflag = 1;
	fflag = 1;
	iflag = 0;
	rflag =  fl_r;
	vflag = 0;
	Wflag = 0;

	if (rflag)
		rm_tree(argv);
	else
		rm_file(argv);

	return 0;
}

static void
rm_tree(char **argv)
{
	FTS *fts;
	FTSENT *p;
	int flags, needstat, rval;
			
	/*
	 * Remove a file hierarchy.  If forcing removal (-f), or interactive
	 * (-i) or can't ask anyway (stdin_ok), don't stat the file.
	 */
	needstat = !fflag && !iflag && stdin_ok;

	/*
	 * If the -i option is specified, the user can skip on the pre-order
	 * visit.  The fts_number field flags skipped directories.
	 */
#define	SKIPPED	1

	flags = FTS_PHYSICAL;
	if (!needstat)
		flags |= FTS_NOSTAT;
	if (Wflag)
		flags |= FTS_WHITEOUT;
	if ((fts = fts_open(argv, flags, NULL)) == NULL)
		err(1, "fts_open failed");
	while ((p = fts_read(fts)) != NULL) {
	
		switch (p->fts_info) {
		case FTS_DNR:
			if (!fflag || p->fts_errno != ENOENT) {
				warnx("%s: %s", p->fts_path,
						strerror(p->fts_errno));
				eval = 1;
			}
			continue;
		case FTS_ERR:
			errx(EXIT_FAILURE, "%s: %s", p->fts_path,
					strerror(p->fts_errno));
			/* NOTREACHED */
		case FTS_NS:
			/*
			 * FTS_NS: assume that if can't stat the file, it
			 * can't be unlinked.
			 */
			if (fflag && NONEXISTENT(p->fts_errno))
				continue;
			if (needstat) {
				warnx("%s: %s", p->fts_path,
						strerror(p->fts_errno));
				eval = 1;
				continue;
			}
			break;

		case FTS_DP:
			/* Post-order: see if user skipped. */
			if (p->fts_number == SKIPPED)
				continue;
			break;
		}

		rval = 0;
		/*
		 * If we can't read or search the directory, may still be
		 * able to remove it.  Don't print out the un{read,search}able
		 * message unless the remove fails.
		 */
		switch (p->fts_info) {
		case FTS_DP:
		case FTS_DNR:
			rval = rmdir(p->fts_accpath);
			if (rval != 0 && fflag && errno == ENOENT)
				continue;
		default:
			
			rval = unlink(p->fts_accpath);
			if (rval != 0 && fflag && NONEXISTENT(errno))
				continue;
			break;
		}
		if (rval != 0) {
			//warn("%s", p->fts_path); //печатает при удалении
			eval = 1;
		} else if (vflag || pinfo) {
			pinfo = 0;
			//(void)printf("%s\n", p->fts_path);
		}
	}
	if (errno)
		err(1, "fts_read");
	fts_close(fts);
}

static void
rm_file(char **argv)
{
	struct stat sb;
	int rval;
	char *f;

	/*
	 * Remove a file.  POSIX 1003.2 states that, by default, attempting
	 * to remove a directory is an error, so must always stat the file.
	 */
	while ((f = *argv++) != NULL) {
		/* Assume if can't stat the file, can't unlink it. */
		if (lstat(f, &sb)) {
			if (Wflag) {
				sb.st_mode = S_IWUSR|S_IRUSR;
			} else {
				if (!fflag || !NONEXISTENT(errno)) {
					warn("%s", f);
					eval = 1;
				}
				continue;
			}
		} else if (Wflag) {
			warnx("%s: %s", f, strerror(EEXIST));
			eval = 1;
			continue;
		}

		if (S_ISDIR(sb.st_mode) && !dflag) {
			warnx("%s: is a directory", f);
			eval = 1;
			continue;
		}
		
		else if (S_ISDIR(sb.st_mode))
			rval = rmdir(f);
		else {
			rval = unlink(f);
		}
		if (rval && (!fflag || !NONEXISTENT(errno))) {
			warn("%s", f);
			eval = 1;
		}
		if (vflag && rval == 0)
			(void)printf("%s\n", f);
	}
}