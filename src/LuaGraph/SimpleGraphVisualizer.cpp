#include "LuaGraph/SimpleGraphVisualizer.h"
#include "LuaGraph/HyperGraphVisualizer.h"
#include "LuaGraph/LuaGraphVisualizer.h"
#include "LuaGraph/LuaGraph.h"
#include "Importer/GraphOperations.h"
#include "limits"

Lua::SimpleGraphVisualizer::SimpleGraphVisualizer(Data::Graph *graph, osg::ref_ptr<osg::Camera> camera)
    : Lua::LuaGraphVisualizer(graph, camera){}

void Lua::SimpleGraphVisualizer::visualize()
{
    Lua::LuaGraph *g = Lua::LuaGraph::loadGraph();
    g->printGraph();

    for (QMap<qlonglong, Lua::LuaNode *>::iterator i = g->getNodes()->begin(); i != g->getNodes()->end(); ++i){
        Data::Type *newNodeType = currentGraph->addType(i.value()->getLabel());
        newNodeType->getSettings()->insert("color.R", QString::number(i.value()->getFloatParam("colorR", 1)));
        newNodeType->getSettings()->insert("color.G", QString::number(i.value()->getFloatParam("colorG", 1)));
        newNodeType->getSettings()->insert("color.B", QString::number(i.value()->getFloatParam("colorB", 1)));
        newNodeType->getSettings()->insert("color.A", QString::number(i.value()->getFloatParam("colorA", 1)));
        float scale = i.value()->getFloatParam("size", 8);
        osg::ref_ptr<Data::Node> n = currentGraph->addNode(i.key() , i.value()->getLabel(), newNodeType);
        n->setScale(scale);
//        setNodeParams(n, i.value(), osg::Vec4f(1,1,1,1), 8);
    }

    for (QMap<qlonglong, Lua::LuaEdge *>::iterator i = g->getEdges()->begin(); i != g->getEdges()->end(); ++i){
        if (i.value()->getIncidences().size() != 2) throw new std::runtime_error("Not a simple graph");
        Data::Type *newEdgeType = currentGraph->addType(i.value()->getLabel());
        newEdgeType->getSettings()->insert("color.R", QString::number(i.value()->getFloatParam("colorR", 1)));
        newEdgeType->getSettings()->insert("color.G", QString::number(i.value()->getFloatParam("colorG", 1)));
        newEdgeType->getSettings()->insert("color.B", QString::number(i.value()->getFloatParam("colorB", 1)));
        newEdgeType->getSettings()->insert("color.A", QString::number(i.value()->getFloatParam("colorA", 1)));
        LuaIncidence * const incid1 = g->getIncidences()->value(i.value()->getIncidences()[0]);
        LuaIncidence * const incid2 = g->getIncidences()->value(i.value()->getIncidences()[1]);
        osg::ref_ptr<Data::Node> srcNode = currentGraph->getNodes()->value(incid1->getEdgeNodePair().second);
        osg::ref_ptr<Data::Node> dstNode = currentGraph->getNodes()->value(incid2->getEdgeNodePair().second);
        osg::ref_ptr<Data::Edge> newEdge;
        if (incid1->getOriented()){
            if (incid1->getOutGoing()){
                newEdge = currentGraph->addEdge(i.key(), i.value()->getLabel(), dstNode, srcNode, newEdgeType, true);
            } else {
                newEdge = currentGraph->addEdge(i.key(), i.value()->getLabel(), srcNode, dstNode, newEdgeType, true);
            }
//            newEdge->setEdgeColor(osg::Vec4(1,0,0,1));
        } else {
            newEdge = currentGraph->addEdge(i.key(), i.value()->getLabel(), srcNode, dstNode, newEdgeType, false);
//            newEdge->setEdgeColor(osg::Vec4(0,1,0,1));
        }
        newEdge->setCamera(camera);
        newEdge->setEdgeStrength(i.value()->getFloatParam("edgeStrength", 1));
        if (i.value()->getParams().type() != 0 && i.value()->getParams()["visible"] == false)
            newEdge->setScale(0);
    }
    g->setObserver(this);

    QString metaNodeName = "metaNode";
    QString metaEdgeName = "metaEdge";
    osg::ref_ptr<Data::Node> filesAnchor = currentGraph->addNode(std::numeric_limits<qlonglong>::max(), metaNodeName, currentGraph->getNodeMetaType(), osg::Vec3(0, 0, 500));
    osg::ref_ptr<Data::Node> functionsAnchor = currentGraph->addNode(std::numeric_limits<qlonglong>::max() - 1, metaNodeName, currentGraph->getNodeMetaType(), osg::Vec3(0, 0, -500));
    filesAnchor->setColor(osg::Vec4(0,0,0,0));
    functionsAnchor->setColor(osg::Vec4(0,0,0,0));

    for (QMap<qlonglong, Lua::LuaNode *>::iterator i = g->getNodes()->begin(); i != g->getNodes()->end(); ++i){
        std::cout << i.value()->getParams().type() << i.value()->getParams().typeName() << std::endl;
        if (i.value()->getParams().type() == 0) continue;
        if (i.value()->getParams()["root"]== true){
            osg::ref_ptr<Data::Node> root = currentGraph->getNodes()->value(i.key());
            osg::ref_ptr<Data::Edge> metaLink = currentGraph->addEdge(metaEdgeName, root, filesAnchor, currentGraph->getEdgeMetaType(), false);
            metaLink->setEdgeColor(osg::Vec4(0,0,0,0));
            metaLink->setInvisible(true);
        }
        if (i.value()->getParams()["type"] == "function"){
            osg::ref_ptr<Data::Node> func = currentGraph->getNodes()->value(i.key());
            osg::ref_ptr<Data::Edge> metaLink = currentGraph->addEdge(metaEdgeName, func, functionsAnchor, currentGraph->getEdgeMetaType(), false);
            metaLink->setEdgeColor(osg::Vec4(0,0,0,0));
            metaLink->setInvisible(true);
            metaLink->setEdgeStrength(0.1f);
        }
    }
}

void Lua::SimpleGraphVisualizer::onUpdate()
{
    std::cout << "SimpleGraph update called" << std::endl;
    Lua::LuaGraph *g = Lua::LuaGraph::loadGraph();
    for (QMap<qlonglong, Lua::LuaNode *>::iterator i = g->getNodes()->begin(); i != g->getNodes()->end(); ++i){
        osg::ref_ptr<Data::Node> n = currentGraph->getNodes()->value(i.key());
        setNodeParams(n, i.value(), osg::Vec4f(1,1,1,1), 8);
    }
    for (QMap<qlonglong, Lua::LuaEdge *>::iterator i = g->getEdges()->begin(); i != g->getEdges()->end(); ++i){
        osg::ref_ptr<Data::Edge> n = currentGraph->getEdges()->value(i.key());
        if (i.value()->getParams()["visible"] == false)
            n->setScale(0);
        else
            n->setScale(1);
    }
}



