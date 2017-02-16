#ifndef COLORUTIL_H_INCLUDED
#define COLORUTIL_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include "Constant.h"

using namespace std;

class Colorutil{
    public :
        Colorutil();
        ~Colorutil();
        void rgb2hsl(double, double, double);
        void hsl2rgb(double, double, double);
        void hex2rgb(int);
        unsigned int rgb2hex( int, int, int);
        unsigned int incBright(unsigned int, int);
        void hex2all(int hexRGB);
        void incBright(double, double, double, int);

        t_color getColor();
        double getH(){ return hsl[0];}
        double getS(){ return hsl[1];}
        double getL(){ return hsl[2];}
        double getR(){ return rgb[0];}
        double getG(){ return rgb[1];}
        double getB(){ return rgb[2];}
        void setH(double tH){hsl[0] = tH;}
        void setS(double tS){hsl[1] = tS;}
        void setL(double tL){hsl[2] = tL;}
        void setR(double tR){rgb[0] = tR;}
        void setG(double tG){rgb[1] = tG;}
        void setB(double tB){rgb[2] = tB;}

        void calcDegradation(t_color iniColor, t_color finColor, int maxRange);
        void getDegradedColor(int posRange, t_color *resultColor);


    private :
        double rgb[3];
        double hsl[3];
        double color(double, double, double);


        struct tDegradation{
            int maxColorRange;
            int posColorRange;
            t_color iniColor;
            t_color finColor;
            double deltaR;
            double deltaG;
            double deltaB;
            double auxR;
            double auxG;
            double auxB;
        } degradation;
};

#endif // COLORUTIL_H_INCLUDED
