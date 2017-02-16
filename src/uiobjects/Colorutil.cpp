#include "Colorutil.h"

/**
* Conversion extraida de http://130.113.54.154/~monger/hsl-rgb.html
*/
Colorutil::Colorutil(){
}
/**
*
*/
Colorutil::~Colorutil(){
}

void Colorutil::incBright(double r, double g, double b, int diferencia){
    rgb2hsl(r, g, b);
    setL(getL()+diferencia);
    hsl2rgb(getH(), getS(), getL());
}


/**
*
*/
unsigned int Colorutil::incBright(unsigned int hexRGB, int diferencia){
    hex2rgb(hexRGB);
    rgb2hsl(getR(), getG(), getB());
    setL(getL()+diferencia);
    hsl2rgb(getH(), getS(), getL());
    return rgb2hex((int)getR(), (int)getG(), (int)getB());
}

/**
* TODO: ATENCION - FALTA COMPROBAR QUE ESTAMOS EN LITTLEENDIAN O BIGENDIAN
*/
void Colorutil::hex2rgb(int hexRGB){
    setR((hexRGB & 0xFF0000) >> 16 );
    setG((hexRGB & 0x00FF00) >> 8);
    setB((hexRGB & 0x0000FF));
}




/**
* TODO: ATENCION - FALTA COMPROBAR QUE ESTAMOS EN LITTLEENDIAN O BIGENDIAN
*/
unsigned int Colorutil::rgb2hex( int r, int g, int b){
    unsigned int hex = 0x00000000;
    hex = 0x000000FF & b;
    hex |= (0x000000FF & g) << 8;
    hex |= (0x000000FF & r) << 16;
    return hex;
}

/**
*
*/
void Colorutil::hex2all(int hexRGB){
    hex2rgb(hexRGB);
    rgb2hsl(getR(),getG(),getB());
}

/**
*
*/
t_color Colorutil::getColor(){
     t_color color = {(int)getR(),(int)getG(),(int)getB()};
     return color;
}

/**
*
*/
void Colorutil::hsl2rgb(double h, double s, double l){
    double temp1 = 0.;
    double temp2 = 0.;
    double rtemp3 = 0.;
    double gtemp3 = 0.;
    double btemp3 = 0.;

  	l = l/100;
	s = s/100;
	h = h/360;


    //1. If S=0, define R, G, and B all to L
    if(s == 0.) {
        setR(l);
        setG(l);
        setB(l);
    } else {
//        2. Otherwise, test L.
//           If L < 0.5, temp2=L*(1.0+S)
//           If L >= 0.5, temp2=L+S - L*S

        if (l < 0.5){
            temp2=l*(1.0+s);
        } else {
            temp2=l+s - l*s;
        }
        // 3. temp1 = 2.0*L - temp2
        temp1 = 2.0*l - temp2;

        //4. Convert H to the range 0-1
        //In the example, H=120/360 = .33

//        5. For each of R, G, B, compute another temporary value, temp3, as follows:
//        for R, temp3=H+1.0/3.0
//        for G, temp3=H
//        for B, temp3=H-1.0/3.0
//        if temp3 < 0, temp3 = temp3 + 1.0
//        if temp3 > 1, temp3 = temp3 - 1.0
        rtemp3=h+1.0/3.0;
        gtemp3=h;
        btemp3=h-1.0/3.0;

        if (rtemp3 < 0) rtemp3 += 1.0;
        if (rtemp3 > 1) rtemp3 -= 1.0;
        if (gtemp3 < 0) gtemp3 += 1.0;
        if (gtemp3 > 1) gtemp3 -= 1.0;
        if (btemp3 < 0) btemp3 += 1.0;
        if (btemp3 > 1) btemp3 -= 1.0;

    //6. For each of R, G, B, do the following test:
    //   If 6.0*temp3 < 1, color=temp1+(temp2-temp1)*6.0*temp3
    //   Else if 2.0*temp3 < 1, color=temp2
    //   Else if 3.0*temp3 < 2, color=temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0
    //   Else color=temp1
        setR(color(temp1, temp2, rtemp3)*255);
        setG(color(temp1, temp2, gtemp3)*255);
        setB(color(temp1, temp2, btemp3)*255);
    }
}

