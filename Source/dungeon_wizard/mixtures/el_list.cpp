#include "el_list.h"
#include "class_id.h"
#include <errno.h>
#include <cstring>

Basic_el * El_list::find(void * what)
{
    Basic_el * cur = list;
    while(cur) {
        if (find_check(cur, what)) return cur;
        cur = cur->next;
    }
    return nullptr;
}

Basic_el * El_list::find_at(void * pos)
{
    Basic_el * cur = list;
    while(cur) {
        if (find_at_check(cur, pos)) return cur;
        cur = cur->next;
    }
    return nullptr;
}

void El_list::show(bool details)
{
    Basic_el * cur = list;
    printf("%s> %s (%d): ", colorGreenBold, name, nr_elements);
    while(cur) {
        cur->show(details);
        cur = cur->next;
    }
    printf("%s\n", colorGreen);
}


Basic_el * El_list::add(Basic_el *el)
{
    if (nr_elements) {
        tail->add(el);
        tail = el;
    } else {
        list = el;
        tail = list;
    }
    nr_elements++;
    return el;
}

Basic_el * El_list::remove(Basic_el *el)
{
   Basic_el * cur = list;
   Basic_el * tmp;
   if (list == el)
   {
       tmp = list->next;
       if (tail == el) tail = nullptr;
      // delete el;
       el->next = nullptr;
       nr_elements--;
       list = tmp;
       return list;
   }
   while(cur)
   {
        if (cur->next == el)
        {
            cur->next = cur->next->next;
            if (tail == el)
            {
                tail = cur;
            }
        //    delete el;
            el->next=nullptr;
            nr_elements--;
            return cur;
        }
        cur = cur->next;
   }
   return nullptr;
}

void El_list::copy_elements(El_list * dst)
{
    Basic_el * cur = list;
    while(cur) {
        dst->copy(cur);
        cur = cur->next;
    }
}

const char *colorGray="\033[1;30m";
const char *colorRed="\033[2;31m";
const char *colorRedBold="\033[1;31m";
const char *colorGreen="\033[2;32m";
const char *colorGreenBold="\033[1;32m";
const char *colorYellow="\033[2;33m";
const char *colorYellowBold="\033[1;33m";
const char *colorBlue="\033[2;34m";
const char *colorBlueBold="\033[1;34m";
const char *colorMagenta="\033[2;35m";
const char *colorCyan="\033[2;36m";
const char *colorWhite="\033[1;37m";
const char *colorNormal="\033[0m";
const char *clrscr="\033[H\033[J";
/*
 * \033[ m
 * \e[ m
 * \E[ m
 * 0 - reset
 * 1 - bold/ increased intensity
 * 2 - faint / decreased intensity
 * 4 - singly underlined
 * 5 - slow blink
 * 30-37 - foreground color
 * 38;5;x - foreground color (256)
 * 38;2;r;g;b - foreground color
 * 40-47 - background color
 * 48;5;x- background color (256)
 * 48;2;r;g;b - background color
 * 90-97 - bright foreground color
 * 100-107 - bright background color
 *
 * 0 - black
 * 1 - red
 * 2 - green
 * 3 - yellow
 * 4 - blue
 * 5 - magenta
 * 6 - cyan
 * 7 - white
 */
