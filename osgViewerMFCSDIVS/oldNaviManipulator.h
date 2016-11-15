#include "stdafx.h"  
#include <osgGA/TerrainManipulator>  

using namespace osg;
using namespace osgGA;

//定义操作器  
class CNaviManipulator : public TerrainManipulator
{
public:
	bool naviMode;  // true->pan; false->orbit;

	// 鼠标左右键状态  
	bool m_bLeftButtonDown;
	bool m_bRightButtonDown;
	// 鼠标位置  
	float m_fpushX;
	float m_fpushY;
	
	CNaviManipulator();
	~CNaviManipulator();

	virtual bool handle(const GUIEventAdapter& ea, GUIActionAdapter& us);

	osg::Vec3d _oldcenter;
};