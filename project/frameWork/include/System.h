#ifndef _SYSTEM__H_
#define _SYSTEM__H_

#include <vector>
#include <ngl/Camera.h>
#include <ngl/Mat4.h>
#include "Agent.h"
#include "agentOctree.h"
#include "boundary4.h"

enum SpatialDivision {BRUTE,OCTREE,HASH};
class AgentOctree;
class HashTable;
class System
{
public:
    System(float _width);
    ~System();

    void update();
    void addAgent(const Avoidance &_avoidType);
    void addNeighbours();
    void addBoundaries();
    void clearNeighbours();
    void clearBoundaries();

    void setSpatialDivision(const SpatialDivision &_type);
    void setBounds(ngl::BBox _bounds);
    void addBounds(Boundary *_boundary);
    void addBounds4(Boundary4 * _boundary);

    void setGloablGoal(const ngl::Vec3 &_goal);
    void setRandomGoal();

    void loadMatricesToShader();
    void draw();
    void setUpDraw(const ngl::Camera &_cam, const ngl::Mat4 &_tx);
    ngl::Camera getCam()const;
    ngl::Mat4 getGlobalTX()const;
    float getSystemWidth()const;

    void updateHashTable();

    void setScene(const int &_scene);

    void printInfo()const;


private:

    std::vector< Agent* > m_agents;
    std::vector<Boundary*> m_Boundaries;
    std::vector<Boundary4*> m_Bounds4;
    int m_scene;

    SpatialDivision m_spatialDivision;
    AgentOctree *m_octree;
    HashTable *m_hashTable;

    ngl::BBox m_bounds;
    ngl::Vec3 m_groundPlane[4];
    ngl::Vec3 m_globalGoal;
    ngl::Camera m_cam;
    ngl::Mat4 m_globalTX;

    int m_numAgents;
    int m_numBoundaries;
    float m_systemWidth;

};


#endif //_SYSTEM__H_
