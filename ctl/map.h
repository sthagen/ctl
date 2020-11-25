// For implementation details:
//   https://web.archive.org/web/20140328232325/http://en.literateprograms.org/Red-black_tree_(C)

#ifndef T
#error "Template type T undefined for <map>"
#endif

#ifndef U
#error "Template type U undefined for <map>"
#endif

#include <ctl.h>

#define A JOIN(map, JOIN(T, U))
#define B JOIN(A, node)
#define I JOIN(A, it)

typedef struct B
{
    struct B* l;
    struct B* r;
    struct B* p;
    T first;
    U second;
    int color; // RED 0, BLK 1
}
B;

typedef struct
{
    B* root;
    int (*compare)(T*, T*);
    void (*free)(T*, U*);
    void (*copy)(T*, T*, U*, U*);
    size_t size;
}
A;

typedef struct I
{
    void (*step)(struct I*);
    B* end;
    B* node;
    B* next;
    int done;
}
I;

static inline B*
JOIN(A, begin)(A* self)
{
    return self->root;
}

static inline B*
JOIN(A, end)(A* self)
{
    (void) self;
    return NULL;
}

static inline int
JOIN(A, empty)(A* self)
{
    return self->size == 0;
}

static inline void
JOIN(A, implicit_copy)(T* a, T* b, U* c, U* d)
{
    *a = *b;
    *c = *d;
}

static inline A
JOIN(A, init)(void)
{
    static A zero;
    A self = zero;
#ifdef P
#undef P
    self.copy = JOIN(A, implicit_copy);
#else
    self.free = JOIN(T, JOIN(U, free));
    self.copy = JOIN(T, JOIN(U, copy));
#endif
    return self;
}

static inline A
JOIN(A, create)(int compare(T*, T*))
{
    A self = JOIN(A, init)();
    self.compare = compare;
    return self;
}

static inline A
JOIN(A, init_default)(void)
{
    return JOIN(A, init)();
}

static inline void
JOIN(A, free_node)(A* self, B* node)
{
    if(self->free)
        self->free(&node->first, &node->second);
    free(node);
}

static inline int
JOIN(B, color)(B* self)
{
    return self ? self->color : 1;
}

static inline int
JOIN(B, is_blk)(B* self)
{
    return JOIN(B, color)(self) == 1;
}

static inline int
JOIN(B, is_red)(B* self)
{
    return JOIN(B, color)(self) == 0;
}

static inline B*
JOIN(B, lower_bound)(B* self) // XXX. CORRECT?
{
    while(self->l)
        self = self->l;
    return self;
}

static inline B*
JOIN(B, upper_bound)(B* self) // XXX. CORRECT?
{
    while(self->r)
        self = self->r;
    return self;
}

static inline B*
JOIN(B, grandfather)(B* self)
{
    return self->p->p;
}

static inline B*
JOIN(B, sibling)(B* self)
{
    if(self == self->p->l)
        return self->p->r;
    else
        return self->p->l;
}

static inline B*
JOIN(B, uncle)(B* self)
{
    return JOIN(B, sibling)(self->p);
}

static inline B*
JOIN(B, init)(T key, U value, int color)
{
    B* self = (B*) malloc(sizeof(B));
    self->first = key;
    self->second = value;
    self->color = color;
    self->l = self->r = self->p = NULL;
    return self;
}

static inline B*
JOIN(A, find)(A* self, T key)
{
    B* node = self->root;
    while(node)
    {
        int diff = self->compare(&key, &node->first);
        if(diff == 0)
            return node;
        else
        if(diff < 0)
            node = node->l;
        else
            node = node->r;
    }
    return node;
}

static inline U*
JOIN(A, at)(A* self, T key)
{
    return &JOIN(A, find)(self, key)->second;
}

static inline int
JOIN(A, count)(A* self, T key)
{
    return JOIN(A, find)(self, key) ? 1 : 0;
}

static inline int
JOIN(A, contains)(A* self, T key)
{
    return JOIN(A, count)(self, key) == 1;
}

static inline void
JOIN(A, equal_range)(A* self, T key, B** a, B** b) // XXX. CORRECT?
{
    B* node = JOIN(A, find)(self, key);
    *a = node ? JOIN(B, lower_bound)(node) : NULL;
    *b = node ? JOIN(B, upper_bound)(node) : NULL;
}

static inline void
JOIN(A, set)(A* self, T key, U value)
{
    B* node = JOIN(A, find)(self, key);
    if(self->free)
        self->free(&node->first, &node->second);
    node->first = key;
    node->second = value;
}

