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

#include "BVHParser.h"

namespace cm
{

static int animCount = 0;

//////////////////////////////////////////////////////
BVHParser::BVHParser()
{
	_root = 0;
	
}
			
//////////////////////////////////////////////////////

void 	BVHParser::release()
{
	SAFE_DELETE(_root);
	_linearNodes.clear();
}

//////////////////////////////////////////////////////


bool	BVHParser::parse( const char * path )
{
	release();
	
	fileName = getFilenameFromPath(path);
	
	Buffer buf;
	CM_TEST( buf.fromFile(path) );
	
	std::string str;
	buf.getToken(str);
	
	// parse hierarchy
	
	if( str!="HIERARCHY" )
	{
		return false;
	}
	
	buf.getToken(str); // root node
	_root = parseNode(buf,0);
	
	if( _root == 0 )
		return false;
		
	TEST_TRUE( parseMotion( buf ) );
	
	return true;
}



//////////////////////////////////////////////////////


BVH_CHANNEL 	BVHParser::parseChannel( const std::string & str )
{
	std::string s = str;
	toUpper(s);
	
	if(s=="XROTATION")
	{
		return XROT;
	}
	
	if(s=="YROTATION")
	{
		return YROT;
	}
	
	if(s=="ZROTATION")
	{
		return ZROT;
	}
	
	if(s=="XPOSITION")
	{
		return XPOS;
	}
	
	if(s=="YPOSITION")
	{
		return YPOS;
	}
	
	if(s=="ZPOSITION")
	{
		return ZPOS;
	}
	
	return CHANNEL_UNKNOWN;
}



//////////////////////////////////////////////////////

BVHNode *BVHParser::parseEndSite( Buffer & buf, BVHNode * parent )
{
	BVHNode * node = new BVHNode();
	node->parent = parent;
	
	node->endNode = true;
	
	std::string str;
	
	buf.getToken(str);

	if( str != "{" )
	{
		debugPrint("Error didnt find opening bracket\n");
		return 0;
	}
	
	// parse offset
	buf.getToken(str);
	
	if(str!="OFFSET")
	{
		debugPrint("Error didnt find OFFSET\n");
		return 0;
	}
	
	buf.readLine(str);
	
	sscanf(str.c_str(),"%f %f %f",&node->offset.x,&node->offset.y,&node->offset.z);
	
	buf.getToken(str);
	if(str!="}")
	{
		debugPrint("Error didnt find CHANNELS\n");
		delete node;
		return 0;
	}
	
	return node;
}

BVHNode *BVHParser::parseNode( Buffer & buf, BVHNode * parent )
{
	BVHNode * node = new BVHNode();
	node->parent = parent;
	
	_linearNodes.push_back(node);
	
	buf.getToken(node->name);
	
	std::string str;
	
	buf.getToken(str);
	if( str != "{" )
	{
		debugPrint("Error didnt find opening bracket\n");
		delete node;
		return 0;
	}
	
	// parse offset
	buf.getToken(str);
	if(str!="OFFSET")
	{
		debugPrint("Error didnt find OFFSET\n");
		delete node;
		return 0;
	}
	
	buf.readLine(str);
	
	sscanf(str.c_str(),"%f %f %f",&node->offset.x,&node->offset.y,&node->offset.z);
	
	//buf.getToken(str);
	if(!buf.testToken("CHANNELS"))//, <#bool caseSensitive#>)str!="CHANNELS")
	{
		debugPrint("Error didnt find CHANNELS\n");
		delete node;
		return 0;
	}

	// parse channels 
	
	int nChannels = 0;
	
	buf.readInt(&nChannels);
	
	for( int i = 0; i < nChannels; i++ )
	{
		buf.getToken(str);
		BVH_CHANNEL chan = parseChannel(str);
		if(chan == CHANNEL_UNKNOWN) 
		{
			debugPrint("Error undefined channel %s\n",str.c_str());
			delete node;
			return 0;
		}
		node->channels.push_back(chan);
	}
	
	buf.getToken(str);
	
	// parse children ( if any... )
	while( str!="}" )
	{
		if( str=="JOINT" )
		{
			BVHNode * child = parseNode(buf,node);
			if(child==0)
			{
				debugPrint("Could not add child\n");
				delete node;
				return 0;
			}
			
			node->addChild(child);
			child->computeBindPoseMatrix(node);
			//buf.getToken(str);
		}
		else if( str=="End" )
		{
			buf.getToken(str); // skip site
			
			BVHNode * child = parseEndSite(buf,node);
			if(child==0)
			{
				debugPrint("Could not add child\n");
				delete node;
				return 0;
			}
			
			node->addChild(child);
			child->computeBindPoseMatrix(node);
		}
		
		buf.getToken(str);
	}
	
	// Hacky compute bone length...
	if( node->children.size() )
	{
		BVHNode * child = node->children[0];
		node->length = child->offset.length();
		node->direction( child->offset / node->length );
	}
	
	if( node->parent )
	{
		node->parentDirection = node->offset;
		node->parentLength = length(node->parentDirection);
        if(node->parentLength == 0)
        {
            // hack.
            node->parentDirection(0,0,1);
            node->parentLength = 1.0;
            node->boneCorrupted = true;
            //assert(false);
            //node->parentLength = 1.0;
        }
		node->parentDirection /= node->parentLength;//normalize();
        assert(!node->parentDirection.hasNans());

	}
	
	return node;
}

//////////////////////////////////////////////////////

bool			BVHParser::parseMotionChannel(  BVHNode * node, Buffer & buf )
{
	cm::M44 m;
	m.identity();
	cm::Vec3 pos(0,0,0);
	cm::Vec3 rot(0,0,0);
	cm::Quat qrot;
	qrot.identity();
	cm::Quat qaxis;
	
	for( int i = 0; i < node->channels.size(); i++ )
	{
		BVH_CHANNEL chan = node->channels[i];
		
		float v;
		if( buf.readFloat(&v) == -1 )
			return false;
		
		switch(chan)
		{
			case XROT:
				if(v<0)
					v+=360;
				if(v>360)
					v-=360;
					
				qaxis.axisAngle(cm::Vec3(1,0,0),radians(v));

				qrot = qrot*qaxis;
				rot.x = radians(v);
				m.rotateX(radians(v));
				break;
				
			case YROT:
				if(v<0)
					v+=360;
				if(v>360)
					v-=360;
					
				qaxis.axisAngle(cm::Vec3(0,1,0),radians(v));
                
				qrot = qrot*qaxis;
				rot.y = radians(v);
				m.rotateY(radians(v));
				break;
				
			case ZROT:
				if(v<0)
					v+=360;
				if(v>360)
					v-=360;
					
				qaxis.axisAngle(cm::Vec3(0,0,1),radians(v));
				qrot = qrot*qaxis;
				rot.z = radians(v);
				break;
				
			case XPOS:
				pos.x = v;
				break;
				
			case YPOS:
				pos.y = v;
				break;
				
			case ZPOS:
				pos.z = v;
				break;
				
			default:
				debugPrint("Found unknown channel can't parse\n");
				break;
		}
	}
	
	qrot.normalize();
	
	cm::Transform tm;
	tm.position = node->offset+pos;
	tm.rotation = qrot;
	
	node->keys.push_back(tm);
	
	return true;
}

//////////////////////////////////////////////////////

bool	BVHParser::parseMotionLine( Buffer & buf  )
{
	for( int i = 0; i < _linearNodes.size(); i++ )
	{
		BVHNode * node = _linearNodes[i];
		TEST_TRUE( parseMotionChannel(node,buf) );
	}
	
	return true;
}

//////////////////////////////////////////////////////

bool	BVHParser::parseMotion( Buffer & buf )
{
	std::string str;
	
	if(!buf.testToken("MOTION"))
		return false;
	
	buf.getToken(str); //Frames:
	buf.readInt(&numFrames);	
	
	buf.getToken(str);//Frame
	buf.getToken(str);//Time:
	
	buf.readFloat(&frameTime);
	
	buf.skipWhitespace();
	
	for( int i = 0; i < numFrames; i++ )
	{
		TEST_TRUE( parseMotionLine(buf) );
	}
    
    return true;
}


//////////////////////////////////////////////////////

Joint *	BVHParser::createJoint( BVHNode * node )
{
	Joint * b = new Joint();
	b->length = node->length;
	b->name = node->name;
	b->direction = node->direction;
	b->parentDirection = node->parentDirection;
	b->parentLength = node->parentLength;

    b->isJointOk = !node->boneCorrupted;

	b->offset = node->offset;
	b->invBindPoseMatrix = node->bindPoseMatrix.inverted();

    for( int i = 0; i < node->children.size(); i++ )
	{
		if(!node->children[i]->endNode)
			b->addChild( createJoint( node->children[i] ) );
	}
	
	return b;
}


//////////////////////////////////////////////////////


Skeleton * BVHParser::createSkeleton()
{
	Skeleton * s = new Skeleton();
	
	// set default pose...
	Joint * b = createJoint(_root);
	if( !s->setJoints(b) )
	{
		delete s;
		return 0;
	}
	
	Pose * pose = new Pose(s->getNumJoints());
	
	for(int i = 0; i < _linearNodes.size(); i++ )
	{
		BVHNode * n = _linearNodes[i];
		pose->transforms[i].rotation.identity();
		pose->transforms[i].position(0,0,0);
	}
	
	s->pose = pose;
	
	return s;
}


//////////////////////////////////////////////////////

SkeletonKeyAnimSource * BVHParser::createAnimation()
{
	std::string animName;
	stdPrintf(animName,"anim%d",animCount);
	animCount++;
	
	SkeletonKeyAnimSource * anim = new SkeletonKeyAnimSource(_linearNodes.size(),fileName.c_str());
	for( int j = 0; j < numFrames; j++ )
	{
		Pose * pose = new Pose(_linearNodes.size());
		
		for(int i = 0; i < _linearNodes.size(); i++ )
		{
			BVHNode * n = _linearNodes[i];
			pose->transforms[i] = n->keys[j];
		}
		
		anim->addKeyFrame(pose,frameTime*j);
	}
	
	return anim;
}

}