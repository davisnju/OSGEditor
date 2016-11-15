#include "stdafx.h"
#include "EventHandler.h"

#include <osgParticle/FireEffect>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>

bool CEventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    // 获取鼠标位置  
    float mouseX = ea.getX();
    float mouseY = ea.getY();
    osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
    switch (ea.getEventType())
    {
    case  osgGA::GUIEventAdapter::KEYDOWN:
    {
        
    }

    case osgGA::GUIEventAdapter::PUSH:
        if (ea.getButton() == 1)
        { // 1 表示按鼠标左键
            m_bLeftButtonDown = true;
            // AfxMessageBox(msg.GetBuffer());
            // AfxMessageBox(_T("leftclick"));
            m_fpushX = mouseX;
            m_fpushY = mouseY;

            if (mModelRotate && !mModelRotating)
            {
                return rotateSelected(viewer, ea);
            }
            else if (mModeltrans && !mModeltransfering)
            {
                return transferSelected(viewer, ea);
            }

        }

        return false;
        // 鼠标释放  
    case(osgGA::GUIEventAdapter::RELEASE) :
        if (ea.getButton() == 1)
        {
            m_bLeftButtonDown = false;
            if (addFireValid) return addFire(viewer, ea);
        }
        if (ea.getButton() == 4)  //4 表示按鼠标右键  
        {
            // AfxMessageBox(msg.GetBuffer());
            // AfxMessageBox(_T("rightclick"));
            if (addModelValid) return addModel(viewer, ea);
        }
    }
    return false;
}

bool CEventHandler::addFire(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea)
{
    if (!addFireValid)
        return false;

    Group* root = dynamic_cast<Group*>(viewer->getSceneData());
    if (!root) return false;

    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (viewer->computeIntersections(ea, intersections))
    {
        const osgUtil::LineSegmentIntersector::Intersection& hit = *intersections.begin();

        bool handleMovingModels = false;
        const NodePath& nodePath = hit.nodePath;
        for (NodePath::const_iterator nitr = nodePath.begin();
             nitr != nodePath.end();
             ++nitr)
        {
            std::cout << "NodePath Node " << (*nitr)->getName() << std::endl;
            const MatrixTransform* cube = dynamic_cast<const MatrixTransform*>(*nitr);
            if (cube)
            {
                if (cube->getName() == "Cube") handleMovingModels = true;
            }
        }

        Vec3 positionfire = handleMovingModels ? hit.getLocalIntersectPoint() : hit.getWorldIntersectPoint();
        float scale = 1.0f * ((float)rand() / (float)RAND_MAX);
        if (m_firescale > 0.0f)
        {
            scale = m_firescale;
        }
        float intensity = 1.0f;
        osgParticle::FireEffect* fire = new osgParticle::FireEffect(positionfire, scale, intensity);
        Vec3 wind(0.0f, 0.0f, -1.0f);
        fire->setWind(wind);
        Group* effectsGroup = new Group;
        effectsGroup->addChild(fire);
        effectsGroup->setName("Fire");

        if (handleMovingModels)
        {
            fire->setUseLocalParticleSystem(false);

            ref_ptr<Node> hitNode = hit.nodePath.back();
            Node::ParentList parents = hitNode->getParents();
            Group* insertGroup = 0;
            unsigned int numGroupsFound = 0;
            for (Node::ParentList::iterator itr = parents.begin();
                 itr != parents.end();
                 ++itr)
            {
                Group* parent = (*itr);
                if (typeid(*parent) == typeid(Group))
                {
                    ++numGroupsFound;
                    insertGroup = parent;
                }
            }
            if (numGroupsFound == parents.size() && numGroupsFound == 1 && insertGroup)
            {
                // just reuse the existing group.
                insertGroup->addChild(effectsGroup);
                std::cout << "PickHandler::pick(,) hit node's parent is a single Group so we can simple the insert the particle effects group here." << std::endl;
                std::cout << "insert at " << positionfire.x() << positionfire.y() << positionfire.z() << std::endl;
                std::cout << "Node Name " << insertGroup->getName() << std::endl;
            }
            else
            {
                insertGroup = new Group;
                for (Node::ParentList::iterator itr = parents.begin();
                     itr != parents.end();
                     ++itr)
                {
                    (*itr)->replaceChild(hit.nodePath.back(), insertGroup);
                }
                insertGroup->addChild(hitNode.get());
                insertGroup->addChild(effectsGroup);
                std::cout << "PickHandler::pick(,) hit node's parent is a multiple Group so we can insert the particle effects group to last parent." << std::endl;
                std::cout << "insert at " << positionfire.x() << positionfire.y() << positionfire.z() << std::endl;
                std::cout << "Node Name " << insertGroup->getParent(0)->getName() << std::endl;
            }

            Geode* geode = new Geode;
            geode->addDrawable(fire->getParticleSystem());

            root->addChild(geode);

            return true;
        }
        else
        {

        }
    }
    return false;
}

