#ifndef STREAMCLOUD_H
#define STREAMCLOUD_H

#include "httputil.h"
#include "htmlparser.h"
#include "util/ConstantHttp.h"
#include "soutils/SOUtils.h"

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
