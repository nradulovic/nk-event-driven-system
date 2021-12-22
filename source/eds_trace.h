/*
 * eds_trace.h
 *
 *  Created on: Dec 22, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_EDS_SOURCE_EDS_TRACE_H_
#define NEON_KIT_EDS_SOURCE_EDS_TRACE_H_

#define EDS_TRACE__SOURCE_AGENT_CREATE      0x01

#ifndef EDS_TRACE__ERROR
#define EDS_TRACE__ERROR(source, error, format, ...)
#endif

#ifndef EDS_TRACE__INFO
#define EDS_TRACE__INFO(source, format, ...)
#endif

extern const char * eds_trace__g__sources[];

#endif /* NEON_KIT_EDS_SOURCE_EDS_TRACE_H_ */
