#ifndef ALLMYVIDEOS_H
#define ALLMYVIDEOS_H

#include "httpcurl/httputil.h"
#include "httpcurl/htmlparser.h"
#include "httpcurl/util/ConstantHttp.h"

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