bool CEventHandler::addModel(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea)
{
    if (!addModelValid)
        return false;

    CString modelstr(".osg");
    modelstr = m_modelname + modelstr;
    Group* root = dynamic_cast<Group*>(viewer->getSceneData());
    if (!root) return false;
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (viewer)
    {
        if (viewer->computeIntersections(ea, intersections))
        {
            const osgUtil::LineSegmentIntersector::Intersection& hit = *intersections.begin();
            Vec3 position = hit.getWorldIntersectPoint();
            Vec3 positionlocal = hit.getLocalIntersectPoint();
            CString cstr;
            cstr.Format(_T("world: %.1f, %.1f, %.1f\nlocal: %.1f, %.1f, %.1f\n")
                        , position.x(), position.y(), position.z()
                        , positionlocal.x(), positionlocal.y(), positionlocal.z());
            cstr += modelstr;

            bool handleMovingModels = false;
            const NodePath& nodePath = hit.nodePath;
            for (NodePath::const_iterator nitr = nodePath.begin();
                 nitr != nodePath.end();
                 ++nitr)
            {
                std::cout << "NodePath Node " << (*nitr)->getName() << std::endl;
                const MatrixTransform* cube = dynamic_cast<const MatrixTransform*>(*nitr);
                if (cube)
                {
                    if (cube->getName() == "BASE") handleMovingModels = true;
                }
            }

            if (handleMovingModels)
            {
                std::cout << "handleMovingModels" << std::endl;
            }
            else
            {
                std::cout << "base" << std::endl;
            }

            Node* newNode = osgDB::readNodeFile(CStringA(modelstr).GetBuffer(0));
            MatrixTransform* trans = new MatrixTransform;
            trans->setName("Cube");
            float r = newNode->getBound().radius();
            float z = newNode->getBound().center().z() - r;
            Vec3f positionAdj = position*Matrix::translate(Vec3f(0, 0, -z/2)); // adjust zxis position

            // Sphere
            ref_ptr<Geode> unitSphere = new Geode;
            ref_ptr<Sphere> sphere = new Sphere(newNode->getBound().center(), r);
            ref_ptr<ShapeDrawable> shapeDrawable = new ShapeDrawable(sphere.get());
            unitSphere->addDrawable(shapeDrawable.get());
            //ref_ptr<PositionAttitudeTransform> sphereForm = new PositionAttitudeTransform;
            //sphereForm->setPosition(Vec3(2.5, 0.0, 0.0));
            //sphereForm->addChild(unitSphere.get());

            cstr.Format(_T("%.1f, %.1f, %.1f\n%.1f, %.1f, %.1f\n"),
                        position.x(), position.y(), position.z()
                        , positionAdj.x(), positionAdj.y(), positionAdj.z()
                        );
            //AfxMessageBox(cstr.GetBuffer());

            trans->setMatrix(Matrix::scale(m_modelsize, m_modelsize, m_modelsize) // adjust scale
                             *Matrix::translate(positionAdj) // adjust position
                             );
            trans->addChild(newNode);
            //trans->addChild(unitSphere);
            Group* newGroup = new Group;
            newGroup->setName(CStringA(m_modelname).GetBuffer(0));
            newGroup->addChild(trans);
            newGroup->setName("Model");
            // m_Root->addChild(newNode.get());
            if (root)
            {

                // 添加模型到根节点
                root->addChild(newGroup);
                //root->addChild(newNode);
            }
            else
            {
                AfxMessageBox(_T("root is null"));
            }
        }
        else
        {
            AfxMessageBox(_T("no Intersections"));
        }
    }
    else
    {
        AfxMessageBox(_T("view is null"));
    }
    return false;
}

