#ifndef ADDRESSES_H
#define ADDRESSES_H

#include <msp430.h>

#ifndef __MSP430_BASEADDRESS_PORT1_R__
#define __MSP430_BASEADDRESS_PORT1_R__ PAIN_
#endif

#ifndef __MSP430_BASEADDRESS_PORT2_R__
#define __MSP430_BASEADDRESS_PORT2_R__ PAIN_
#endif

#ifndef __MSP430_BASEADDRESS_PORT3_R__
#define __MSP430_BASEADDRESS_PORT3_R__ PBIN_
#endif

#ifndef __MSP430_BASEADDRESS_RTC__
#define __MSP430_BASEADDRESS_RTC__ RTCCTL01_L
#endif
#endif // ADDRESSES_H
