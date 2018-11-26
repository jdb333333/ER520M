#ifndef MESSAGE_H
#define MESSAGE_H

#if defined(CASE_FORHANZI)

//#include "MessageCH.h"
#include "MessageCH_C.h"
#include "MessageCH_D16.h"
#include "MessageCH_P32.h"
#else

#if defined(CASE_VIETNAM)//Ô½ÄÏ

#if defined(CHAR_6X8)
#include "MessageVN_C.h"
#include "MessageVN_D21.h"
#include "MessageVN_P32.h"
#else
#include "MessageVN_260.h"
#endif

#elif defined(CASE_SPANISH)

#include "MessageSP.h"

#elif defined(CHAR_6X8)

//#include "MessageEN_21N.h"
#include "MessageEN_C.h"
#include "MessageEN_D21.h"
#include "MessageEN_P32.h"

#else
#ifdef EXAM_VAT //jdb2018-08-11
#define NETVATVERSION 1 //jdb2018-08-11 net vat
#else
#define NETVATVERSION 0 //jdb2018-08-11 net vat
#endif
#define ORRECEIPT 0 //jdb2018-07-27 0 SI 1 OR
#include "MessageEN_260_M.h"
//#include "MessageSP.h" //jdb2018-07-20 Î÷°àÑÀÓï
#include "MessageEN_P32_M.h"

#endif

#endif

#endif
