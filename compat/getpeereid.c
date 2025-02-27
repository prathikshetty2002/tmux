/*
 * Copyright (c) 2022 Nicholas Marriott <nicholas.marriott@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>

#ifdef HAVE_UCRED_H
#include <ucred.h>
#endif

#include "compat.h"

int
getpeereid(int s, uid_t *uid, gid_t *gid)
{
#ifdef HAVE_SO_PEERCRED
	struct ucred	uc;
	int		len = sizeof uc;

	if (getsockopt(s, SOL_SOCKET, SO_PEERCRED, &uc, &len) == -1)
		return (-1);
	*uid = uc.uid;
	*gid = uc.gid;
	return (0);
#elif defined(HAVE_GETPEERUCRED)
int
getpeereid(int s, uid_t *uid, gid_t *gid)
{
        ucred_t *ucred = NULL;

        if (getpeerucred(s, &ucred) == -1)
                return (-1);
        if ((*uid = ucred_geteuid(ucred)) == -1)
                return (-1);
        if ((*gid = ucred_getrgid(ucred)) == -1)
                return (-1);
        ucred_free(ucred);
        return (0);
}
#else
	return (getuid());
#endif
}
