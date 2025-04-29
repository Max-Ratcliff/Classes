#include "item.h"
#include "ll.h"

#include <stdio.h>
#include <string.h>
#define MAGICN 457

#define itemptr(x) ((item *) ((void *) (&(x))))

#define pickitem(intval, itemval) (sizeof(item) == sizeof(int)) ? itemptr(intval) : itemptr(itemval)

typedef struct realitem {
    char key[255];
    int id;
} realitem;

int main(void) {
    if ((sizeof(item) != sizeof(realitem)) && (sizeof(item) != sizeof(int))) {
        printf("item may be defined incorrectly");
        return 1;
    }

    LL *l = list_create();
    for (int i = 0; i < 1000; i++) {
        realitem ip;
        ip.id = i;
        for (int i = 0; i < 256; i++) {
            (ip.key)[i] = 0;
        }
        sprintf(ip.key, "%d", i);
        list_add(l, pickitem(i, ip));
    }

    int magic_int = MAGICN;
    realitem magic_item;
    magic_item.id = MAGICN;
    for (int i = 0; i < 256; i++) {
        (magic_item.key)[i] = 0;
    }
    sprintf(magic_item.key, "%d", MAGICN);

    list_remove(l, cmp, pickitem(magic_int, magic_item));

    int ret;
    for (int i = 0; i < 1000; i++) {
        int i_int = i;
        realitem i_item;
        i_item.id = i;
        sprintf(i_item.key, "%d", i);
        item *founditem = list_find(l, cmp, pickitem(i_int, i_item));
        if (founditem) {
            if (sizeof(founditem) == sizeof(int)) {
                ret = *((int *) founditem);
            } else {
                ret = ((realitem *) founditem)->id;
            }
        }
        if (i == MAGICN) {
            if (founditem != NULL) {
                printf("something went wrong. I found %d after deleting it.\n", MAGICN);
                return 1;
            }
        } else if (founditem == NULL || ret != i) {
            printf("something went wrong. I inserted %d into the list but then could not find it. "
                   "%d\n",
                i, ret);
            return 1;
        }
    }
    list_destroy(&l);
    return 0;
}
