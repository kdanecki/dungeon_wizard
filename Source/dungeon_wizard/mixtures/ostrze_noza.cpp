#include "skladnik.h"

Skladnik * mix_Ostrze_noza(Element* m)
{
    Skladnik * skl = new Skladnik(CLASS_OSTRZE_NOZA, CAT_CZESC, m->quantity);
    const int cat_count=3;
    //check m->quantity
    Property_categories cat[cat_count]= {PROP_CIEZAR, PROP_OSTROSC, PROP_TWARDOSC};

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
                skl->quality+=1;
                skl->latwosc_uzycia+=5;
            break;
        }

        switch (m->get_prop_val(PROP_OSTROSC))
        {
            case TEPY:
                skl->quality+=1;
                skl->latwosc_uzycia+=5;
                skl->wytrzymalosc=20;
            break;
            case OSTRY:
                skl->quality+=45;
                skl->wytrzymalosc=1;
                skl->latwosc_uzycia+=1;
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
                skl->quality+=45;
                skl->wytrzymalosc=1;
                skl->latwosc_uzycia+=1;
            break;
        }
        skl->show(false);

        return skl;
    }

    delete skl;
    return nullptr;
}
