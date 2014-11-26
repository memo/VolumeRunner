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
	
	class Joint
	{
	public:
		Joint()
		{
			worldMatrix.identity();
			invBindPoseMatrix.identity();
			velocity(0,0,0);
			parent = 0;
		}
		
		~Joint()
		{
			
		}
		
		int			id;
		
		Joint*		parent;
		std::string name;
		cm::Transform	transform;
		
		cm::Vec3		offset;
		
		cm::Vec3		velocity;
		
		cm::M44			worldMatrix;
		cm::M44			invBindPoseMatrix;
        cm::M44         bindPoseMatrix;
        
        const cm::Vec3 & getRestPosition() const { return bindPoseMatrix.trans(); }
        const cm::Vec3 & getPosition() const { return worldMatrix.trans(); }
    
        /*
		// hacky shit...
		M44		getJointMatrix() 
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
		*/
        
		Joint*		getChild( int i ) { return _children[i]; }
		void		addChild( Joint * b ) { _children.push_back(b); b->parent = this; }
		int			getNumChildren() const { return _children.size(); }
	protected:
	
		friend class Skeleton;
		
		
		
		std::vector <Joint*> _children;
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
		
        void identity()
        {
            for( int i = 0; i < nTransforms; i++ )
                transforms[i].identity();
        }
        
		const cm::Transform & getTransform( int index ) const { return transforms[index]; }
		
		cm::Transform * transforms;
		int			nTransforms;
	};
	
	void	slerp( Pose * out, Pose * a, Pose * b, float t );

	class Skeleton
	{
		public:
			Skeleton()
			{
				_finalized = false;
				matrixPalette = 0;
				pose = 0;
				joints.clear();
			}
			
			~Skeleton()
			{
				DELETE_VECTOR(joints);
				SAFE_DELETE_ARRAY(matrixPalette);
				SAFE_DELETE(pose);
			}
			
            void            init();
        
			int				getNumJoints() const { return joints.size(); }
			Joint*			getJoint( int i) const { return joints[i]; }
			int 			getJointIndex( const std::string & name ) const;
        
			void			update();
			
			bool			setJoints( Joint * rootJoint );
			void			copyPose( Pose * pose );
        
			Joint*			root;
			std::vector<Joint*>	joints;
			
			Pose*			pose;
			
			cm::M44*			matrixPalette;
			
		protected:
			void			parseJoint( Joint * b );
			
			void			update(const cm::M44 & parentMatrix, Joint * joint);
            void            initTPose( Joint * joint, const M44 & parentMatrix );
        
			bool			_finalized;
			
	};
}
