#ifndef fileattr_H
#define fileattr_H

#include <string>
#include "Constant.h"
using namespace std;


    class FileProps{
        public:
            string filename;
            int filetype;
            string dir;
            int ico;
            
            bool comparar(const FileProps &c) const {
                string str1 = filename;
                string str2 = c.filename;
                Constant::lowerCase(&str1);
                Constant::lowerCase(&str2);
                return str1.compare(str2);
            }
            
            bool operator==(const FileProps &c) const {
                return !comparar(c);
            }
            bool operator!=(const FileProps &c) const {
                return comparar(c);
            }
            bool operator<(const FileProps &c) const {
                return comparar(c) < 0;
            }
            bool operator>(const FileProps &c) const {
                return comparar(c) > 0;
            }
            bool operator<=(const FileProps &c) const {
                return comparar(c) <= 0;
            }
            bool operator>=(const FileProps &c) const {
                return comparar(c) >= 0;
            }
    };
#endif // fileattr_H

