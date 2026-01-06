/* idvec.h - XNU compatibility layer for Hurd idvec
   Minimal implementation for auth server */

#ifndef _IDVEC_H
#define _IDVEC_H

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/* A vector of ids (uids or gids) */
struct idvec {
    uid_t *ids;
    size_t num;
    size_t alloced;
};

/* Initialize an empty idvec */
static inline void
idvec_init(struct idvec *idvec)
{
    idvec->ids = NULL;
    idvec->num = 0;
    idvec->alloced = 0;
}

/* Free the contents of an idvec */
static inline void
idvec_free_contents(struct idvec *idvec)
{
    if (idvec->ids) {
        free(idvec->ids);
        idvec->ids = NULL;
    }
    idvec->num = 0;
    idvec->alloced = 0;
}

/* Ensure space for at least AMOUNT more ids */
static inline int
idvec_ensure(struct idvec *idvec, size_t amount)
{
    size_t needed = idvec->num + amount;
    if (needed > idvec->alloced) {
        size_t new_size = idvec->alloced ? idvec->alloced * 2 : 8;
        while (new_size < needed)
            new_size *= 2;
        uid_t *new_ids = realloc(idvec->ids, new_size * sizeof(uid_t));
        if (!new_ids)
            return -1;
        idvec->ids = new_ids;
        idvec->alloced = new_size;
    }
    return 0;
}

/* Add ID to IDVEC */
static inline int
idvec_add(struct idvec *idvec, uid_t id)
{
    if (idvec_ensure(idvec, 1) < 0)
        return -1;
    idvec->ids[idvec->num++] = id;
    return 0;
}

/* Check if IDVEC contains ID */
static inline int
idvec_contains(const struct idvec *idvec, uid_t id)
{
    for (size_t i = 0; i < idvec->num; i++)
        if (idvec->ids[i] == id)
            return 1;
    return 0;
}

/* Merge ids from an array into IDVEC */
static inline int
idvec_merge_ids(struct idvec *idvec, const uid_t *ids, size_t num)
{
    if (idvec_ensure(idvec, num) < 0)
        return -1;
    memcpy(idvec->ids + idvec->num, ids, num * sizeof(uid_t));
    idvec->num += num;
    return 0;
}

/* Merge another idvec into IDVEC */
static inline int
idvec_merge(struct idvec *idvec, const struct idvec *other)
{
    return idvec_merge_ids(idvec, other->ids, other->num);
}

#endif /* _IDVEC_H */
