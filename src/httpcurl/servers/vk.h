#ifndef VK_H
#define VK_H

#include "httpcurl/httputil.h"
#include "httpcurl/htmlparser.h"
#include "httpcurl/util/ConstantHttp.h"

class vk
{
    public:
        vk();
        virtual ~vk();
        string download(string url);
    protected:
    private:
        string getVideoLink(string str1, string str2, string str3, string str4);
};

#endif // VK_H