bool CEventHandler::rotateSelected(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter & ea)
{
    if (!mModelRotate)
        return false;

    Group* root = dynamic_cast<Group*>(viewer->getSceneData());
    if (!root) return false;

    Node* selectedModel = root;

    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (viewer->computeIntersections(ea, intersections))
    {
        const osgUtil::LineSegmentIntersector::Intersection& hit = *intersections.begin();

        bool handleMovingModels = false;
        const NodePath& nodePath = hit.nodePath;
        for (NodePath::const_iterator nitr = nodePath.begin();
             nitr != nodePath.end();
             ++nitr)
        {
            std::cout << "NodePath Node " << (*nitr)->getName() << std::endl;
            const Group* cube = dynamic_cast<const Group*>(*nitr);
            if (cube)
            {
                if (cube->getName() == "Model")
                {
                    handleMovingModels = true;
                    selectedModel = (*nitr);
                }
            }
        }
        
        //selection
        osgManipulator::Selection* selection = new osgManipulator::Selection;

        if (handleMovingModels)
        {
            ref_ptr<Node> hitNode = selectedModel; // hit.nodePath.back();
            Node::ParentList parents = hitNode->getParents();
            Group* insertGroup = 0;
            unsigned int numGroupsFound = 0;
            for (Node::ParentList::iterator itr = parents.begin();
                 itr != parents.end();
                 ++itr)
            {
                Group* parent = (*itr);
                if (typeid(*parent) == typeid(Group))
                {
                    ++numGroupsFound;
                    insertGroup = parent;
                }
            }
            if (numGroupsFound == parents.size() && numGroupsFound == 1 && insertGroup)
            {
                // 一般情况都是单父节点
                // AfxMessageBox(_T("single"));
                // 选当前节点selectedModel和其父节点insertGroup
                selection->addChild(selectedModel);//将需要操控的场景对象添加到Selection下  
                float scale = selectedModel->getBound().radius()*1.6;//Dragger的大小是依照物体的外包围球来计算，获取半径，然后稍微放大一点，在将这个值传给Dragger,
                ////设置TrackballDragger  
                osgManipulator::TrackballDragger* dragger = new osgManipulator::TrackballDragger();
                dragger->setupDefaultGeometry();
                dragger->setMatrix(Matrix::scale(scale*0.5, scale*0.5, scale*0.5)
                                   *Matrix::translate(selectedModel->getBound().center()));
                dragger->addTransformUpdating(selection);
                dragger->setHandleEvents(true);
                //设置了这个dragger的启动热键，当前为Ctrl，当按住Ctrl时，单击圆环，方能旋转。 
                dragger->setActivationModKeyMask(osgGA::GUIEventAdapter::MODKEY_CTRL);

                //CString cstr;
                //cstr.Format(_T("%d"), insertGroup->getChildIndex(selectedModel));//4
                //cstr.Format(_T("%d"), root->getChildIndex(selectedModel));//4
                //cstr = insertGroup->getName().c_str();
                //cstr = root->getName().c_str();
                //cstr = selectedModel->getName().c_str();

                if (root == insertGroup)
                {
                    root->addChild(selection);
                    root->addChild(dragger);
                    root->removeChild(
                        root->getChildIndex(selectedModel));
                    mModelRotating = true;
                }
                //insertGroup->addChild(selection);
                //insertGroup->addChild(dragger);
                //insertGroup->removeChild(
                //    insertGroup->getChildIndex(selectedModel));

                return true;
            }
            else
            {
                insertGroup = new Group;
                for (Node::ParentList::iterator itr = parents.begin();
                     itr != parents.end();
                     ++itr)
                {
                    (*itr)->replaceChild(hit.nodePath.back(), insertGroup);
                }
                insertGroup->addChild(hitNode.get());

                AfxMessageBox(_T("multiple"));
                // 选最后一个父节点insertGroup 
            }

            return true;
        }
        else
        {
            AfxMessageBox(_T("no model selected"));
        }
    }
    return false;
}

