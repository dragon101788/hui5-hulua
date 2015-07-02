
#ifndef __TRANSCODING__
#define __TRANSCODING__

#include <stdint.h>
#include "hulib.h"
typedef int32_t unichar;
typedef uint32_t size_t;
#define ToS8(x)   ((int8_t)(x))
#define ToU8(x)   ((uint8_t)(x))
#define ToS16(x)  ((int16_t)(x))
#define ToU16(x)  ((uint16_t)(x))
#define ToS32(x)  ((int32_t)(x))
#define ToU32(x)  ((uint32_t)(x))
#define ToInt(x)  ((int)(x))
#define ToUInt(x) ((unsigned)(x))
#define ToSizeT(x) ((size_t)(x))
#define ToOffT(x) ((off_t)(x))


#define kMaxBytesInUTF8Sequence     4

#ifdef SK_DEBUG
    int UTF8_LeadByteToCount(unsigned c);
#else
    #define UTF8_LeadByteToCount(c)   ((((0xE5 << 24) >> ((unsigned)c >> 4 << 1)) & 3) + 1)
#endif

inline int UTF8_CountUTF8Bytes(const char utf8[]) {
    return UTF8_LeadByteToCount(*(const uint8_t*)utf8);
}

int         UTF8_CountUnichars(const char utf8[]);
int         UTF8_CountUnichars(const char utf8[], size_t byteLength);
unichar   UTF8_ToUnichar(const char utf8[]);
unichar   UTF8_NextUnichar(const char**);
unichar   UTF8_PrevUnichar(const char**);

/** Return the number of bytes need to convert a unichar
    into a utf8 sequence. Will be 1..kMaxBytesInUTF8Sequence,
    or 0 if uni is illegal.
*/
size_t      UTF8_FromUnichar(unichar uni, char utf8[] = NULL);

///////////////////////////////////////////////////////////////////////////////

#define UTF16_IsHighSurrogate(c)  (((c) & 0xFC00) == 0xD800)
#define UTF16_IsLowSurrogate(c)   (((c) & 0xFC00) == 0xDC00)

int UTF16_CountUnichars(const uint16_t utf16[]);
int UTF16_CountUnichars(const uint16_t utf16[], int numberOf16BitValues);
// returns the current unichar and then moves past it (*p++)
unichar UTF16_NextUnichar(const uint16_t**);
// this guy backs up to the previus unichar value, and returns it (*--p)
unichar UTF16_PrevUnichar(const uint16_t**);
size_t UTF16_FromUnichar(unichar uni, uint16_t utf16[] = NULL);

size_t UTF16_ToUTF8(const uint16_t utf16[], int numberOf16BitValues,
                      char utf8[] = NULL);

inline bool Unichar_IsVariationSelector(unichar uni) {
/*  The 'true' ranges are:
 *      0x180B  <= uni <=  0x180D
 *      0xFE00  <= uni <=  0xFE0F
 *      0xE0100 <= uni <= 0xE01EF
 */
    if (uni < 0x180B || uni > 0xE01EF) {
        return false;
    }
    if ((uni > 0x180D && uni < 0xFE00) || (uni > 0xFE0F && uni < 0xE0100)) {
        return false;
    }
    return true;
}


#endif
