#include "skladnik.h"

Skladnik * mix_Glowka_mlotka(Element * m)
{
    if (m->check_prop_val(PROP_STAN_SKUPIENIA, STAN_STALY)) {
        Skladnik * skl= new Skladnik(CLASS_GLOWKA_MLOTKA, CAT_CZESC, m->quantity);
        const int cat_count=3;
        Property_categories cat[cat_count]= {PROP_CIEZAR, PROP_WIELKOSC, PROP_TWARDOSC};

        if (skl->transform(m, cat_count, cat)) {

            skl->quality=0;
            skl->wytrzymalosc=0;
            skl->latwosc_uzycia=0;

            switch (m->get_prop_val(PROP_CIEZAR))
            {
                case LEKKI:
                    skl->quality+=10;
                    skl->latwosc_uzycia+=1;
                break;
                case CIEZKI:
                    skl->quality+=30;
                    skl->latwosc_uzycia+=5;
                break;
            }

            switch (m->get_prop_val(PROP_WIELKOSC))
            {
                case MALY:
                    skl->quality+=10;
                    skl->latwosc_uzycia+=1;
                break;
                case DUZY:
                    skl->quality+=30;
                    skl->latwosc_uzycia+=5;
                break;
            }

            switch (m->get_prop_val(PROP_TWARDOSC))
            {
                case MIEKKI:
                    skl->quality+=1;
                    skl->wytrzymalosc=50;
                    skl->latwosc_uzycia+=5;
                break;
                case TWARDY:
                    skl->quality+=40;
                    skl->wytrzymalosc=1;
                    skl->latwosc_uzycia+=1;
                break;
            }
            skl->show(false);
            return skl;
        }

        delete skl;
    }
    printf("%s can't transform %s to Główka młotka\n", colorRedBold, m->get_class_name());
    return nullptr;
}
