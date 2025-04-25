#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "el_list.h"
#include <cstdlib>

enum Property_categories
{
    PROP_BRAK,
    PROP_CIEZAR,
    PROP_TWARDOSC,
    PROP_OSTROSC,
    PROP_WIELKOSC,
    PROP_DLUGOSC,
    PROP_GLADKOSC,
    PROP_STAN_SKUPIENIA,
    PROP_JASNOSC,
    PROP_COUNT,
};

enum Property_values
{
    NIC,
    LEKKI, CIEZKI,
    MIEKKI, TWARDY,
    TEPY, OSTRY,
    MALY, DUZY,
    B_KROTKI, KROTKI, DLUGI, B_DLUGI,
    GLADKI, SZORSTKI,
    STAN_STALY, STAN_CIEKLY, STAN_GAZOWY,
    CIEMNY, JASNY, SWIECI,
    
    VAL_COUNT
};


struct Property_cat
{
    Property_categories id;
    const char * name;
};

const char * get_property_cat_name(Property_categories c);

struct Property_val
{
    Property_values id;
    const char * name;
};

class Property : public Basic_el
{
    Property_cat *prop;
    Property_val *val;

public:

    Property(Property_cat *p, Property_val *v) {
        prop = p;
        val = v;
    }
    Property * clone_prop() {
        return new Property(prop, val);
    }
    Property(Property_categories id, Property_values v);
    void show(bool details=true) {
        if (details) {
            printf("%s%s:%s ", colorYellowBold, prop->name, val->name);
        }
    }
    Property_values get_val() {
        return val->id;
    }
    Property_categories get_cat() {
        return prop->id;
    }
    void set_val(Property_values new_val)
    {
        val->id = new_val;
    }
    void set_val(Property_val *new_val)
    {
        val = new_val;
    }
    bool check_val(Property_values v)
    {
        return val->id == v;
    }
};

class Prop_list: public El_list
{
    public:
        Prop_list(const char *n) : El_list(n) {}
        bool find_check(Basic_el *el, void * what) { 
             Property *prop = (Property*) el;
             Property_categories p = *(Property_categories*)what;
             if (prop->get_cat() == p) return true; else return false;
        }
};

class Skladnik;
extern struct Property_cat ciezar;
extern struct Property_val lekki;
extern struct Property_val ciezki;
Property * get_ciezar();
Property * mix_ciezar(Skladnik *s1, Skladnik *s2);

extern struct Property_cat twardosc;
extern struct Property_val miekki;
extern struct Property_val twardy;
Property * get_twardosc();
Property * mix_twardosc(Skladnik *s1, Skladnik *s2);

extern struct Property_cat ostrosc;
extern struct Property_val tepy;
extern struct Property_val ostry;
Property * get_ostrosc();

extern struct Property_cat wielkosc;
extern struct Property_val maly;
extern struct Property_val duzy;
Property * get_wielkosc();
Property * mix_wielkosc(Skladnik *s1, Skladnik *s2);

extern struct Property_cat dlugosc;
extern struct Property_val b_krotki;
extern struct Property_val krotki;
extern struct Property_val dlugi;
extern struct Property_val b_dlugi;
Property * get_dlugosc();

extern struct Property_cat gladkosc;
extern struct Property_val gladki;
extern struct Property_val szorstki;
Property * get_gladkosc();

extern struct Property_cat stan_skupienia;
extern struct Property_val cialo_stale;
extern struct Property_val plyn;
extern struct Property_val gaz;
Property * get_stan_skupienia();

extern struct Property_cat jasnosc;
extern struct Property_val ciemny;
extern struct Property_val jasny;
extern struct Property_val swieci;
Property * get_jasnosc();

#endif

