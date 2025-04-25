#include "properties.h"
#include "skladnik.h"

#define FUN_GET(prop, val1,val2) \
Property * get_##prop() \
{\
    return new Property(&prop, (rand() % 2) ? &val1 : &val2);\
}

#define FUN_GET3(prop, val1,val2,val3) \
Property * get_##prop() \
{\
    Property_val * val=0; \
    switch (rand() % 3) \
    { \
        case 0: val = &val1; break; \
        case 1: val = &val2; break; \
        case 2: val = &val3; break; \
    } \
    return new Property(&prop, val);\
}

#define FUN_GET4(prop, val1,val2,val3,val4) \
Property * get_##prop() \
{\
    Property_val * val=0; \
    switch (rand() % 4) \
    { \
        case 0: val = &val1; break; \
        case 1: val = &val2; break; \
        case 2: val = &val3; break; \
        case 3: val = &val4; break; \
    } \
    return new Property(&prop, val);\
}

struct Property_cat ciezar = {PROP_CIEZAR, "ciężar"};
struct Property_val lekki ={LEKKI, "lekki"};
struct Property_val ciezki = {CIEZKI, "ciężki"};
FUN_GET(ciezar, lekki, ciezki);

Property * mix_ciezar(Skladnik * s1, Skladnik * s2)
{    
    Property *p1 = s1->get_prop(PROP_CIEZAR);
    Property *p2 = s2->get_prop(PROP_CIEZAR);
    Property_val *v;

    if ((p1->get_val() == CIEZKI) || (p2->get_val() == CIEZKI)) {
          v=&ciezki;
    } else v=&lekki;
    return new Property(&ciezar, v);
}

struct Property_cat twardosc = {PROP_TWARDOSC, "twardość"};
struct Property_val miekki = {MIEKKI, "miękki"};
struct Property_val twardy = {TWARDY, "twardy"};
FUN_GET(twardosc, miekki, twardy);
Property * mix_twardosc(Skladnik * s1, Skladnik *s2)
{
    Property *p1 = s1->get_prop(PROP_TWARDOSC);
    Property *p2 = s2->get_prop(PROP_TWARDOSC);

    Property_val *v;
    if ((p1->get_val() == MIEKKI) || (p2->get_val() == MIEKKI)) {
          v = &miekki;
    } else v = &twardy;
    return new Property(&ciezar, v);
}

struct Property_cat ostrosc = {PROP_OSTROSC, "ostrość"};
struct Property_val tepy = {TEPY, "tępy"};
struct Property_val ostry = {OSTRY, "ostry"};
FUN_GET(ostrosc, tepy, ostry);

struct Property_cat wielkosc = {PROP_WIELKOSC, "wielkość"};
struct Property_val maly = {MALY, "mały"};
struct Property_val duzy = {DUZY, "duży"};
FUN_GET(wielkosc, maly, duzy);

Property * mix_wielkosc(Skladnik * s1, Skladnik *s2)
{    
    Property *p1 = s1->get_prop(PROP_WIELKOSC);
    Property *p2 = s2->get_prop(PROP_WIELKOSC);

    Property_val *v;
    if ((p1->get_val() == DUZY) || (p2->get_val() == DUZY)) {
          v = &duzy;
    } else v =&maly;
    return new Property(&wielkosc, v);
}

struct Property_cat dlugosc = {PROP_DLUGOSC, "długość"};
struct Property_val b_krotki = {B_KROTKI, "bardzo krótki"};
struct Property_val krotki = {KROTKI, "krótki"};
struct Property_val dlugi = {DLUGI, "długi"};
struct Property_val b_dlugi = {B_DLUGI, "bardzo długi"};
FUN_GET4(dlugosc, b_krotki, krotki, dlugi, b_dlugi);

struct Property_cat gladkosc = {PROP_GLADKOSC, "gładkość"};
struct Property_val gladki = {GLADKI, "gładki"};
struct Property_val szorstki = {SZORSTKI, "szorstki"};
FUN_GET(gladkosc, gladki, szorstki);

struct Property_cat stan_skupienia = {PROP_STAN_SKUPIENIA, "stan skupienia"};
struct Property_val cialo_stale = {STAN_STALY, "ciało stałe"};
struct Property_val plyn = {STAN_CIEKLY, "płyn"};
struct Property_val gaz = {STAN_GAZOWY, "gaz"};
FUN_GET3(stan_skupienia, cialo_stale, plyn, gaz);

struct Property_cat jasnosc = {PROP_JASNOSC, "jasność"};
struct Property_val ciemny = {CIEMNY, "ciemny"};
struct Property_val jasny = {JASNY, "jasny"};
struct Property_val swieci = {SWIECI, "świeci"};
FUN_GET3(jasnosc, ciemny, jasny, swieci);

struct Property_cat * property_cat_table[PROP_COUNT]
{
    nullptr,
    &ciezar,
    &twardosc,
    &ostrosc,
    &wielkosc,
    &dlugosc,
    &gladkosc,
    &stan_skupienia,
    &jasnosc,
};

const char * get_property_cat_name(Property_categories c)
{
    return property_cat_table[c]->name;
}

struct Property_val * property_val_table[VAL_COUNT]
{
    nullptr,
    &lekki, &ciezki,
    &miekki, &twardy,
    &tepy, &ostry,
    &maly, &duzy,
    &b_krotki, &krotki, &dlugi, &b_dlugi,
    &gladki, &szorstki,
    &cialo_stale, &plyn, &gaz,
    &ciemny, &jasny, &swieci,

};

Property::Property(Property_categories id, Property_values v)
{
    prop=property_cat_table[id];
    val =property_val_table[v];
}
