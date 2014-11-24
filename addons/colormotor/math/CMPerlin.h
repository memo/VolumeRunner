#pragma once

#include "CM.h" // just for win32 warnings

namespace cm
{

double noise1(double x);
double noise2(double x, double y);
double noise3(double x, double y, double z);

double noise1o(double x, int n = 3, double alpha = 2.0 ,double beta = 2.0);
double noise2o(double x, double y, int n = 3, double alpha = 2.0, double beta= 2.0);
double noise3o(double x, double y, double z, int n = 3, double alpha= 2.0, double beta= 2.0);

inline float noiseRange1( float x, float a, float b ) 
{
	return a + (noise1o(x)*0.5+0.5)*(b-a);
}

inline float noiseRange2( float x, float y, float a, float b ) 
{
	float n = noise2o(x,y);
	return a + (n*0.5+0.5)*(b-a);
}

inline float noiseRange3( float x, float y, float z, float a, float b ) 
{
	return a + (noise3o(x,y,z)*0.5+0.5)*(b-a);
}

float ridgedNoise( float x, float y, float offset = 1.0, float lacunarity = 1.7, float gain = 0.25, int octaves = 4);

}
