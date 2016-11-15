#include "stdafx.h"  
#include <osgGA/TerrainManipulator>  

using namespace osg;
using namespace osgGA;

//���������  
class CNaviManipulator : public TerrainManipulator
{
public:
	bool naviMode;  // true->pan; false->orbit;

	// ������Ҽ�״̬  
	bool m_bLeftButtonDown;
	bool m_bRightButtonDown;
	// ���λ��  
	float m_fpushX;
	float m_fpushY;
	
	CNaviManipulator();
	~CNaviManipulator();

	virtual bool handle(const GUIEventAdapter& ea, GUIActionAdapter& us);

	osg::Vec3d _oldcenter;
};