static inline void
JOIN(B, replace)(A* self, B* a, B* b)
{
    if(a->p)
    {
        if(a == a->p->l)
            a->p->l = b;
        else
            a->p->r = b;
    }
    else
        self->root = b;
    if(b)
        b->p = a->p;
}

#ifdef USE_INTERNAL_VERIFY

#include <assert.h>

static inline void
JOIN(B, verify_property_1)(B* self)
{
    assert(JOIN(B, is_red)(self) || JOIN(B, is_blk)(self));
    if(self)
    {
        JOIN(B, verify_property_1)(self->l);
        JOIN(B, verify_property_1)(self->r);
    }
}

static inline void
JOIN(B, verify_property_2)(B* self)
{
    assert(JOIN(B, is_blk)(self));
}

static inline void
JOIN(B, verify_property_4)(B* self)
{
    if(JOIN(B, is_red)(self))
    {
        assert(JOIN(B, is_blk)(self->l));
        assert(JOIN(B, is_blk)(self->r));
        assert(JOIN(B, is_blk)(self->p));
    }
    if(self)
    {
        JOIN(B, verify_property_4)(self->l);
        JOIN(B, verify_property_4)(self->r);
    }
}

static inline void
JOIN(B, count_blk)(B* self, int nodes, int* in_path)
{
    if(JOIN(B, is_blk)(self))
        nodes += 1;
    if(self)
    {
        JOIN(B, count_blk)(self->l, nodes, in_path);
        JOIN(B, count_blk)(self->r, nodes, in_path);
    }
    else
    {
        if(*in_path == -1)
            *in_path = nodes;
        else
            assert(nodes == *in_path);
    }
}

static inline void
JOIN(B, verify_property_5)(B* self)
{
    int in_path = -1;
    JOIN(B, count_blk)(self, 0, &in_path);
}

static inline void
JOIN(A, verify)(A* self)
{
    JOIN(B, verify_property_1)(self->root); // PROPERTY 1: EACH NODE IS EITHER RED OR BLACK.
    JOIN(B, verify_property_2)(self->root); // PROPERTY 2: THE ROOT NODE IS BLACK.
    /* IMPLICIT */                          // PROPERTY 3: LEAVES ARE COLORED BLACK
    JOIN(B, verify_property_4)(self->root); // PROPERTY 4: EVERY RED NODE HAS TWO BLACK NDOES.
    JOIN(B, verify_property_5)(self->root); // PROPERTY 5: ALL PATHS FROM A NODE HAVE THE SAME NUMBER OF BLACK NODES.
}

#endif

static inline void
JOIN(A, rotate_l)(A* self, B* node)
{
    B* r = node->r;
    JOIN(B, replace)(self, node, r);
    node->r = r->l;
    if(r->l)
        r->l->p = node;
    r->l = node;
    node->p = r;
}

static inline void
JOIN(A, rotate_r)(A* self, B* node)
{
    B* l = node->l;
    JOIN(B, replace)(self, node, l);
    node->l = l->r;
    if(l->r)
        l->r->p = node;
    l->r = node;
    node->p = l;
}

static inline void
JOIN(A, insert_1)(A*, B*),
JOIN(A, insert_2)(A*, B*),
JOIN(A, insert_3)(A*, B*),
JOIN(A, insert_4)(A*, B*),
JOIN(A, insert_5)(A*, B*);

static inline void
JOIN(A, insert)(A* self, T key, U value)
{
    B* insert = JOIN(B, init)(key, value, 0);
    if(self->root)
    {
        B* node = self->root;
        while(1)
        {
            int diff = self->compare(&key, &node->first);
            if(diff == 0)
            {
                JOIN(A, free_node)(self, insert);
                return;
            }
            else
            if(diff < 0)
            {
                if(node->l)
                    node = node->l;
                else
                {
                    node->l = insert;
                    break;
                }
            }
            else
            {
                if(node->r)
                    node = node->r;
                else
                {
                    node->r = insert;
                    break;
                }
            }
        }
        insert->p = node;
    }
    else
        self->root = insert;
    JOIN(A, insert_1)(self, insert);
    self->size += 1;
#ifdef USE_INTERNAL_VERIFY
    JOIN(A, verify)(self);
#endif
}

static inline void
JOIN(A, insert_1)(A* self, B* node)
{
    if(node->p)
        JOIN(A, insert_2)(self, node);
    else
        node->color = 1;
}

