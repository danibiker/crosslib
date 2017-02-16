#ifndef UICHECK_H
#define UICHECK_H

#include <object.h>


class UICheck : public Object
{
    public:
        UICheck();
        virtual ~UICheck();
        void checkPos(tEvento evento);

    protected:
    private:
};

#endif // UICHECK_H
