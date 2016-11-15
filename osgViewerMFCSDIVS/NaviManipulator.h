/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2010 Robert Osfield
*
* This library is open source and may be redistributed and/or modified under
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* OpenSceneGraph Public License for more details.
*/

#ifndef OSGGA_TERRAIN_MANIPULATOR
#define OSGGA_TERRAIN_MANIPULATOR 1

#include <osgGA/OrbitManipulator>

using namespace osg;
using namespace osgGA;


class CNaviManipulator : public OrbitManipulator
{
    typedef OrbitManipulator inherited;

public:

    bool naviMode;  // true->pan; false->orbit;

    CNaviManipulator(int flags = DEFAULT_SETTINGS);
    CNaviManipulator(const CNaviManipulator& tm,
                     const CopyOp& copyOp = CopyOp::SHALLOW_COPY);

    META_Object(osgGA, CNaviManipulator);

    enum RotationMode
    {
        ELEVATION_AZIM_ROLL,
        ELEVATION_AZIM
    };

    virtual void setRotationMode(RotationMode mode);
    RotationMode getRotationMode() const;

    virtual void setByMatrix(const Matrixd& matrix);

    virtual void setTransformation(const Vec3d& eye, const Vec3d& center, const Vec3d& up);

    virtual void setNode(Node* node);

protected:
    virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);
    virtual bool performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy);
    virtual bool performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy);

    bool intersect(const Vec3d& start, const Vec3d& end, Vec3d& intersection) const;
    void clampOrientation();

    Vec3d _previousUp;
};


#endif /* OSGGA_TERRAIN_MANIPULATOR */
