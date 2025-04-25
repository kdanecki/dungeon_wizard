#include "skladnik.h"


Skladnik * mix_Raczka_mlotka(Element * m)
{
  if (m->check_prop_val(PROP_STAN_SKUPIENIA, STAN_STALY)) {
        Skladnik *skl = new Skladnik(CLASS_RACZKA_MLOTKA, CAT_CZESC, m->quantity);
        const int cat_count=5;
        Property_categories cat[cat_count]= {PROP_CIEZAR, PROP_WIELKOSC, PROP_DLUGOSC, PROP_GLADKOSC, PROP_TWARDOSC};

        if (skl->transform(m, cat_count, cat)) {

            switch (m->get_prop_val(PROP_CIEZAR))
            {
                case LEKKI:
                    skl->quality+=10;
                    skl->latwosc_uzycia+=1;
                    skl->wytrzymalosc=10;
                break;
                case CIEZKI:
                    skl->quality+=1;
                    skl->latwosc_uzycia+=3;
                break;
            }
            switch (m->get_prop_val(PROP_WIELKOSC))
            {
                case MALY:
                    skl->quality+=10;
                    skl->latwosc_uzycia+=1;
                break;
                case DUZY:
                    skl->quality+=2;
                    skl->latwosc_uzycia+=3;
                    skl->wytrzymalosc=10;
                break;
            }
            switch (m->get_prop_val(PROP_DLUGOSC))
            {
                case B_KROTKI:
                    skl->quality+=1;
                    skl->latwosc_uzycia+=1;
                    break;
                case KROTKI:
                    skl->quality+=10;
                    skl->latwosc_uzycia+=1;
                    break;
                case DLUGI:
                    skl->quality+=1;
                    skl->latwosc_uzycia+=1;
                    skl->wytrzymalosc=20;
                    break;
                case  B_DLUGI:
                    skl->quality+=1;
                    skl->latwosc_uzycia+=3;
                    skl->wytrzymalosc=30;
                    break;
            }
            switch (m->get_prop_val(PROP_GLADKOSC))
            {
                case GLADKI:
                    skl->quality+=40;
                    skl->latwosc_uzycia+=1;
                break;
                case SZORSTKI:
                    skl->quality+=1;
                    skl->latwosc_uzycia+=3;
                break;
            }

            switch (m->get_prop_val(PROP_TWARDOSC))
            {
                case MIEKKI:
                    skl->quality+=1;
                    skl->wytrzymalosc=60;
                    skl->latwosc_uzycia+=5;
                break;
                case TWARDY:
                    skl->quality+=30;
                    skl->wytrzymalosc=1;
                    skl->latwosc_uzycia+=1;
                break;
            }
            skl->show(false);

            return skl;
        }
        delete skl;
    }
     printf("%s can't transform %s to Rączka młotka\n", colorRedBold, m->get_class_name());
    return nullptr;
}
