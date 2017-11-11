#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

enum {
    COBJECT = 0,
    GUIBUTTON,
    GUICHECK,
    GUIPANELBORDER,
    GUILISTBOX,
    GUIPICTURE,
    GUIINPUTWIDE,
    GUIARTSURFACE,
    GUILABEL,
    GUIPROGRESSBAR,
    GUIPANEL,
    GUIPOPUPMENU,
    GUICOMBOBOX,
    GUITEXTELEMENTSAREA,
    GUISPECTRUM,
    GUILISTGROUPBOX,
    GUISLIDER,
    GUITREELISTBOX
};

    struct t_region{
        int selX;
        int selY;
        int selW;
        int selH;
    };


    struct tEvento{
        int key;
        int keyMod;
        int unicode;
        int joy;
        int mouse;
        int mouse_x;
        int mouse_y;
        int mouse_state;
        t_region region;
        bool isMousedblClick;
        bool resize;
        bool isJoy;
        bool isKey;
        bool isMouse;
        bool isMouseMove;
        bool isRegionSelected;
        bool quit;
        bool keyjoydown;
    };

#endif // COMMON_H_INCLUDED