bool CEventHandler::transferSelected(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter & ea)
{
    if (!mModeltrans)
        return false;

    Group* root = dynamic_cast<Group*>(viewer->getSceneData());
    if (!root) return false;

    Node* selectedModel = root;

    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (viewer->computeIntersections(ea, intersections))
    {
        const osgUtil::LineSegmentIntersector::Intersection& hit = *intersections.begin();

        bool handleMovingModels = false;
        const NodePath& nodePath = hit.nodePath;
        for (NodePath::const_iterator nitr = nodePath.begin();
             nitr != nodePath.end();
             ++nitr)
        {
            std::cout << "NodePath Node " << (*nitr)->getName() << std::endl;
            const MatrixTransform* cube = dynamic_cast<const MatrixTransform*>(*nitr);
            if (cube)
            {
                if (cube->getName() == "Cube")
                {
                    handleMovingModels = true;
                    selectedModel = (*nitr);
                }
            }
        }
        
        //selection
        osgManipulator::Selection* selection = new osgManipulator::Selection;
        
        if (handleMovingModels)
        {
            ref_ptr<Node> hitNode = selectedModel;// hit.nodePath.back();
            Node::ParentList parents = hitNode->getParents();
            Group* insertGroup = 0;
            unsigned int numGroupsFound = 0;
            for (Node::ParentList::iterator itr = parents.begin();
                 itr != parents.end();
                 ++itr)
            {
                Group* parent = (*itr);
                if (typeid(*parent) == typeid(Group))
                {
                    ++numGroupsFound;
                    insertGroup = parent;
                }
            }
            if (numGroupsFound == parents.size() && numGroupsFound == 1 && insertGroup)
            {
                // just reuse the existing group.
                // AfxMessageBox(_T("single"));
                // 选当前节点selectedModel和其父节点insertGroup
                selection->addChild(selectedModel);//将需要操控的场景对象添加到Selection下  
                float scale = selectedModel->getBound().radius()*1.6;//Dragger的大小是依照物体的外包围球来计算，获取半径，然后稍微放大一点，在将这个值传给Dragger,
                ////设置TabBoxDragger
                osgManipulator::TabBoxDragger* tabDragger = new osgManipulator::TabBoxDragger();
                tabDragger->setupDefaultGeometry();
                tabDragger->setMatrix(Matrix::scale(scale*0.5, scale*0.5, scale*0.5)
                                   *Matrix::translate(selectedModel->getBound().center()));
                tabDragger->addTransformUpdating(selection);
                tabDragger->setHandleEvents(true);
                //设置了这个dragger的启动热键，当前为Ctrl，当按住Ctrl时，单击圆环，方能旋转。 
                tabDragger->setActivationModKeyMask(osgGA::GUIEventAdapter::MODKEY_CTRL);

                if (root == insertGroup)
                {
                    insertGroup->addChild(selection);
                    insertGroup->addChild(tabDragger);
                    insertGroup->removeChild(
                        insertGroup->getChildIndex(selectedModel));
                    mModeltransfering = true;
                }
            }
            else
            {
                insertGroup = new Group;
                for (Node::ParentList::iterator itr = parents.begin();
                     itr != parents.end();
                     ++itr)
                {
                    (*itr)->replaceChild(hit.nodePath.back(), insertGroup);
                }
                insertGroup->addChild(hitNode.get());

                AfxMessageBox(_T("multiple"));
                //insertGroup->addChild(effectsGroup);
            }

            return true;
        }
        else
        {
            AfxMessageBox(_T("no model selected"));
        }
    }
    return false;
}
