
#ifndef NEON_KIT_EDS_SOURCE_EDS_NETWORK_H_
#define NEON_KIT_EDS_SOURCE_EDS_NETWORK_H_

struct eds_object__epn;
struct eds_object__epa;

void
eds_epn__ready(struct eds_object__epn *epn, struct eds_object__epa *epa);

void
eds_epn__block(struct eds_object__epn * epn, struct eds_object__epa * epa);

void
eds_epn__add(struct eds_object__epn *epn, struct eds_object__epa *epa);

void
eds_epn__remove(struct eds_object__epn *epn, struct eds_object__epa *epa);

#endif /* NEON_KIT_EDS_SOURCE_EDS_NETWORK_H_ */