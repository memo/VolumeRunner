///////////////////////////////////////////////////////////////////////////
//	 _________  __   ____  ___  __  _______  __________  ___
//	/ ___/ __ \/ /  / __ \/ _ \/  |/  / __ \/_  __/ __ \/ _ \
// / /__/ /_/ / /__/ /_/ / , _/ /|_/ / /_/ / / / / /_/ / , _/
// \___/\____/____/\____/_/|_/_/  /_/\____/ /_/  \____/_/|_|alpha.
//
//  Daniel Berio 2008-2014
//	http://www.enist.org/
//
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Skeleton.h"
#include "SkeletonAnimSource.h"


namespace cm {


	//////////////////////////////////////////////////////

	enum BVH_CHANNEL
	{
		XPOS = 0,
		YPOS,
		ZPOS,
		ZROT,
		XROT,
		YROT,
		XSCALE,
		YSCALE,
		ZSCALE,
		CHANNEL_UNKNOWN
	};
		
		
	//////////////////////////////////////////////////////

	struct BVHNode
	{
		
			
		BVHNode()
		{
			endNode = false;
			bindPoseMatrix.identity();
            boneCorrupted = false;
		}
		
		~BVHNode()
		{
			for( int i = 0; i < children.size(); i++ )
				delete children[i];
			children.clear();
			channels.clear();
		}
		
		void	addChild( BVHNode * node )
		{
			children.push_back(node);
			node->parent = this;
		}
		
		int 	getNumChildren() const { return children.size(); }
		
		void	computeBindPoseMatrix( BVHNode * parent )
		{
			bindPoseMatrix.translation(offset);
			bindPoseMatrix *= parent->bindPoseMatrix;	
		}
		
		std::string 	name;
		cm::Vec3	offset;
		
		cm::Vec3	direction;
		cm::Vec3	parentDirection;
		float	parentLength;

		float	length;

        bool    boneCorrupted;
        
		cm::M44		bindPoseMatrix;
		BVHNode * parent;
		
		std::vector <cm::Transform> 	keys;
		std::vector <BVH_CHANNEL>	channels;
		std::vector <BVHNode*> 	children;
	
		bool	endNode;
	};
	
	
	//////////////////////////////////////////////////////


	class BVHParser
	{
		public:
			BVHParser();
			
			~BVHParser()
			{
				release();
			}
			
			void 	release();
			bool	parse( const char * path );
			
			Skeleton * 		createSkeleton();
			SkeletonKeyAnimSource * createAnimation();
			
			int		numFrames;
			float	frameTime;
			
			std::string 	fileName;
		protected:
			BVH_CHANNEL 	parseChannel( const std::string & str );
			BVHNode*		parseNode( Buffer & buf, BVHNode * parent );
			BVHNode*		parseEndSite( Buffer & buf, BVHNode * parent );
			
			bool			parseMotion( Buffer & buf );
			bool			parseMotionLine( Buffer & buf );
			bool			parseMotionChannel( BVHNode * node, Buffer & buf );
			
			Joint *			createJoint( BVHNode * node );
			
			BVHNode * 				_root;
			std::vector<BVHNode*>			_linearNodes;
	};
}