

#include "transcoding.h"



///////////////////////////////////////////////////////////////////////////////

/*  0xxxxxxx    1 total
    10xxxxxx    // never a leading byte
    110xxxxx    2 total
    1110xxxx    3 total
    11110xxx    4 total

    11 10 01 01 xx xx xx xx 0...
    0xE5XX0000
    0xE5 << 24
*/


int UTF8_CountUnichars(const char utf8[]) {
    int count = 0;

    for (;;) {
        int c = *(const uint8_t*)utf8;
        if (c == 0) {
            break;
        }
        utf8 += UTF8_LeadByteToCount(c);
        count += 1;
    }
    return count;
}

int UTF8_CountUnichars(const char utf8[], size_t byteLength) {
    int         count = 0;
    const char* stop = utf8 + byteLength;

    while (utf8 < stop) {
        utf8 += UTF8_LeadByteToCount(*(const uint8_t*)utf8);
        count += 1;
    }
    return count;
}

unichar UTF8_ToUnichar(const char utf8[]) {
    const uint8_t*  p = (const uint8_t*)utf8;
    int             c = *p;
    int             hic = c << 24;


    if (hic < 0) {
        uint32_t mask = (uint32_t)~0x3F;
        hic <<= 1;
        do {
            c = (c << 6) | (*++p & 0x3F);
            mask <<= 5;
        } while ((hic <<= 1) < 0);
        c &= ~mask;
    }
    return c;
}

unichar UTF8_NextUnichar(const char** ptr) {

    const uint8_t*  p = (const uint8_t*)*ptr;
    int             c = *p;
    int             hic = c << 24;


    if (hic < 0) {
        uint32_t mask = (uint32_t)~0x3F;
        hic <<= 1;
        do {
            c = (c << 6) | (*++p & 0x3F);
            mask <<= 5;
        } while ((hic <<= 1) < 0);
        c &= ~mask;
    }
    *ptr = (char*)p + 1;
    return c;
}

unichar UTF8_PrevUnichar(const char** ptr) {

    const char* p = *ptr;

    if (*--p & 0x80) {
        while (*--p & 0x40) {
            ;
        }
    }

    *ptr = (char*)p;
    return UTF8_NextUnichar(&p);
}

size_t UTF8_FromUnichar(unichar uni, char utf8[]) {
    if ((uint32_t)uni > 0x10FFFF) {
      //  DEBUGFAIL("bad unichar");
        return 0;
    }

    if (uni <= 127) {
        if (utf8) {
            *utf8 = (char)uni;
        }
        return 1;
    }

    char    tmp[4];
    char*   p = tmp;
    size_t  count = 1;

 //   SkDEBUGCODE(unichar orig = uni;)

    while (uni > 0x7F >> count) {
        *p++ = (char)(0x80 | (uni & 0x3F));
        uni >>= 6;
        count += 1;
    }

    if (utf8) {
        p = tmp;
        utf8 += count;
        while (p < tmp + count - 1) {
            *--utf8 = *p++;
        }
        *--utf8 = (char)(~(0xFF >> count) | uni);
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////

int UTF16_CountUnichars(const uint16_t src[]) {


    int count = 0;
    unsigned c;
    while ((c = *src++) != 0) {
        if (UTF16_IsHighSurrogate(c)) {
            c = *src++;

        }
        count += 1;
    }
    return count;
}

int UTF16_CountUnichars(const uint16_t src[], int numberOf16BitValues) {
    const uint16_t* stop = src + numberOf16BitValues;
    int count = 0;
    while (src < stop) {
        unsigned c = *src++;
        if (UTF16_IsHighSurrogate(c)) {
            c = *src++;
        }
        count += 1;
    }
    return count;
}

unichar UTF16_NextUnichar(const uint16_t** srcPtr) {
    const uint16_t* src = *srcPtr;
    unichar       c = *src++;
    if (UTF16_IsHighSurrogate(c)) {
        unsigned c2 = *src++;

        // c = ((c & 0x3FF) << 10) + (c2 & 0x3FF) + 0x10000
        // c = (((c & 0x3FF) + 64) << 10) + (c2 & 0x3FF)
        c = (c << 10) + c2 + (0x10000 - (0xD800 << 10) - 0xDC00);
    }
    *srcPtr = src;
    return c;
}

unichar UTF16_PrevUnichar(const uint16_t** srcPtr) {
    const uint16_t* src = *srcPtr;
    unichar       c = *--src;

    if (UTF16_IsLowSurrogate(c)) {
        unsigned c2 = *--src;
        c = (c2 << 10) + c + (0x10000 - (0xD800 << 10) - 0xDC00);
    }
    *srcPtr = src;
    return c;
}

size_t UTF16_FromUnichar(unichar uni, uint16_t dst[]) {
    int extra = (uni > 0xFFFF);

    if (dst) {
        if (extra) {
            // dst[0] = SkToU16(0xD800 | ((uni - 0x10000) >> 10));
            // dst[0] = SkToU16(0xD800 | ((uni >> 10) - 64));
            dst[0] = ToU16((0xD800 - 64) + (uni >> 10));
            dst[1] = ToU16(0xDC00 | (uni & 0x3FF));

        } else {
            dst[0] = ToU16(uni);

        }
    }
    return 1 + extra;
}

size_t UTF16_ToUTF8(const uint16_t utf16[], int numberOf16BitValues,
                      char utf8[]) {
    if (numberOf16BitValues <= 0) {
        return 0;
    }

    const uint16_t* stop = utf16 + numberOf16BitValues;
    size_t          size = 0;

    if (utf8 == NULL) {    // just count
        while (utf16 < stop) {
            size += UTF8_FromUnichar(UTF16_NextUnichar(&utf16), NULL);
        }
    } else {
        char* start = utf8;
        while (utf16 < stop) {
            utf8 += UTF8_FromUnichar(UTF16_NextUnichar(&utf16), utf8);
        }
        size = utf8 - start;
    }
    return size;
}
