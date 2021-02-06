/* Generic List Header */

#ifndef LIST_H
#define LIST_H

struct _private;

typedef struct _list {

	int size;
	struct _private *p;

} list_t;

extern struct _list* ls_ctor(const void *ffp, const void *cfp);
extern struct _list* ls_cpy(struct _list *src);
extern void ls_dtor(struct _list *this);

extern int ls_push(struct _list *this, void *data, const size_t size);
extern void* ls_pop(struct _list *this);

extern int ls_add(struct _list *this, void *data, const size_t size, const int pos);
extern int ls_del(struct _list *this, const int pos);

extern int ls_insert(struct _list *this, void *data, const size_t size, const int pos);

extern void* ls_at(struct _list *this, const int pos);

#endif
