#ifndef MATERIAL_H
#define MATERIAL_H

#include "element.h"

class Skladnik: public Element
{
public:
    Element * skladnik;
    int quality; //[0..100] slaby..najlepszy
    int wytrzymalosc; // [0..100] wytrzymaly..słaby
    int latwosc_uzycia; // [0..15] łatwy..trudny
    Skladnik(Class_id i, Categories_type cat, int q): Element(i, cat, q)
        {            
            skladnik = nullptr;

        }

    void show(bool details=true) {
        Element::show(details);
        printf(" quality=%d/100 wytrzymałość=%d/100 użycie=%d/15\n", quality, wytrzymalosc, latwosc_uzycia);
        if (skladnik) {
            printf("-> Składnik: ");
            skladnik->show();
        }
    }
    bool transform(Element *m, int cnt, Property_categories * cat)
    {
        if (!m->check_props(cnt, cat)) {
            printf(" dla %s z %s\n", get_class_name(), m->get_class_name());
            return false;
        }
        skladnik = m;

        return true;
    }
};

Skladnik * mix_Raczka_noza(Element *m);
Skladnik * mix_Raczka_mlotka(Element *m);
Skladnik * mix_Glowka_mlotka(Element *m);
Skladnik * mix_Ostrze_noza(Element *m);
Skladnik * mix_Raczka_siekiery(Element *m);
Skladnik * mix_Ostrze_siekiery(Element *m);
#endif
