#ifndef INTERNAL_H_
#define INTERNAL_H_

#include "gbm_backend_abi.h"

/* GCC visibility */
#if defined(__GNUC__)
#define GBM_EXPORT __attribute__ ((visibility("default")))
#else
#define GBM_EXPORT
#endif

/**
 * \file gbmint.h
 * \brief Internal implementation details of gbm
 */

extern struct gbm_core gbm_core;

#endif
