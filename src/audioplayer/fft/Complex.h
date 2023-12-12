#ifndef ComplexD_H
#define ComplexD_H

#include <cmath>

//------------------------------------
//  ComplexD.h
//  ComplexD number
//  (c) Reliable Software, 1996
//------------------------------------

class ComplexD
{
public:
    ComplexD () {}
    ComplexD (double re): _re(re), _im(0.0) {}
    ComplexD (double re, double im): _re(re), _im(im) {}
    double Re () const { return _re; }
    double Im () const { return _im; }
    void operator += (const ComplexD& c)
    {
        _re += c._re;
        _im += c._im;
    }
    void operator -= (const ComplexD& c)
    {
        _re -= c._re;
        _im -= c._im;
    }
    void operator *= (const ComplexD& c)
    {
        double reT = c._re * _re - c._im * _im;
        _im = c._re * _im + c._im * _re;
        _re = reT;
    }
    ComplexD operator- ()
    {
            return ComplexD (-_re, -_im);
    }
    double Mod () const { return sqrt (_re * _re + _im * _im); }
private:
    double _re;
    double _im;
};

#endif
