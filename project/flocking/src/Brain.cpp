#include "Brain.h"

#include <iostream>
#include <math.h>
#include <boost/foreach.hpp>
#include <ngl/NGLStream.h>

#include "Agent.h"

Brain::Brain(Agent *_agent, System *_system)
{
    // ctor that should be used
    m_agent = _agent;
    m_system = _system;

    m_perceiveRad = 1.0f;
    m_perceiveAng = 360.0f;
    m_desSpeed = 0.1;
}

Brain::~Brain()
{

}

void Brain::update()
{
    findNextGoal();

    switch (m_avoidanceType)
    {
        case FLOCKING:
            flocking();break;
        case RVO:
            rvo();break;
        case SOCIAL:
            socialForces();break;
        default:
            break;
    }

    // collision response goes here

    //std::cout<<"num neighbours "<<m_neighbours.size()<<"\n";

}

void Brain::setSystem(System *_system)
{
    m_system = _system;
}

void Brain::setAvoidanceType(const Avoidance &_avoidance)
{
    m_avoidanceType = _avoidance;
}

void Brain::setGoal(const ngl::Vec3 &_goal)
{
    m_goal = _goal;
}

void Brain::mapRoute()
{

}

void Brain::findNextGoal()
{
    //m_goal = ?
}

void Brain::findNeighbours()
{

}

void Brain::findBoundaries()
{

}

//===============RVO==================
void Brain::rvo()
{
    //std::cout<<"RVO avoidance in use\n"<<std::endl;
}

