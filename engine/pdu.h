  
#include "common/types.h"

typedef enum bc_channel
    {
        zero    = 0,  other      = 0,
        one     = 1,  le         = 1,
        two     = 2,  bccmd      = 2, h2hc = 2,
        three   = 3,  hq         = 3, hc2h = 3,
        four    = 4,  dm         = 4,
        five    = 5,  hciCommand = 5,
        six     = 6,  hciACL     = 6,
        seven   = 7,  hciSCO     = 7,
        eight   = 8,  l2cap      = 8,
        nine    = 9,  rfcomm     = 9,
        ten     = 10, sdp        = 10,
        eleven  = 11, debug      = 11,
        twelve  = 12, upgrade    = 12,
        thirteen= 13, vm         = 13,
        fourteen= 14,
        fifteen = 15,
        bcsp_channel_count = 16, // 0x10
        lmp_debug = 20,
        h4dp = 21,
        raw = 22,
        csr_hci_extensions_count = 64 // 0x40
    }enum_channel;



