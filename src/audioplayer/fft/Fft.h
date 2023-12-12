#if !defined FFT_H
#define FFT_H
//------------------------------------
//  fft.h
//  Fast Fourier Transform
//  (c) Reliable Software, 1996
//------------------------------------

#include "Complex.h"
#include "assert.h"
#include <stdio.h>
#include <string.h>

//class SampleIter;

class Fft
{
public:
    Fft (int Points, long sampleRate);
    ~Fft ();
    int     Points () const { return _Points; }
    void    Transform ();
    //void    CopyIn (SampleIter& iter);
    void CopyIn (void * _stream, int _len);

    double  GetIntensity (int i) const
    {
        assert (i < _Points);
        return _X[i].Mod()/_sqrtPoints;   //4096 => _sqrtPoints = 64
    }

    int     GetFrequency (int point) const
    {
        // return frequency in Hz of a given point
        assert (point < _Points);
        long x =_sampleRate * point;
        return x / _Points;
    }

    int     HzToPoint (int freq) const
    {
        return (long)_Points * freq / _sampleRate;
    }

    int     MaxFreq() const { return _sampleRate; }

    int     Tape (int i) const
    {
        assert (i < _Points);
        return (int) _aTape[i];
    }

private:

    void PutAt ( int i, double val )
    {
        _X [_aBitRev[i]] = ComplexD (val);
    }

    int			_Points;
    long		_sampleRate;
    int			_logPoints;
    double		_sqrtPoints;
    int		   *_aBitRev;       // bit reverse vector
    ComplexD	   *_X;             // in-place fft array
    ComplexD	  **_W;             // exponentials
    double     *_aTape;         // recording tape
};

#endif
