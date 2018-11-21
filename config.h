#ifndef __X_CONFIG_H
#define __X_CONFIG_H

// default enable json
#define XTOSTRUCT_JSON

//#define XTOSTRUCT_XML
//#define XTOSTRUCT_LIBCONIFG
//#define XTOSTRUCT_BSON

#if (defined _MSC_VER || defined WINNT)
    #ifndef WINDOWS
        #define WINDOWS
    #endif
#endif

#endif
