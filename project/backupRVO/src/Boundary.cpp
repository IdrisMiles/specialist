#include "Boundary.h"
#include <ngl/ShaderLib.h>

Boundary::Boundary()
{

}

Boundary::Boundary(const ngl::Vec3 &_p0, const ngl::Vec3 &_p1,
                   const ngl::Vec3 &_p2, const ngl::Vec3 &_p3,
                   const bool &_drawFlag)
{
    m_points[0] = _p0;  //top left
    m_points[1] = _p1;  //top right
    m_points[2] = _p2;  //bottom right
    m_points[3] = _p3;  //bottom left
    m_isVAOinit = false;

    if(_drawFlag){initVAO();}
}

Boundary::~Boundary()
{

  m_vao->removeVOA();
}



ngl::Vec3 *Boundary::getBoundaryPoints()
{
    return m_points;
}

ngl::Vec3 Boundary::getBoundaryPoint(const int &_i) const
{
    if(_i<4)
    {
        return m_points[_i];
    }
}
//=========drawing stuff===========
void Boundary::initVAO()
{
  if(m_isVAOinit){return;}

  std::vector<ngl::Vec3> verts;

  // face side 1
  verts.push_back(m_points[0]);
  verts.push_back(m_points[0] + ngl::Vec3(0,2,0));
  verts.push_back(m_points[1]);
  verts.push_back(m_points[1] + ngl::Vec3(0,2,0));
  // face side 2
  verts.push_back(m_points[1]);
  verts.push_back(m_points[1] + ngl::Vec3(0,2,0));
  verts.push_back(m_points[2]);
  verts.push_back(m_points[2] + ngl::Vec3(0,2,0));
  // face side 3
  verts.push_back(m_points[2]);
  verts.push_back(m_points[2] + ngl::Vec3(0,2,0));
  verts.push_back(m_points[3]);
  verts.push_back(m_points[3] + ngl::Vec3(0,2,0));

  // face side 4
  verts.push_back(m_points[3]);
  verts.push_back(m_points[3] + ngl::Vec3(0,2,0));
  verts.push_back(m_points[0]);
  verts.push_back(m_points[0] + ngl::Vec3(0,2,0));
  // face 5 top
  /*verts.push_back(m_points[0]);
  verts.push_back(m_points[0] + ngl::Vec3(0,2,0));
  verts.push_back(m_points[1]);
  verts.push_back(m_points[1] + ngl::Vec3(0,2,0));
  // face 6 botom
  verts.push_back(m_points[0]);
  verts.push_back(m_points[0] + ngl::Vec3(0,2,0));
  verts.push_back(m_points[1]);
  verts.push_back(m_points[1] + ngl::Vec3(0,2,0));*/


  m_vao = ngl::VertexArrayObject::createVOA(GL_TRIANGLE_STRIP);
  m_vao->bind();
  m_vao->setData(verts.size()*sizeof(ngl::Vec3),verts[0].m_x);
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vao->setNumIndices(verts.size());
  m_vao->unbind();

  m_isVAOinit = true;
}

void Boundary::loadMatricesToShader(ngl::Mat4 &_globalTX,ngl::Camera &_cam)
{
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  shader->use("Wall");

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;

  M   = _globalTX;
  MV  = M*_cam.getViewMatrix();
  MVP = M*_cam.getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();

  shader->setShaderParamFromMat4("MVP",MVP);

}

void Boundary::updateVAO()
{
  if(!m_isVAOinit){return;}

}

void Boundary::draw()
{
  if(!m_isVAOinit){return;}

  std::vector<ngl::Vec3> verts;
  // face side 1
  verts.push_back(m_points[0]);
  verts.push_back(m_points[0] + ngl::Vec3(0,2,0));
  verts.push_back(m_points[1]);
  verts.push_back(m_points[1] + ngl::Vec3(0,2,0));
  // face side 2
  verts.push_back(m_points[1]);
  verts.push_back(m_points[1] + ngl::Vec3(0,2,0));
  verts.push_back(m_points[2]);
  verts.push_back(m_points[2] + ngl::Vec3(0,2,0));
  // face side 3
  verts.push_back(m_points[2]);
  verts.push_back(m_points[2] + ngl::Vec3(0,2,0));
  verts.push_back(m_points[3]);
  verts.push_back(m_points[3] + ngl::Vec3(0,2,0));

  // face side 4
  verts.push_back(m_points[3]);
  verts.push_back(m_points[3] + ngl::Vec3(0,2,0));
  verts.push_back(m_points[0]);
  verts.push_back(m_points[0] + ngl::Vec3(0,2,0));

  m_vao->bind();
  m_vao->updateData(verts.size()*sizeof(ngl::Vec3),verts[0].m_x);
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vao->draw();
  m_vao->unbind();

}


//=======hash table stuff==========
void Boundary::setHashID(const int &_id)
{
    m_hashTableID.push_back(_id);
}

void Boundary::setCellID(const int &_id)
{
    m_cellID = _id;
}

void Boundary::setCell(Cell *_cell)
{
    m_cell = _cell;
}


Cell *Boundary::getCell()
{
    return m_cell;
}

int Boundary::getCellID()
{
    return m_cellID;
}

std::vector<int> Boundary::getHashID()const
{
    return m_hashTableID;
}
