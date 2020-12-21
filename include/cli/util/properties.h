/* Properties Header */

#ifndef PROPERTIES_H
#define PROPERTIES_H

typedef struct _win_properties {
    
    int height;
    int width;
    int y;
    int x;

} win_properties_t;

typedef struct _properties {
    
    struct _win_properties window;

    int page_init;
    int page_range;
    
} properties_t;

#endif
