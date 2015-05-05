/*!
 * Node.h
 * Projekt 3DVisual
 */
#ifndef DATA_NODE_DEF
#define DATA_NODE_DEF 1

#include <osg/Vec3f>
#include <osg/AutoTransform>

#include <QMap>
#include <QString>

#include <Data/OsgNode.h>



//volovar_zac
namespace Layout {
class RadialLayout;
}
//volovar_kon

namespace Data {
class Edge;
class Type;
class Graph;
class GraphLayout;
class Cluster;



/**
	*  \class Node
	*  \brief Node object represents a single node in a Graph
	*  \author Aurel Paulovic, Michal Paprcka
	*  \date 29. 4. 2010
	*/
class Node : public OsgNode
{
public:

	/**
		*  \fn public constructor  Node(qlonglong id, QString name, Data::Type* type, Data::Graph* graph, osg::Vec3f position)
		*  \brief Creates new Node object
		*  \param  id     ID of the Node
		*  \param  name     name of the Node
		*  \param  type    Type of the Node
		*  \param  graph   Graph to which the Node belongs
		*  \param  position    Node position in space
		*/
    Node( qlonglong id, QString name, Data::Type* type, float scaling, Data::Graph* graph, osg::Vec3f position );

	/**
		*  \fn public virtual destructor  ~Node
		*  \brief Destroys the Node object
		*/
	~Node( void );

	/**
	    *  \fn inline public numberOfVertigoPlane
	    *  \brief Returns the value of the vertigo plane, which the node belongs to
	    *  \return qlonglong value of the vertigo plane
	    */
	qlonglong  numberOfVertigoPlane;

	/**
	    *  \fn inline public  setNumberOfVertigoPlane(qlonglong value);
	    *  \brief Sets the value of the vertigo plane, which the node belongs to
	    *  \param  qlonglong value of the vertigo plane
	    */
	void setNumberOfVertigoPlane( qlonglong value );

	/**
	    *  \fn inline public  getNumberOfVertigoPlane();
	    *  \brief Gets the value of the vertigo plane, which the node belongs to
	    *  \return  qlonglong value of the vertigo plane
	    */
	qlonglong getNumberOfVertigoPlane();

	/**
		 * \fn public constant isFocused
		 * \brief Gets whether this node is focused at the moment.
		 * \return whether this node is focused at the moment
		 */
	bool isFocused() const;

	/**
		 * \fn public setIsFocused(bool value)
		 * \brief Sets whether this node is focused at the moment.
		 * \param value whether this node is focused at the moment
		 */
	void setIsFocused( bool value );

	/**
		*	\fn public removeAllEdges
		*	\brief Removes all Edges which connect to the Node
		*
		*	It unlinks all edges connected to the Node and removes them from the Graph (an Edge can't exist without both it's Nodes)
		*/
	void removeAllEdges();

	/**
		*	\fn public getParentNode
		*	\brief Return parent node, else null
		*
		*
		*/
    Data::Node* getParentNode();

	/**
		*	\fn public setParentNode
		*	\brief set parent's node
		*
		*
		*/
    void setParentNode( Node* parent );

	/**
	    *	Node parent
	    *	\brief parent node of current node in nested graphs, in top level graph is null
	    */
	osg::ref_ptr<Data::Node> nested_parent;

	/**
	    *  \fn inline public  getNestedParent
	    *  \brief Return node parent, if no parent then return NULL
	        *  \return QString name of the Node
	    */
	osg::ref_ptr<Data::OsgNode> getNestedParent()
	{
		return nested_parent;
	}

	/**
		*  \fn inline public constant  getEdges
		*  \brief Returns all Edges connected to the Node
		*  \return QMap<qlonglong,osg::ref_ptr<Data::Edge> > * Edges connected to the Node
		*/
	QMap<qlonglong, osg::ref_ptr<Data::Edge> >* getEdges() const
	{
		return edges;
	}

	/**
		*  \fn inline public  setEdges(QMap<qlonglong, osg::ref_ptr<Data::Edge> > *val)
		*  \brief Sets (overrides) new Edges which are connected to the Node
		*  \param   val   new Edges
		*/
	void setEdges( QMap<qlonglong, osg::ref_ptr<Data::Edge> >* val )
	{
		edges = val;
	}

