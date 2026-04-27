#ifndef SAMPLE_DATA_H
#define SAMPLE_DATA_H

#include "../include/member.h"
#include "../include/violation.h"
#include "../include/auth.h"

void seedSampleData(Member members[], int *mCount,
                    Violation violations[], int *vCount,
                    Account accounts[], int *aCount);

#endif
