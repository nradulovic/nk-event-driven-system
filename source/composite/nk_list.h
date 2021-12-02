/*
 * Neon
 * Copyright (C) 2018   REAL-TIME CONSULTING
 *
 * For license information refer to LGPL-3.0.md file at the root of this project.
 */
/** @file
 *  @addtogroup neon-kit
 *  @{
 */
/** @defgroup   nk_list Doubly linked list
 *  @brief      Doubly linked list
 *  @{
 */

#ifndef NEON_KIT_GENERIC_NK_LIST_H_
#define NEON_KIT_GENERIC_NK_LIST_H_

#include <stdbool.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define NK_LIST__INITIALIZER(list)                                          \
        {                                                                   \
            .p__next = (list),                                              \
            .p__prev = (list),                                              \
        }

/** @brief      Macro to get the first element in list pointed by @a sentinel.
 *  @param[in]  sentinel
 *              Pointer to sentinel of a list.
 *  @note       This macro is exception to macro naming rule since it is does
 *              not have side effects.
 */
#define nk_list__first(sentinel)       nk_list__next(sentinel)

/** @brief      Helper macro, get the last element in list pointed by
 *              @a sentinel.
 *  @param[in]  sentinel
 *              Pointer to sentinel of a list.
 *  @note       This macro is exception to macro naming rule since it is does
 *              not have side effects.
 */
#define nk_list__last(sentinel)        nk_list__prev(sentinel)

/** @brief      Helper macro, add a node @a node at list head pointed by
 *              @a sentinel.
 *  @param[in]  sentinel
 *              Pointer to sentinel of a list.
 *  @param[in]  node
 *              Pointer to a node to be added to list.
 *  @note       This macro is exception to macro naming rule since it is does
 *              not have side effects.
 */
#define nk_list__add_head(sentinel, node)                                  \
        nk_list_add_before(sentinel, node)

/** @brief      Helper macro, add a node @a node at list tail pointed by
 *              @a sentinel.
 *  @param[in]  sentinel
 *              Pointer to sentinel of a list.
 *  @param[in]  node
 *              Pointer to a node to be added to list.
 *  @note       This macro is exception to macro naming rule since it is does
 *              not have side effects.
 */
#define nk_list__add_tail(sentinel, node)                                  \
        nk_list_add_after(sentinel, node)


/** @brief      Construct for @a FOR loop to iterate over each element in a
 *              list.
 *
 *  @code
 *  struct nk_list * current;
 *  struct nk_list * sentinel = &g_list_sentinel.list;
 *
 *  for (NK_LIST__EACH(current, sentinel)) {
 *      ... do something with @a current (excluding remove)
 *  }
 *  @endcode
 *  @mseffect
 */
#define NK_LIST__EACH(current, sentinel)                                    \
        (current) = nk_list__next(sentinel);                                \
        (current) != (sentinel);                                            \
        (current) = nk_list__next(current)

/** @brief      Construct for FOR loop to iterate over each element in list
 *              which is safe against element removal.
 *
 *  @code
 *  struct nk_list * current;
 *  struct nk_list * iterator;
 *  struct nk_list * sentinel = &g_list_sentinel.list;
 *
 *  for (NL_LIST__EACH_SAFE(current, iterator, sentinel)) {
 *      ... do something with current (including remove)
 *  }
 *  @endcode
 *  @mseffect
 */
#define NL_LIST__EACH_SAFE(current, iterator, sentinel)                     \
    current = nk_list__next(sentinel), iterator = nk_list__next(current);   \
    current != (sentinel);                                                  \
    current = iterator, iterator = nk_list__next(iterator)

struct nk_list
{
    struct nk_list *p__next;
    struct nk_list *p__prev;
};

/** \brief      Initialize a list sentinel or node.
 *
 *  \param[in]  node
 *              A list sentinel or node.
 */
static inline void
nk_list__init(struct nk_list *self)
{
    self->p__next = self;
    self->p__prev = self;
}

static inline struct nk_list*
nk_list__next(struct nk_list *self)
{
    return self->p__next;
}

static inline struct nk_list*
nk_list__prev(struct nk_list *self)
{
    return self->p__prev;
}

/** \brief      Add \a self node after \a other node.
 *
 *  Before calling the function `(M) next-> (other) next-> (N)`.
 *  After calling the function `(M) next-> (other) next-> (self) next-> (N)`.
 */
static inline void
nk_list__add_after(struct nk_list *self,
                   struct nk_list *other)
{
    self->p__next = other->p__next;
    self->p__prev = other;
    self->p__next->p__prev = self;
    self->p__prev->p__next = self;
}

/**
 *
 * (M) next-> (other) next-> (N)
 * (M) next-> (self)  next-> (other) next-> (N)
 */
static inline void
nk_list__add_before(struct nk_list *self,
                    struct nk_list *other)
{
    self->p__next = other;
    self->p__prev = other->p__prev;
    self->p__next->p__prev = self;
    self->p__prev->p__next = self;
}

static inline void
nk_list__add_sorted(struct nk_list *self,
                    struct nk_list *list,
                    bool
                    (*should_break)(const struct nk_list *lhs,
                                     const struct nk_list *rhs))
{
    struct nk_list *current;

    current = nk_list__next(list);
    for (; current != list; current = nk_list__next(current)) {
        if (should_break(self, current)) {
            break;
        }
    }
    nk_list__add_after(self, current);
}

/**
 * (M) next-> (self) next-> (N)
 * (M) next-> (N)
 */
static inline void
nk_list__remove(struct nk_list *self)
{
    self->p__next->p__prev = self->p__prev;
    self->p__prev->p__next = self->p__next;
}

static inline bool
nk_list__is_empty(const struct nk_list *self)
{
    return !!(self->p__next == self);
}

#if defined(__cplusplus)
}
#endif

#endif /* NEON_KIT_GENERIC_NK_LIST_H_ */

/** @} */
/** @} */