	/**
		*  \fn public  addEdge(osg::ref_ptr<Data::Edge> edge)
		*  \brief Adds new Edge to the Node
		*  \param    edge   new Edge
		*/
	void addEdge( osg::ref_ptr<Data::Edge> edge );

	void removeEdge( osg::ref_ptr<Data::Edge> edge );

	/**
		*  \fn inline public  setFixed(bool fixed)
		*  \brief Sets node fixed state
		*  \param     fixed     fixed state
		*/
	void setFixed( bool fixed )
	{
		this->fixed = fixed;
		if ( fixed && this->containsDrawable( square ) ) {
			this->setDrawableColor( 0, osg::Vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
		}
		else if ( !fixed && this->containsDrawable( square ) ) {
            this->setDrawableColor( 0, colorOfNode );
		}
	}

	/**
		*  \fn inline public constant  isFixed
		*  \brief Returns if the Node is fixed
		*  \return bool true, if the Node is fixed
		*/
	bool isFixed() const
	{
		return fixed;
	}

	/**
		*  \fn inline public constant cluster
		*  \brief Returns Cluster which contains this Node
		*  \return Cluster which contains this Node
		*/
	Cluster* getCluster() const;

	/**
		*  \fn inline public  setCluster(Cluster* cluster)
		*  \brief Sets cluster which contains this Node
		*  \param     cluster     cluster containing this node
		*/
	void setCluster( Cluster* cluster );

	virtual QSet<Node*> getIncidentNodes() const;

	/**
		 * \brief \link Node::positionCanBeRestricted [setter] \endlink
		 */
	void setPositionCanBeRestricted( bool b )
	{
		positionCanBeRestricted = b;
	}

	/**
		 * \brief \link Node::positionCanBeRestricted [getter] \endlink
		 */
	bool getPositionCanBeRestricted()
	{
		return positionCanBeRestricted;
	}

	/**
		*  \fn inline public constant  toString
		*  \brief Returns human-readable string representing the Node
		*  \return QString
		*/
	QString toString() const;

	/**
		*  \fn public  showLabel(bool visible)
		*  \brief If true, node name will be shown.
		*  \param     visible     node name shown
		*/
	void showLabel( bool visible );

    void setDefaultColor();

	/**
		*  \fn public  reloadConfig
		*  \brief Reloads node configuration
		*/
	void reloadConfig();

	/**
		*  \fn inline public  setNestedParent(Data::Node val)
		*  \brief Sets parent of note, if null, node has no parent
		*  \param      val
		*/
	void setNestedParent( osg::ref_ptr<Data::Node> val )
	{
		nested_parent = val;
	}

	//volovar_zac
	void setLayerID( int id ); //get unique layer id of selected radial layout, 0 node has no radial layout
	int getLayerID();
	Layout::RadialLayout* getRadialLayout(); //pointer to radial layout, NULL if node doesn't belong to radial layout
	void setRadialLayout( Layout::RadialLayout* rl );
	//volovar_kon
private:

    /**
		 * bool mIsFocused
		 * \brief Flag whether this node is focused at the moment.
		 */
	bool mIsFocused;

	/**
		*  QMap<qlonglong, osg::ref_ptr<Data::Edge> > * edges
		*  \brief Edges connected to the Node
		*/
	QMap<qlonglong, osg::ref_ptr<Data::Edge> >* edges;

	/**
		*  bool fixed
		*  \brief node fixed state
		*/
	bool fixed;

	/**
	    *  Cluster* cluster
	    *  \brief Cluster containing this node
	    */
	Cluster* cluster;

	/**
		 * \brief If the node position can be restricted by layout restrictions.
		 * RestrictionsManager uses this to determine if the restriction can be attached to this node.
		 */
	bool positionCanBeRestricted;

	//volovar_zac pre repulzivne sily, ktore posobia iba na uzly nachadzajuce sa na guli
	int layerID;
	Layout::RadialLayout* radialLayout;
	//volovak_kon
};
}

#endif
