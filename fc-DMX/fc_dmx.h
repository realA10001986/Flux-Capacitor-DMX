/*
 * -------------------------------------------------------------------
 * CircuitSetup.us Flux Capacitor - DMX-controlled
 * (C) 2024 Thomas Winischhofer (A10001986)
 * All rights reserved.
 * -------------------------------------------------------------------
 */

#ifndef _FC_DMX_H
#define _FC_DMX_H

extern unsigned long powerupMillis;

void dmx_boot();
void dmx_setup();
void dmx_loop();


void showWaitSequence();
void endWaitSequence();
void showCopyError();

#endif
