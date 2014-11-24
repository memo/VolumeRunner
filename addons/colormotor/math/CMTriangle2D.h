/*
 *  CMTriangle2D.h
 *  colormotor
 *
 *  Created by Daniel Berio on 12/29/10.
 *  Copyright 2010 http://www.enist.org. All rights reserved.
 *
 */

#pragma once

template <class PT, class RT> class Triangle2D
{
public:
	PT a;
	PT b;
	PT c;
	
	Triangle2D() {}
	Triangle2D( const PT & a, const PT & b, const PT & c )
	:
	a(a),b(b),c(c)
	{
	}
	
	bool isPointIn( const PT & p ) const;
	RT area();
	PT centroid() const;
	PT orthocenter();
	PT circumcenter;
	
};