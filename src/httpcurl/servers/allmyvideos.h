#ifndef ALLMYVIDEOS_H
#define ALLMYVIDEOS_H

#include "httputil.h"
#include "htmlparser.h"
#include "util/ConstantHttp.h"

class Allmyvideos
{
    public:
        Allmyvideos();
        virtual ~Allmyvideos();

        string download(string url);
    protected:
    private:
};

#endif // ALLMYVIDEOS_H
