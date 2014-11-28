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