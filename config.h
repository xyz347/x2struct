#ifndef __X_CONFIG_H
#define __X_CONFIG_H

// default enable json
#define XTOSTRUCT_JSON

//#define XTOSTRUCT_XML
//#define XTOSTRUCT_LIBCONIFG
//#define XTOSTRUCT_BSON

//#define XTOSTRUCT_SUPPORT_CHAR_ARRAY

#if (defined _MSC_VER || defined WINNT)
    #ifndef WINDOWS
        #define WINDOWS
    #endif
#endif

// support qt
//#define XTOSTRUCT_QT


// support xhas
// xhas will add a std::set in struct and will crash if use memset
// if want to use memset, can close this macro, but xhas will not work
#define XTOSTRUCT_XHAS

#endif
