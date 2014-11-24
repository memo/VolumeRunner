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

#include "colormotor.h"

namespace cm
{
	/////////////////////////////////////////////////
	
	class Bone
	{
	public:
		Bone() 
		{
			worldMatrix.identity();
			invBindPoseMatrix.identity();
			velocity(0,0,0);
			parent = 0;
		}
		
		~Bone()
		{
			
		}
		
		int			id;
		
		Bone*		parent;
		
		float		length;
		cm::Vec3		direction;
		cm::Vec3		parentDirection;
		float		parentLength;
		
		
		std::string name;
		cm::Transform	transform;
		
		cm::Vec3		offset;
		
		cm::Vec3		velocity;
		
		cm::M44			worldMatrix;
		cm::M44			invBindPoseMatrix;
		
		// hacky shit...
		M44		getBoneMatrix() 
		{
			M44 m;
			m.identity();
			
			if(!parent)
				return m;
				
			Quat parentdir;
			parentdir.direction(-parentDirection);
			M44 dirm = (M44)parentdir;
			
			dirm = parent->worldMatrix*dirm;
			return dirm;
		}
		
		Bone*		getChild( int i ) { return _children[i]; }
		void		addChild( Bone * b ) { _children.push_back(b); b->parent = this; }
		int			getNumChildren() const { return _children.size(); }
	protected:
	
		friend class Skeleton;
		
		
		
		std::vector <Bone*> _children;
	};
	
	/////////////////////////////////////////////////
	
	struct Pose
	{
		Pose( int nTransforms ) : nTransforms(nTransforms)
		{
			transforms = new cm::Transform[nTransforms];
		}
		
		~Pose()
		{
			SAFE_DELETE_ARRAY(transforms);
			nTransforms = 0;
		}
		
		void	copy( Pose * p )
		{
			for( int i = 0; i < nTransforms; i++ )
			{
				transforms[i] = p->transforms[i];
			}
		}
		
		const cm::Transform & getTransform( int index ) const { return transforms[index]; }
		
		cm::Transform * transforms;
		int			nTransforms;
	};
	
	void	slerp( Pose * out, Pose * a, Pose * b, float t );
	
	/////////////////////////////////////////////////
	
	// an animation can be connected to different bones..
	// animations are simply a number of repetitions of each 'Pose' of the skeleton for each keyframe
	

	/*
		s = new Skeleton();
		s->addBone( b, s->root );
		s->addBone( b2, b );
		s->finalize(); 
		
	*/
	
	
//	ld: duplicate symbol CM::distanceToSegment(CM::cm::Vec2 const&, CM::cm::Vec2 const&, CM::cm::Vec2 const&)in ../../cm/libs/colormotor/bin/libcolormotorD10_4.a(cmVector.o) and ../../cm/libs/colormotor/bin/libcolormotorD10_4.a(cmMathUtils.o)



	class Skeleton
	{
		public:
			Skeleton()
			{
				_finalized = false;
				matrixPalette = 0;
				pose = 0;
				bones.clear();
			}
			
			~Skeleton()
			{
				DELETE_VECTOR(bones);
				SAFE_DELETE_ARRAY(matrixPalette);
				SAFE_DELETE(pose);
			}
			
			int				getNumBones() const { return bones.size(); }
			Bone*			getBone( int i) const { return bones[i]; }
			int 			getBoneIndex( const std::string & name ) const;

			void			update();
			
			#ifdef DODEBUGRENDER
			void			debugDraw( DebugDraw * ddraw, float axisSize = 1.0f );
			#endif
			
			bool			setBones( Bone * rootBone );
			
			void			copyPose( Pose * pose );
			
			//void			addChild( Bone * b, Bone * parent );
			/// create 
			//bool			finalize();
			
			
	
			Bone*			root;
			std::vector<Bone*>	bones;
			
			Pose*			pose;
			
			cm::M44*			matrixPalette;
			
		protected:
			void			parseBone( Bone * b );
			
			void			update(const cm::M44 & parentMatrix, Bone * bone);
			bool			_finalized;
			
	};
}