static inline void
JOIN(A, insert_2)(A* self, B* node)
{
    if(JOIN(B, is_blk)(node->p))
        return;
    else
       JOIN(A, insert_3)(self, node);
}

static inline void
JOIN(A, insert_3)(A* self, B* node)
{
    if(JOIN(B, is_red)(JOIN(B, uncle)(node)))
    {
        node->p->color = 1;
        JOIN(B, uncle)(node)->color = 1;
        JOIN(B, grandfather)(node)->color = 0;
        JOIN(A, insert_1)(self, JOIN(B, grandfather)(node));
    }
    else
        JOIN(A, insert_4)(self, node);
}

static inline void
JOIN(A, insert_4)(A* self, B* node)
{
    if(node == node->p->r && node->p == JOIN(B, grandfather)(node)->l)
    {
        JOIN(A, rotate_l)(self, node->p);
        node = node->l;
    }
    else
    if(node == node->p->l && node->p == JOIN(B, grandfather)(node)->r)
    {
        JOIN(A, rotate_r)(self, node->p);
        node = node->r;
    }
    JOIN(A, insert_5)(self, node);
}

static inline void
JOIN(A, insert_5)(A* self, B* node)
{
    node->p->color = 1;
    JOIN(B, grandfather)(node)->color = 0;
    if(node == node->p->l && node->p == JOIN(B, grandfather)(node)->l)
        JOIN(A, rotate_r)(self, JOIN(B, grandfather)(node));
    else
        JOIN(A, rotate_l)(self, JOIN(B, grandfather)(node));
}

static inline void
JOIN(A, erase_1)(A*, B*),
JOIN(A, erase_2)(A*, B*),
JOIN(A, erase_3)(A*, B*),
JOIN(A, erase_4)(A*, B*),
JOIN(A, erase_5)(A*, B*),
JOIN(A, erase_6)(A*, B*);

static inline void
JOIN(A, erase_node)(A* self, B* node)
{
    if(node->l && node->r)
    {
        B* pred = JOIN(B, upper_bound)(node->l);
        SWAP(T, &node->first, &pred->first);
        SWAP(U, &node->second, &pred->second);
        node = pred;
    }
    B* child = node->r ? node->r : node->l;
    if(JOIN(B, is_blk)(node))
    {
        node->color = JOIN(B, color)(child);
        JOIN(A, erase_1)(self, node);
    }
    JOIN(B, replace)(self, node, child);
    if(node->p == NULL && child)
        child->color = 1;
    JOIN(A, free_node)(self, node);
    self->size -= 1;
#ifdef USE_INTERNAL_VERIFY
    JOIN(A, verify)(self);
#endif
}

static inline void
JOIN(A, erase)(A* self, T key)
{
    B* node = JOIN(A, find)(self, key);
    if(node)
        JOIN(A, erase_node)(self, node);
}

static inline void
JOIN(A, erase_1)(A* self, B* node)
{
    if(node->p)
        JOIN(A, erase_2)(self, node);
}

static inline void
JOIN(A, erase_2)(A* self, B* node)
{
    if(JOIN(B, is_red)(JOIN(B, sibling)(node)))
    {
        node->p->color = 0;
        JOIN(B, sibling)(node)->color = 1;
        if(node == node->p->l)
            JOIN(A, rotate_l)(self, node->p);
        else
            JOIN(A, rotate_r)(self, node->p);
    }
    JOIN(A, erase_3)(self, node);
}

static inline void
JOIN(A, erase_3)(A* self, B* node)
{
    if(JOIN(B, is_blk)(node->p)
    && JOIN(B, is_blk)(JOIN(B, sibling)(node))
    && JOIN(B, is_blk)(JOIN(B, sibling)(node)->l)
    && JOIN(B, is_blk)(JOIN(B, sibling)(node)->r))
    {
        JOIN(B, sibling)(node)->color = 0;
        JOIN(A, erase_1)(self, node->p);
    }
    else
        JOIN(A, erase_4)(self, node);
}

static inline void
JOIN(A, erase_4)(A* self, B* node)
{
    if(JOIN(B, is_red)(node->p)
    && JOIN(B, is_blk)(JOIN(B, sibling)(node))
    && JOIN(B, is_blk)(JOIN(B, sibling)(node)->l)
    && JOIN(B, is_blk)(JOIN(B, sibling)(node)->r))
    {
        JOIN(B, sibling)(node)->color = 0;
        node->p->color = 1;
    }
    else
        JOIN(A, erase_5)(self, node);
}

