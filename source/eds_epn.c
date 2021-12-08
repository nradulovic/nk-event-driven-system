
#include "eds_epn.h"
#include "eds.h"
#include "eds_mem.h"
#include "eds_core.h"
#include "eds_port.h"
#include "eds_epa.h"

eds__error
eds__epn_create(const struct eds__epn_attr *attr, eds__network **network)
{
    static const struct eds__epn_attr default_attr =
    {
        .name = EDS__DEFAULT_EPN_NAME
    };
    struct eds_object__epn * epn;
    struct eds_object__mem * mem;

    if (network == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (attr == NULL) {
        attr = &default_attr;
    }
    if (attr->instance == NULL) {
        struct eds_port__critical critical;

        mem = eds_mem__select(sizeof(*epn));
        if (mem == NULL) {
            return EDS__ERROR_NO_RESOURCE;
        }
        eds_port__critical_lock(&critical);
        epn = eds_mem__allocate_from(mem, sizeof(*epn));
        eds_port__critical_unlock(&critical);
        if (epn == NULL) {
            return EDS__ERROR_NO_MEMORY;
        }
    } else {
        mem = NULL;
        epn = attr->instance;
    }
    eds_core__escheduler_init(&epn->p__scheduler);
    eds_port__sleep_init(&epn->p__sleep);
    epn->p__mem = mem;
    epn->p__name = attr->name != NULL ? attr->name : EDS__DEFAULT_EPN_NAME;
    *network = epn;
    return EDS__ERROR_NONE;
}

eds__error
eds__epn_delete(eds__network *epn)
{
    struct eds_port__critical critical;
    struct eds_object__list epa_list;
    struct eds_object__list * current;

    if (epn == NULL) {
        return EDS__ERROR_INVALID_ARGUMENT;
    }
    if (epn->p__mem == NULL) {
        return EDS__ERROR_NO_PERMISSION;
    }
    eds_port__critical_lock(&critical);
    eds_core__escheduler_export(&epn->p__scheduler, &epa_list);
    eds_port__critical_unlock(&critical);

    for (EDS_CORE__LIST_EACH(current, &epa_list)) {
        struct eds_object__epa * epa;

        epa = EDS_CORE__CONTAINER_OF(current, struct eds_object__epa, p__list);
        eds_epa__deallocate(epa);
    }
    return EDS__ERROR_NONE;
}

eds__error
eds__epn_add_epa(eds__network *epn, eds__agent *sm);

eds__error
eds__epn_remove_epa(eds__network *epn, eds__agent *sm);

eds__error
eds__epn_start(eds__network *epn);

eds__error
eds__epn_stop(eds__network *epn);