//=============flocking===============
void Brain::flocking()
{
    float goalWeight = 0.1f;
    float alignmentWeight = 0.1f;
    float cohesionWeight = 0.1f;
    float separationWeight = 0.1f;
    //---------------goal rule----------------------
    ngl::Vec3 goal = m_goal - m_agent->getOrigState().m_pos;
    if(goal != ngl::Vec3(0.0f,0.0f,0.0f))
    {
        goal.normalize();
        goal *= goalWeight;
    }
    else
    {
        m_agent->setForce(goal);
        return;
    }
    //----------------------------------------------

    //-------------check for neigbours--------------
    if(m_neighbours.size() <1)
    {
        // no neightbours, only goal force will apply
        goal.normalize();
        m_agent->setForce(goal);
        return;
    }
    //----------------------------------------------

    //------------separation rule-------------------
    ngl::Vec3 separation;
    BOOST_FOREACH(boost::shared_ptr<Agent> n, m_neighbours)
    {
        ngl::Vec3 tmpSeparation;
        // separation from neighbouring boids
        ngl::Vec3 distV = m_agent->getOrigState().m_pos - n->getOrigState().m_pos;
        float distF = (m_agent->getOrigState().m_pos - n->getOrigState().m_pos).length() -
                     (m_agent->getOrigState().m_rad + n->getOrigState().m_rad);

        if(distF < m_perceiveRad)
        {
            tmpSeparation -= (n->getOrigState().m_pos - m_agent->getOrigState().m_pos);
            tmpSeparation *= (m_perceiveRad/distF);

            if(distF <= m_agent->getOrigState().m_rad)
            {
                //m_agent->setVel(ngl::Vec3(0.0f,0.0f,0.0f));
                ngl::Vec3 tmpVel = m_agent->getOrigState().m_vel;
                distV.normalize();
                distV * m_agent->getOrigState().m_rad;
                m_agent->setVel(tmpVel+0.5*distV);
                //std::cout<<"TOO CLOSE \n";
            }
        }
        separation += tmpSeparation;
    }
    BOOST_FOREACH(Boundary *b,m_Boundaries)
    {
        // find closest perpendicular point to boundary
        // use this distance for repulsion
        ngl::Vec3 tmpSeparation;
        // separation from neighbouring boids
        ngl::Vec3 bPoints1 = b->getBoundaryPoint(0);
        ngl::Vec3 bPoints2 = b->getBoundaryPoint(1);
        ngl::Vec3 bEdge1 = bPoints1 - bPoints2;
        ngl::Vec3 perpEdge = ngl::Vec3(-bEdge1.m_z,0,bEdge1.m_x);
        //ngl::Vec3 edge2 = m_agent->getOrigState().m_pos + perpEdge;

        // To find perpendicular point:
        // bounddary p+tr = q+us agent
        // t = (q − p) × s / (r × s)
        // if 0<t<1  point exists
        float t = (m_agent->getCurrentState().m_pos - bPoints2).cross(perpEdge).m_y /
                  (bEdge1.cross(perpEdge).m_y);
        // nearest point on boundary
        ngl::Vec3 iPoint = bPoints2 + (t*bEdge1);

        ngl::Vec3 distV = m_agent->getOrigState().m_pos - iPoint;

        float distF = (m_agent->getOrigState().m_pos - iPoint).length() -
                     (m_agent->getOrigState().m_rad);

        if(distF < m_perceiveRad)
        {
            tmpSeparation -= (iPoint - m_agent->getOrigState().m_pos);
            tmpSeparation *= (m_perceiveRad/distF);

            if(distF <= m_agent->getOrigState().m_rad)
            {
                //m_agent->setVel(ngl::Vec3(0.0f,0.0f,0.0f));
                ngl::Vec3 tmpVel = m_agent->getOrigState().m_vel;
                distV.normalize();
                distV * m_agent->getOrigState().m_rad;
                m_agent->setVel(tmpVel+1*distV);
                //std::cout<<"TOO CLOSE \n";
            }
        }
        separation += tmpSeparation;

    }

    if(separation != ngl::Vec3(0.0f,0.0f,0.0f))
    {
        separation.normalize();
        separation *= separationWeight;
    }

    //----------------------------------------------

    //------------Alignment rule--------------------
    ngl::Vec3 alignment;
    BOOST_FOREACH(boost::shared_ptr<Agent> n, m_neighbours)
    {
        alignment += n->getOrigState().m_vel;
    }
    if(alignment != ngl::Vec3(0.0f,0.0f,0.0f))
    {
        alignment.normalize();
        //alignment /= m_neighbours.size();
        alignment*= alignmentWeight;
    }
    //----------------------------------------------

    //----------Cohesion rule----------------------
    ngl::Vec3 cohesion = ngl::Vec3(0.0f,0.0f,0.0f);
    BOOST_FOREACH(boost::shared_ptr<Agent> n, m_neighbours)
    {
        cohesion += n->getOrigState().m_pos - m_agent->getOrigState().m_pos;
    }
    cohesion /= m_neighbours.size();
    if(cohesion!= ngl::Vec3(0.0f,0.0f,0.0f))
    {
        //cohesion - m_agent->getOrigState().m_pos;
        cohesion.normalize();
        cohesion *= cohesionWeight;
    }
    //----------------------------------------------

    //-------------Final force----------------------
    ngl::Vec3 finalForce = (separation + alignment + cohesion + goal);
    //finalForce.normalize();
    if(finalForce != ngl::Vec3(0.0f,0.0f,0.0f))
    {
        finalForce.normalize();
    }

    m_agent->setForce(0.01*finalForce);
    //m_agent->setVel(0.01*finalForce);
    //----------------------------------------------
}

//----------------Social forces-----------------------
void Brain::socialForces()
{
    //std::cout<<"social forces avoidance in use\n"<<std::endl;
}

//-----------------------------------------------------
float Brain::getPerceiveRad()const
{
    return m_perceiveRad;
}

float Brain::getPerceiveAng()const
{
    return m_perceiveAng;
}

float Brain::getDesSpeed()const
{
    return m_desSpeed;
}


void Brain::clearNeighbours()
{
    for(unsigned int i=0;i<m_neighbours.size();i++)
    {
        //delete [] m_neighbours[i];
        //m_neighbours.pop_back();
    }
    m_neighbours.clear();
    //m_neighbours.erase(m_neighbours.begin(),m_neighbours.end());
}

void Brain::clearBoundary()
{
    m_neighbours.clear();
}

void Brain::addNeighbour( boost::shared_ptr<Agent> _neighbour)
{
    m_neighbours.push_back(_neighbour);
}

void Brain::addBoundary(Boundary *_boundary)
{
    m_Boundaries.push_back(_boundary);
}
