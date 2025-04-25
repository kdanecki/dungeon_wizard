#ifndef EL_LIST_H
#define EL_LIST_H

#include <cstddef>
#include <cstdio>

class Basic_el
{
public:
    Basic_el(): next(nullptr) {}
    virtual ~Basic_el() {}
    Basic_el * next;
    void add(Basic_el *el) { next=el; }
    virtual void show(bool details=true) {}
};

//void typedef (*for_each_fun)(Basic_el *el, void * what);

class El_list
{
    Basic_el * tail;
public:
    const char * name;
    int nr_elements;
    Basic_el * list;
    El_list(const char * n)
    {
        name = n;
        nr_elements =0;
        tail = nullptr;
        list = nullptr;
    }
    virtual ~El_list() {}
    Basic_el * add(Basic_el *el);
    Basic_el * find(void * what);
    Basic_el * find_at(void * pos);
    Basic_el * remove(Basic_el *el);
    void show(bool details=true);
    //void for_each(for_each_fun fun, Basic_el *el, void * what);
    void copy_elements(El_list * dst);
    int save(FILE *f);
    virtual int load(FILE *f) { return 1;}
protected:
    bool virtual find_check(Basic_el *el, void * what) { return false; }
    bool virtual find_at_check(Basic_el *el, void * pos) { return false; }
    void virtual copy(Basic_el *el) {}
};

extern const char * colorGray;
extern const char * colorRed;
extern const char * colorRedBold;
extern const char * colorGreen;
extern const char * colorGreenBold;
extern const char * colorYellow;
extern const char * colorYellowBold;
extern const char * colorBlue;
extern const char * colorBlueBold;
extern const char * colorMagenta;
extern const char * colorCyan;
extern const char * colorWhite;
extern const char * colorNormal;
extern const char * clrscr;
#endif // EL_LIST_H
