#ifndef MODULE_H_
#define MODULE_H_

#include "gbmint.h"

struct gbm_device *
_gbm_create_device(int fd);

void
_gbm_device_destroy(struct gbm_device *gbm);

#endif
