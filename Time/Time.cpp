//
// Created by work on 10.02.2019.
//

#include "Time.h"

int tm_SubDefault(tm &time){
    if (time.tm_year > 200){
        time.tm_year -= 1900;
        time.tm_mon -= 1;
    }
    return mktime(&time);
}