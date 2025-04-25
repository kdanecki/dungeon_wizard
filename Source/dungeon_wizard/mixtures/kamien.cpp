#include "kamien.h"

void Kamien::add_properties()
{
    add_prop(get_ciezar());
    add_prop(get_ostrosc());
    add_prop(get_wielkosc());
    add_prop(get_twardosc());
    add_prop(get_gladkosc());
    add_prop(new Property(&stan_skupienia,&cialo_stale));
}

Kamien::Kamien(int q) : Element(CLASS_KAMIEN, CAT_MINERAL, q)
{    
    add_properties();
}

Kamien::Kamien() : Element(CLASS_KAMIEN, CAT_MINERAL)
{
    add_properties();
}
