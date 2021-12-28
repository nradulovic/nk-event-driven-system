/*
 * eds_trace.h
 *
 *  Created on: Dec 22, 2021
 *      Author: nenad
 */

#ifndef NEON_KIT_EDS_SOURCE_EDS_TRACE_H_
#define NEON_KIT_EDS_SOURCE_EDS_TRACE_H_

#include <stdint.h>

#define EDS_TRACE__SOURCE_AGENT_CREATE      0x01
#define EDS_TRACE__SOURCE_EDS_TMR           0xf0

#if defined(EDS_TRACE__ENABLE)
#define EDS_TRACE__ERROR(source, error, format, ...)    eds_trace__error(source, error, format, __VA_ARGS__)
#else
#define EDS_TRACE__ERROR(source, error, format, ...)
#endif

#if defined(EDS_TRACE__ENABLE)
#define EDS_TRACE__INFO(source, format, ...)            eds_trace__info(source, format, __VA_ARGS__)
#else
#define EDS_TRACE__INFO(source, format, ...)
#endif

extern const char * eds_trace__g__sources[];

extern void
eds_trace__error(uint32_t, uint32_t, const char * , ...);

extern void
eds_trace__info(uint32_t, const char *, ...);

#endif /* NEON_KIT_EDS_SOURCE_EDS_TRACE_H_ */
