/// ***FILEHEADER

#include "cmHomography.h"
#include "cmMathUtils.h"

namespace cm
{

M44 computeHomography(const std::vector<Vec2> &src, const std::vector<Vec2> &dst)
{
	return computeHomography(&src[0],&dst[0]);
}
M44 computeHomography(const Vec2 *src, const Vec2 *dst)
{  
	// Adapted from:
    // arturo castro - 08/01/2010  
    //  
    // create the equation system to be solved  
    //  
    // from: Multiple View Geometry in Computer Vision 2ed  
    //       Hartley R. and Zisserman A.  
    //  
    // x' = xH  
    // where H is the homography: a 3 by 3 matrix  
    // that transformed to inhomogeneous coordinates for each point  
    // gives the following equations for each point:  
    //  
    // x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13  
    // y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23  
    //  
    // as the homography is scale independent we can let h33 be 1 (indeed any of the terms)  
    // so for 4 points we have 8 equations for 8 terms to solve: h11 - h32  
    // after ordering the terms it gives the following matrix  
    // that can be solved with gaussian elimination:  
      
    float P[8][9]={  
        {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11  
        {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12  
          
        {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13  
        {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21  
          
        {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22  
        {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23  
          
        {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31  
        {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32  
    };  
      
    gaussianElimination(&P[0][0],9);
      
    // gaussian elimination gives the results of the equation system  
    // in the last column of the original matrix.  
	// fill in openGL compatible 4x4 matrix and return 
	
	M44 res;
	res.identity();
	
    // gaussian elimination gives the results of the equation system  
    // in the last column of the original matrix.  
    // opengl needs the transposed 4x4 matrix:  
	
    float aux_H[]={ P[0][8],P[3][8],0,P[6][8], // h11  h21 0 h31  
					P[1][8],P[4][8],0,P[7][8], // h12  h22 0 h32  
					0      ,      0,1,0,       // 0    0   0 0  
					P[2][8],P[5][8],0,1};      // h13  h23 0 h33  
      
    for(int i=0;i<16;i++) res.m[i] = aux_H[i];
	
	/*
	res.m11 = P[0][8]; res.m21 = P[3][8]; res.m41 = P[6][8];
	res.m12 = P[1][8]; res.m22 = P[4][8]; res.m42 = P[7][8];
	res.m14 = P[2][8]; res.m24 = P[5][8]; res.m44 = 1.0;*/
	/*
    float aux_H[]={ P[0][8],P[3][8],0,P[6][8], // h11  h21 0 h31  
        P[1][8],P[4][8],0,P[7][8], // h12  h22 0 h32  
        0      ,      0,1,0,       // 0    0   0 0  
        P[2][8],P[5][8],0,1};      // h13  h23 0 h33  
	*/
	return res;
}  


Homography::Homography()
//:
//selected(-1)
{
	srcPts.assign(4,Vec2(0,0));
	srcPts[0](0,0);
	srcPts[1](1,0);
	srcPts[2](1,1);
	srcPts[3](0,1);
	dstPts = srcPts;
	matrix.identity();
	invMatrix.identity();
}

void Homography::reset(float x, float y, float w, float h)
{
	srcPts[0](x,y);
	srcPts[1](x+w,y);
	srcPts[2](x+w,y+h);
	srcPts[3](x,y+h);
	
	dstPts = srcPts;
}

void Homography::reset()
{
	dstPts = srcPts;
}

bool Homography::compute()
{
	Vec2 tmpSrc[] = {srcPts[0]-srcPts[0],
					 srcPts[1]-srcPts[0],
					 srcPts[2]-srcPts[0],
					 srcPts[3]-srcPts[0] };

	Vec2 tmpDst[] = {dstPts[0]-srcPts[0],
					 dstPts[1]-srcPts[0],
					 dstPts[2]-srcPts[0],
					 dstPts[3]-srcPts[0] };
					 
	matrix = computeHomography(tmpSrc,tmpDst); //&srcPts[0],&dstPts[0]);
	matrix.preTranslate(srcPts[0].x,srcPts[0].y,0);
	matrix.translate(-srcPts[0].x,-srcPts[0].y,0);
	
//	matrix.translate(srcPts[0].x,srcPts[0].y,0);
	
	// TODO test determinant for singular matrices here
	invMatrix = matrix.inverted();
	return true;
}

void Homography::transform( const M33 & m )
{
	for( int i = 0; i < 4; i++ )
	{
		srcPts[i] = mul(srcPts[i],m);
		dstPts[i] = mul(dstPts[i],m);
	}
	
	compute();
}

void Homography::setSrcRect( const Rectf & r )
{
	srcPts[0](r.l,r.t);
	srcPts[1](r.r,r.t);
	srcPts[2](r.r,r.b);
	srcPts[3](r.l,r.b);
	for( int i = 0; i < 4; i++ )
	{
		dstPts[i] = transformPoint(srcPts[i]);
	}
	compute();
}
	
void Homography::updateWithSrcPoints( const std::vector<Vec2> & pts )
{
	srcPts = pts;
	for( int i = 0; i < 4; i++ )
		dstPts[i] = transformPoint(srcPts[i]);
}

bool Homography::isValid() const
{
    return isPolyConvex(&srcPts[0],4) && isPolyConvex(&dstPts[0],4);
}	

Vec2 Homography::untransformPoint( const Vec2 & p ) const
{
	Vec4 hp(p.x,p.y,0,1.0);
	hp = mul(hp,invMatrix);
	hp.x/=hp.w;
	hp.y/=hp.w;
	return Vec2(hp.x,hp.y);
}

Vec2 Homography::transformPoint( const Vec2 & p ) const
{
	Vec4 hp(p.x,p.y,0,1.0);
	hp = mul(hp,matrix);
	hp.x/=hp.w;
	hp.y/=hp.w;
	return Vec2(hp.x,hp.y);
}




}