static inline void
JOIN(A, erase_5)(A* self, B* node)
{
    if(node == node->p->l
    && JOIN(B, is_blk)(JOIN(B, sibling)(node))
    && JOIN(B, is_red)(JOIN(B, sibling)(node)->l)
    && JOIN(B, is_blk)(JOIN(B, sibling)(node)->r))
    {
        JOIN(B, sibling)(node)->color = 0;
        JOIN(B, sibling)(node)->l->color = 1;
        JOIN(A, rotate_r)(self, JOIN(B, sibling)(node));
    }
    else
    if(node == node->p->r
    && JOIN(B, is_blk)(JOIN(B, sibling)(node))
    && JOIN(B, is_red)(JOIN(B, sibling)(node)->r)
    && JOIN(B, is_blk)(JOIN(B, sibling)(node)->l))
    {
        JOIN(B, sibling)(node)->color = 0;
        JOIN(B, sibling)(node)->r->color = 1;
        JOIN(A, rotate_l)(self, JOIN(B, sibling)(node));
    }
    JOIN(A, erase_6)(self, node);
}

static inline void
JOIN(A, erase_6)(A* self, B* node)
{
    JOIN(B, sibling)(node)->color = JOIN(B, color)(node->p);
    node->p->color = 1;
    if(node == node->p->l)
    {
        JOIN(B, sibling)(node)->r->color = 1;
        JOIN(A, rotate_l)(self, node->p);
    }
    else
    {
        JOIN(B, sibling)(node)->l->color = 1;
        JOIN(A, rotate_r)(self, node->p);
    }
}

static inline void
JOIN(A, clear)(A* self)
{
    while(!JOIN(A, empty)(self))
        JOIN(A, erase)(self, self->root->first);
}

static inline void
JOIN(A, free)(A* self)
{
    JOIN(A, clear)(self);
    *self = JOIN(A, init)();
}

static inline void
JOIN(A, swap)(A* self, A* other)
{
    A temp = *self;
    *self = *other;
    *other = temp;
}

static inline B*
JOIN(B, next)(B* self)
{
    if(self->r)
    {
        self = self->r;
        while(self->l)
            self = self->l;
    }
    else
    {
        B* parent = self->p;
        while(parent && self == parent->r)
        {
            self = parent;
            parent = parent->p;
        }
        self = parent;
    }
    return self;
}

static inline void
JOIN(I, step)(I* self)
{
    if(self->next == self->end)
        self->done = 1;
    else
    {
        self->node = self->next;
        self->next = JOIN(B, next)(self->node);
    }
}

static inline I
JOIN(I, range)(B* begin, B* end)
{
    static I zero;
    I self = zero;
    if(begin)
    {
        self.step = JOIN(I, step);
        self.node = JOIN(B, lower_bound)(begin);
        self.next = JOIN(B, next)(self.node);
        self.end = end;
    }
    else
        self.done = 1;
    return self;
}

static inline I
JOIN(I, each)(A* a)
{
    return JOIN(I, range)(JOIN(A, begin)(a), JOIN(A, end)(a));
}

static inline int
JOIN(A, equal)(A* self, A* other, int equal(T*, T*, U*, U*))
{
    if(self->size != other->size)
        return 0;
    I a = JOIN(I, each)(self);
    I b = JOIN(I, each)(other);
    while(!a.done && !b.done)
    {
        if(!equal(&a.node->first, &b.node->first, &a.node->second, &b.node->second))
            return 0;
        a.step(&a);
        b.step(&b);
    }
    return 1;
}

static inline A
JOIN(A, copy)(A* self)
{
    I it = JOIN(I, each)(self);
    A copy =  JOIN(A, create)(self->compare);
    while(!it.done)
    {
        T first;
        U second;
        self->copy(&first, &it.node->first, &second, &it.node->second);
        JOIN(A, insert)(&copy, first, second);
        it.step(&it);
    }
    return copy;
}

static inline size_t
JOIN(A, erase_if)(A* self, int (*equal)(T*, U*))
{
    size_t erases = 0;
    foreach(A, self, it,
        if(equal(&it.node->first, &it.node->second))
        {
            JOIN(A, erase_node)(self, it.node);
            erases += 1;
        }
    );
    return erases;
}

#undef T
#undef U
#undef A
#undef B
#undef C
#undef D
#undef I

#ifdef USE_INTERNAL_VERIFY
#undef USE_INTERNAL_VERIFY
#endif