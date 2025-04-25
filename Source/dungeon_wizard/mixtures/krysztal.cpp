#include "krysztal.h"

Krysztal::Krysztal(int q) : Element(CLASS_KRYSZTAL, CAT_MINERAL, q)
{    
    add_prop(get_ciezar());
    add_prop(get_ostrosc());
    add_prop(get_wielkosc());
    add_prop(get_twardosc());
    add_prop(get_gladkosc());    
    add_prop(new Property(&stan_skupienia,&cialo_stale));
    add_prop(get_jasnosc());
}
