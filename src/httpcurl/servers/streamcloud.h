#ifndef STREAMCLOUD_H
#define STREAMCLOUD_H

#include "httpcurl/httputil.h"
#include "httpcurl/htmlparser.h"
#include "httpcurl/util/ConstantHttp.h"
#include "uiobjects/soutils/SOUtils.h"

class Streamcloud
{
    public:
        Streamcloud();
        virtual ~Streamcloud();
        string download(string url);
    protected:
    private:
};

#endif // STREAMCLOUD_H
