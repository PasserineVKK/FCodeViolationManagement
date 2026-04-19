#ifndef SAMPLE_DATA_H
#define SAMPLE_DATA_H

#include "member.h"
#include "violation.h"
#include "auth.h"

void seedSampleData(Member members[], int *mCount, 
                    Violation violations[], int *vCount, 
                    Account accounts[], int *aCount);

#endif