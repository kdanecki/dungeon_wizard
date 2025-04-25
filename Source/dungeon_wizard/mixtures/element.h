#ifndef ELEMENT_H
#define ELEMENT_H

#include "properties.h"
#include "categories_type.h"
#include "class_id.h"
#include "class_names.h"
#include <cstdio>

class Element: public Basic_el
{
    Class_id id;
    Prop_list *props;
    Categories_type category;

    void initialize(Class_id i, Categories_type cat, int q)
    {
        id = i;
        category = cat;
        props = new Prop_list("props");
        mixed = false;
        quantity = q;
    }

public:
    bool mixed;
    int quantity;

        Element(Class_id i, Categories_type cat, int q )
        {
            initialize(i, cat, q);

        }
        Element(Class_id i, Categories_type cat )
        {
            initialize(i, cat, rand() % 100);

        }
        const char * get_class_name() { return class_names[id]; }
        void show(bool details=true) {
            printf("%s%s ", colorRedBold, get_class_name());
            if (details) props->show();
        }
        int get_prop_val(Property_categories i)
        {
            Property * p = get_prop(i);
            return p ? p->get_val() : 0;
        }
        Property *get_prop(Property_categories i)
        {
            return (Property*) props->find(&i);
        }
        Property *get_prop_and_clone(Property_categories i) {
            return ((Property*) props->find(&i))->clone_prop();
        }

        bool check_prop(Property_categories c) {
            if (!get_prop_val(c)) {
                printf("%scan't create (%s)", colorRedBold, get_property_cat_name(c));
                return false;
            } else return true;
        }
        bool check_prop_val(Property_categories cat, Property_values val)
        {
            return get_prop_val(cat) == val;
        }
        bool check_props(int cnt, Property_categories * cat) {
            for (int i=0; i < cnt; i++)
            {
                if (!check_prop(cat[i])) return false;
            }
            return true;
        }
        void add_prop(Property *p)
        {
            props->add(p);
        }
        Property * find_prop(Property_categories cat)
        {
            return (Property *) props->find(&cat);
        }
};

#endif