/**
*
*/
double Colorutil::color(double temp1, double temp2, double temp3){
    double res = 0.;

    if (6.0*temp3 < 1) res=temp1+(temp2-temp1)*6.0*temp3;
    else if (2.0*temp3 < 1) res=temp2;
    else if (3.0*temp3 < 2) res=temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0;
    else res=temp1;

    return res;
}

/**
*
*/
void Colorutil::rgb2hsl(double r, double g, double b){
    //double hsl[3];
    double maxcolor = 0.;
    double mincolor = 0.;
    //1. Convert the RBG values to the range 0-1
    double rfl = r/(double)255;
    double gfl = g/(double)255;
    double bfl = b/(double)255;

    //2. Find min and max values of R, B, G
    maxcolor = rfl;
    if (gfl > maxcolor) maxcolor = gfl;
    if (bfl > maxcolor) maxcolor = bfl;

    mincolor = rfl;
    if (gfl < mincolor) mincolor = gfl;
    if (bfl < mincolor) mincolor = bfl;

    //3. L = (maxcolor + mincolor)/2
    setL((maxcolor + mincolor)/2);



    //4. If the max and min colors are the same (ie the
    //color is some kind of grey), S is defined to be 0,
    //and H is undefined but in programs usually written as 0
    if (maxcolor == mincolor){
        setS(0);
        setH(0);
    } else{
//    5. Otherwise, test L.
//       If L < 0.5, S=(maxcolor-mincolor)/(maxcolor+mincolor)
//       If L >=0.5, S=(maxcolor-mincolor)/(2.0-maxcolor-mincolor)
        if (getL() < 0.5) {
            setS((maxcolor-mincolor)/(maxcolor+mincolor));
        } else {
            setS((maxcolor-mincolor)/(2.0-maxcolor-mincolor));
        }

        //6. If R=maxcolor, H = (G-B)/(maxcolor-mincolor)
        //   If G=maxcolor, H = 2.0 + (B-R)/(maxcolor-mincolor)
        //   If B=maxcolor, H = 4.0 + (R-G)/(maxcolor-mincolor)
        if (rfl == maxcolor) {
            setH((gfl-bfl)/(maxcolor-mincolor));
        }
        if (gfl == maxcolor) {
            setH(2.0+(bfl-rfl)/(maxcolor-mincolor));
        }
        if (bfl == maxcolor) {
            setH(4.0+(rfl-gfl)/(maxcolor-mincolor));
        }
    }

//    7. To use the scaling shown in the video color page,
    //convert L and S back to percentages, and H into an angle
    //in degrees (ie scale it from 0-360). From the computation
    //in step 6, H will range from 0-6. RGB space is a cube,
    //and HSL space is a double hexacone, where L is the principal
    //diagonal of the RGB cube. Thus corners of the RGB cube;
    //red, yellow, green, cyan, blue, and magenta, become the vertices
    //of the HSL hexagon. Then the value 0-6 for H tells you which
    //section of the hexgon you are in. H is most commonly given as in
    //degrees, so to convert
    //H = H*60.0
    //If H is negative, add 360 to complete the conversion.

    setH(getH()*60);
    if (getH() < 0) setH(getH()+360);

    setS(getS()*100);
    setL(getL()*100);
}

/**
*
*/
void Colorutil::calcDegradation(t_color iniColor, t_color finColor, int maxRange){
    degradation.iniColor = iniColor;
    degradation.finColor = finColor;
    degradation.maxColorRange = maxRange;
    degradation.posColorRange = 0;

    degradation.deltaR = (finColor.r - iniColor.r)/(double)maxRange;
    degradation.deltaG = (finColor.g - iniColor.g)/(double)maxRange;
    degradation.deltaB = (finColor.b - iniColor.b)/(double)maxRange;

    degradation.auxR = iniColor.r;
    degradation.auxG = iniColor.g;
    degradation.auxB = iniColor.b;
}

/**
*
*/
void Colorutil::getDegradedColor(int posRange, t_color *resultColor){
    degradation.auxR += degradation.deltaR * posRange;
    degradation.auxG += degradation.deltaG * posRange;
    degradation.auxB += degradation.deltaB * posRange;

    resultColor->r = degradation.auxR;
    resultColor->g = degradation.auxG;
    resultColor->b = degradation.auxB;


}





