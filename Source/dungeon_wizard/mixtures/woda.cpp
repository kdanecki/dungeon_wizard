#include "woda.h"

void Woda::add_properties()
{
    add_prop(get_ciezar());
    add_prop(get_wielkosc());

    add_prop(new Property(&stan_skupienia,&plyn));
}

Woda::Woda(int q):Element(CLASS_WODA, CAT_MINERAL, q)
{
    add_properties();
}

Woda::Woda():Element(CLASS_WODA, CAT_MINERAL)
{
    add_properties();
}
