//
// Created by work on 10.02.2019.
//

#ifndef PRAO_COMPRESSER_TIME_H
#define PRAO_COMPRESSER_TIME_H


#include <ctime>

/// subs from tm 1900 years and makes mon in range [0-11]
int tm_SubDefault(tm &time);


/// subs from tm 4 hours and makes it in default view
int tm_toUTC(tm &time, int seconds = 60 * 60 * 4);

#endif //PRAO_COMPRESSER_TIME_H
