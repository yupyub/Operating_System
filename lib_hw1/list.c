#include "list.h"
#include <assert.h>	// Instead of	#include "../debug.h"
#include <time.h>
#define ASSERT(CONDITION) assert(CONDITION)	// patched for proj0-2

/* Our doubly linked lists have two header elements: the "head"
   just before the first element and the "tail" just after the
   last element.  The `prev' link of the front header is null, as
   is the `next' link of the back header.  Their other two links
   point toward each other via the interior elements of the list.

   An empty list looks like this:

   +------+     +------+
   <---| head |<--->| tail |--->
   +------+     +------+

   A list with two elements in it looks like this:

   +------+     +-------+     +-------+     +------+
   <---| head |<--->|   1   |<--->|   2   |<--->| tail |<--->
   +------+     +-------+     +-------+     +------+

   The symmetry of this arrangement eliminates lots of special
   cases in list processing.  For example, take a look at
   list_remove(): it takes only two pointer assignments and no
   conditionals.  That's a lot simpler than the code would be
   without header elements.

   (Because only one of the pointers in each header element is used,
   we could in fact combine them into a single header element
   without sacrificing this simplicity.  But using two separate
   elements allows us to do a little bit of checking on some
   operations, which can be valuable.) */

static bool is_sorted (struct list_elem *a, struct list_elem *b,
        list_less_func *less, void *aux);// Remove KERNEL MACRO 'UNUSED';

/* Returns true if ELEM is a head, false otherwise. */
    static inline bool
is_head (struct list_elem *elem)
{
    return elem != NULL && elem->prev == NULL && elem->next != NULL;
}

/* Returns true if ELEM is an interior element,
   false otherwise. */
    static inline bool
is_interior (struct list_elem *elem)
{
    return elem != NULL && elem->prev != NULL && elem->next != NULL;
}

/* Returns true if ELEM is a tail, false otherwise. */
    static inline bool
is_tail (struct list_elem *elem)
{
    return elem != NULL && elem->prev != NULL && elem->next == NULL;
}

/* Initializes LIST as an empty list. */
    void
list_init (struct list *list)
{
    ASSERT (list != NULL);
    list->head.prev = NULL;
    list->head.next = &list->tail;
    list->tail.prev = &list->head;
    list->tail.next = NULL;
}

/* Returns the beginning of LIST.  */
    struct list_elem *
list_begin (struct list *list)
{
    ASSERT (list != NULL);
    return list->head.next;
}

/* Returns the element after ELEM in its list.  If ELEM is the
   last element in its list, returns the list tail.  Results are
   undefined if ELEM is itself a list tail. */
    struct list_elem *
list_next (struct list_elem *elem)
{
    ASSERT (is_head (elem) || is_interior (elem));
    return elem->next;
}

/* Returns LIST's tail.

   list_end() is often used in iterating through a list from
   front to back.  See the big comment at the top of list.h for
   an example. */
    struct list_elem *
list_end (struct list *list)
{
    ASSERT (list != NULL);
    return &list->tail;
}

/* Returns the LIST's reverse beginning, for iterating through
   LIST in reverse order, from back to front. */
    struct list_elem *
list_rbegin (struct list *list) 
{
    ASSERT (list != NULL);
    return list->tail.prev;
}

/* Returns the element before ELEM in its list.  If ELEM is the
   first element in its list, returns the list head.  Results are
   undefined if ELEM is itself a list head. */
    struct list_elem *
list_prev (struct list_elem *elem)
{
???LINES MISSING
???LINES MISSING
???LINES MISSING
   the list is empty, returns its tail. */
    struct list_elem *
list_max (struct list *list, list_less_func *less, void *aux)
{
    struct list_elem *max = list_begin (list);
    if (max != list_end (list)) 
    {
        struct list_elem *e;

        for (e = list_next (max); e != list_end (list); e = list_next (e))
            if (less (max, e, aux))
                max = e; 
    }
    return max;
}

/* Returns the element in LIST with the smallest value according
   to LESS given auxiliary data AUX.  If there is more than one
   minimum, returns the one that appears earlier in the list.  If
   the list is empty, returns its tail. */
    struct list_elem *
list_min (struct list *list, list_less_func *less, void *aux)
{
    struct list_elem *min = list_begin (list);
    if (min != list_end (list)) 
    {
        struct list_elem *e;

        for (e = list_next (min); e != list_end (list); e = list_next (e))
            if (less (e, min, aux))
                min = e; 
    }
    return min;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

void list_swap(struct list_elem *a, struct list_elem *b){
   struct list_elem* aPrev = a->prev;
   struct list_elem* aNext = a->next;
   struct list_elem* bPrev = b->prev;
   struct list_elem* bNext = b->next;
   if(aPrev != NULL)
       aPrev->next = b;
   if(aNext != NULL)
       aNext->prev = b;
   if(bPrev != NULL)
       bPrev->next = a;
   if(bNext != NULL)
       bNext->prev = a;
   a->prev = bPrev;
   b->prev = aPrev;
   a->next = bNext;
   b->next = aNext;
}
struct list_elem* findXth(struct list* List,int X){
    struct list_elem* el;
    for(el = list_begin(List); el != list_end(List)&&X--; el = list_next(el));
    return el;
}
void list_shuffle(struct list *list){
    srand(time(NULL));
    size_t size = list_size(list);
    for(int i = 0;i<size;i++){
        list_swap(findXth(list,i),findXth(list,rand()%size));
    }
}
