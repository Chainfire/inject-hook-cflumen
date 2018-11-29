/* Copyright (c) 2015, Simone 'evilsocket' Margaritelli
   Copyright (c) 2015, Jorrit 'Chainfire' Jongma
   See LICENSE file for details */

#include "libhook/hook.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void hook();

#ifdef __cplusplus
}
#endif

int hooked = 0;
void __attribute__ ((constructor)) OnLoad() {
    if (hooked) return;
#ifndef DEBUG
    libhook_log(NULL);
#endif
    hooked = 1;
    hook();
}
