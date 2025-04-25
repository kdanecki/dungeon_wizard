#include "patyk.h"

void Patyk::add_properties()
{
    add_prop(get_ciezar());
    add_prop(get_ostrosc());
    add_prop(get_wielkosc());
    add_prop(get_twardosc());
    add_prop(get_gladkosc());
    add_prop(get_dlugosc());
    add_prop(new Property(&stan_skupienia,&cialo_stale));
}

Patyk::Patyk(int q) : Element(CLASS_PATYK, CAT_MINERAL, q)
{        
    add_properties();
}

Patyk::Patyk() : Element(CLASS_PATYK, CAT_MINERAL)
{
    add_properties();
}
