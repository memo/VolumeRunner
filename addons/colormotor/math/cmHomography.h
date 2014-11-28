///////////////////////////////////////////////////////////////////////////
//	 _________  __   ____  ___  __  _______  __________  ___
//	/ ___/ __ \/ /  / __ \/ _ \/  |/  / __ \/_  __/ __ \/ _ \
// / /__/ /_/ / /__/ /_/ / , _/ /|_/ / /_/ / / / / /_/ / , _/
// \___/\____/____/\____/_/|_/_/  /_/\____/ /_/  \____/_/|_|alpha.
//
//  Daniel Berio
//	http://www.enist.org/
//
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "math/cmVector.h"
#include "math/cmRect.h"

namespace cm
{

/// compute homography matrix 
/// Note this is a perspective matrix, so we need to use homogenous coordinates and perform perspective division to
/// transform a point.
M44 computeHomography(const std::vector<Vec2> &src, const std::vector<Vec2> &dst);
M44 computeHomography(const Vec2 *src, const Vec2 *dst);

/// Utility class for defining a homography. The homography is defined by providing two sets of 4 points ( srcPts and dstPts )
class Homography
{
public:
	Homography();
	
	/// compute the homography matrix ( and inverse )
	bool compute();

	/// Apply transform to both src and dst rects
	void transform( const M33 & m );

	// set source rect and update dest with current homographu matrix...
	void setSrcRect( const Rectf & r );

	/// Sets source points and sets destination points by applying 
	/// homography transform to them
	void updateWithSrcPoints( const std::vector<Vec2> & pts );

	/// Reset homography with rect defined by parameter
	void reset( const Rectf& r ) { reset(r.l,r.t,r.getWidth(),r.getHeight()); }
	void reset(float x, float y, float w, float h);
	void reset();
	
	/// Returns true if this is a valid homography, false otherwise ( src or dst polygons are not convex )
	bool isValid() const;

	Vec2 untransformPoint( const Vec2 & p ) const;
	Vec2 transformPoint( const Vec2 & p ) const;
	
	M44 matrix;
	M44 invMatrix;

	std::vector<Vec2> srcPts;
	std::vector<Vec2> dstPts;
};


}