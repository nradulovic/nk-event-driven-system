
#include "eds_core.h"

extern inline void
eds_core__list__init(struct eds_object__list *self);

extern inline struct eds_object__list *
eds_core__list__next(struct eds_object__list *self);

extern inline struct eds_object__list *
eds_core__list__prev(struct eds_object__list *self);

extern inline void
eds_core__list__add_after(struct eds_object__list *self, struct eds_object__list *after);

extern inline void
eds_core__list__add_before(struct eds_object__list *self, struct eds_object__list *before);

extern inline void
eds_core__list__remove(struct eds_object__list *self);

extern inline bool
eds_core__list__is_empty(const struct eds_object__list *self);
