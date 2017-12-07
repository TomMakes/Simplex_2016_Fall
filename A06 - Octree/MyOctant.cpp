#include "MyOctant.h"
using namespace Simplex;
	int MyOctant::m_uOctantCount = 0;
	int MyOctant::m_uMaxLevel = 3;
	int MyOctant::m_uIdealEntityCount = 5;

	void Simplex::MyOctant::Release()
	{
		if (m_iLevel == 0)
		{
			KillBranches();
		}
		m_iChildren = 0;
		m_fSize = 0.0f;
		m_EntityList.clear();
		m_lChild.clear();
	}

	void MyOctant::Init()
	{
		m_iID = m_uOctantCount;
		m_iLevel = 0;
		m_iChildren = 0;
		m_fSize = 0.0f;

		m_pMeshMngr = MeshManager::GetInstance();
		m_pEntityMngr = MyEntityManager::GetInstance();

		m_v3Center = vector3(0.0f);
		m_v3Min = vector3(0.0f); 
		m_v3Max = vector3(0.0f);

		m_pRoot = nullptr;
		m_pParent = nullptr;
		for (int i = 0; i < 8; i++)
		{
			m_pChild[i] = nullptr;
		}
	}

	void Simplex::MyOctant::ConstructList()
	{
		for (int i = 0; i < m_iChildren; i++)
		{
			m_pChild[i]->ConstructList();
		}

		if (m_EntityList.size() > 0)
		{
			m_pRoot->m_lChild.push_back(this);
		}
	}

Simplex::MyOctant::MyOctant(uint maxLevel, uint idealEntityCount)
{
	Init();
	m_uMaxLevel = maxLevel;
	m_uIdealEntityCount = idealEntityCount;
	m_uOctantCount = 0;
	
	m_pRoot = this;
	m_lChild.clear();
	//Create a min max list to determine size of octree
	std::vector<vector3> minMaxList; 

	//Generate the min max list data to compare
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		MyEntity* entity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* rigidBody = entity->GetRigidBody();
		minMaxList.push_back(rigidBody->GetMinGlobal());
		minMaxList.push_back(rigidBody->GetMaxGlobal());
	}
	MyRigidBody* rigidBody = new MyRigidBody(minMaxList);

	vector3 halfWidth = rigidBody->GetHalfWidth();
	float maxSize = halfWidth.x;
	for (int i = 0; i < 3; i++)
	{
		if (maxSize < halfWidth[i])
		{
			maxSize = halfWidth[i];
		}
	}
	vector3 v3Center = rigidBody->GetCenterLocal();

	m_fSize = maxSize * 2.0f;
	m_v3Center = v3Center;
	m_v3Max = m_v3Center + (vector3(maxSize));
	m_v3Min = m_v3Center - (vector3(maxSize));

	//Add this octant to the total
	m_uOctantCount++;

	ConstructTree(m_uMaxLevel);

	SafeDelete(rigidBody);
}

Simplex::MyOctant::MyOctant(vector3 octCenter, float octSize)
{
	Init();
	m_v3Center = octCenter;
	m_fSize = octSize;

	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);
	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);

	m_uOctantCount++;
}

Simplex::MyOctant::MyOctant(MyOctant const& other)
{
	m_iChildren = other.m_iChildren;
	m_iID = other.m_iID;
	m_uIdealEntityCount = other.m_uIdealEntityCount;
	m_iLevel = other.m_iLevel;

	m_v3Center = other.m_v3Center;
	m_v3Max = other.m_v3Max;
	m_v3Min = other.m_v3Min;
	m_fSize = other.m_fSize;

	m_pParent = other.m_pParent;
	m_lChild = other.m_lChild;
	m_pRoot = other.m_pRoot;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	for (int i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}

MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	Release();
	Init();
	MyOctant swapme(other);
	Swap(swapme);
	return *this;
}


MyOctant::~MyOctant()
{
	Release();
}

void Simplex::MyOctant::Swap(MyOctant& other)
{
	std::swap(m_iID, other.m_iID);
	std::swap(m_iChildren, other.m_iChildren);
	std::swap(m_iLevel, other.m_iLevel);
	std::swap(m_pParent, other.m_pParent);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);
	std::swap(m_fSize, other.m_fSize);

	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
	std::swap(m_v3Center, other.m_v3Center);

	for (int i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}

float Simplex::MyOctant::GetSize()
{
	return m_fSize;
}

vector3 Simplex::MyOctant::GetCenterGlobal()
{
	return m_v3Center;
}

vector3 Simplex::MyOctant::GetMinGlobal()
{
	return m_v3Min;
}

vector3 Simplex::MyOctant::GetMaxGlobal()
{
	return m_v3Max;
}

bool Simplex::MyOctant::IsColliding(int rBIndex)
{
	//If it is an invalid request
	if (rBIndex >= m_pEntityMngr->GetEntityCount())
	{
		return false;
	}
	//Check if entity is within the octant with AABB
	MyEntity* entity = m_pEntityMngr->GetEntity(rBIndex);
	MyRigidBody* rigidBody = entity->GetRigidBody();
	vector3 min1 = rigidBody->GetMinGlobal();
	vector3 max1 = rigidBody->GetMaxGlobal();

	if (m_v3Max.x < min1.x)
	{
		return false;
	}
	if (m_v3Min.x > max1.x)
	{
		return false;
	}

	if (m_v3Max.y < min1.y)
	{
		return false;
	}
	if (m_v3Min.y > max1.y)
	{
		return false;
	}

	if (m_v3Max.z < min1.z)
	{
		return false;
	}
	if (m_v3Min.z > min1.z)
	{
		return false;
	}

	return true;
}

void Simplex::MyOctant::Display(int index, vector3 color)
{
	//Determine if this octant is the correct one to display
	if (m_iID == index)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
			glm::scale(vector3(m_fSize)), color, RENDER_WIRE);
	}
	for (int i = 0; i < m_iChildren; i++)
	{
		m_pChild[i]->Display(index);
	}
}

void Simplex::MyOctant::Display(vector3 color)
{
	//Display every octant
	for (int i = 0; i < m_iChildren; i++)
	{
		m_pChild[i]->Display(color);
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), color, RENDER_WIRE);
}

void Simplex::MyOctant::DisplayLeafs(vector3 color)
{
	int numLeaves = m_lChild.size();
	for (int i = 0; i < numLeaves; i++)
	{
		m_lChild[i]->DisplayLeafs(color);
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), color, RENDER_WIRE);
}

void Simplex::MyOctant::ClearEntityList()
{
	for (int i = 0; i < m_iChildren; i++)
	{
		m_pChild[i]->ClearEntityList();
	}
	m_EntityList.clear();
}

void Simplex::MyOctant::Subdivide()
{
	//If the max depth has been met
	if (m_iLevel >= m_uMaxLevel)
	{
		return;
	}

	//If there is already children existing for octant
	if (m_iChildren != 0)
	{
		return;
	}

	m_iChildren = 8;

	//Split the octant into smaller quadrants
	float fSize = m_fSize / 4.0f;
	float fSize2 = fSize * 2.0f;
	vector3 center;

	center = m_v3Center;
	center.x -= fSize;
	center.y -= fSize;
	center.z -= fSize;
	m_pChild[0] = new MyOctant(center, fSize2);

	center.x += fSize2;
	m_pChild[1] = new MyOctant(center, fSize2);

	center.z += fSize2;
	m_pChild[2] = new MyOctant(center, fSize2);

	center.x -= fSize2;
	m_pChild[3] = new MyOctant(center, fSize2);

	center.y += fSize2;
	m_pChild[4] = new MyOctant(center, fSize2);

	center.z -= fSize2;
	m_pChild[5] = new MyOctant(center, fSize2);

	center.x += fSize2;
	m_pChild[6] = new MyOctant(center, fSize2);

	center.z += fSize2;
	m_pChild[7] = new MyOctant(center, fSize2);

	for (int i = 0; i < 8; i++)
	{
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_iLevel = m_iLevel + 1;
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
		{
			m_pChild[i]->Subdivide();
		}
	}
}

MyOctant * Simplex::MyOctant::GetChild(int child)
{
	if (child > 7)
	{
		return nullptr;
	}
	return m_pChild[child];
}

MyOctant * Simplex::MyOctant::GetParent()
{
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf()
{
	if (m_iChildren == 0)
	{
		return true;
	}
	else
		return false;
}

bool Simplex::MyOctant::ContainsMoreThan(int entitiesAmount)
{
	int count = 0;
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		if (IsColliding(i))
		{
			count++;
		}
		if (count > entitiesAmount)
		{
			return true;
		}
	}
	return false;
}

void Simplex::MyOctant::KillBranches()
{
	for (int i = 0; i < m_iChildren; i++)
	{
		m_pChild[i]->KillBranches();
		delete m_pChild[i];
		m_pChild[i] = nullptr;
	}
	m_iChildren = 0;
}

void Simplex::MyOctant::ConstructTree(int maxLevel)
{
	//Make sure this is the root octant
	if (m_iLevel != 0)
	{
		return;
	}
	m_uMaxLevel = maxLevel;
	m_uOctantCount = 1;

	m_EntityList.clear();

	KillBranches();
	m_lChild.clear();

	if (ContainsMoreThan(m_uIdealEntityCount))
	{
		Subdivide();
	}

	AssignIDtoEntity();

	ConstructList();
}

void Simplex::MyOctant::AssignIDtoEntity()
{
	for (int i = 0; i < m_iChildren; i++)
	{
		m_pChild[i]->AssignIDtoEntity();
	}
	if (m_iChildren == 0)
	{
		for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			if (IsColliding(i))
			{
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_iID);
			}
		}
	}
}

int Simplex::MyOctant::GetOctantCount()
{
	return m_uOctantCount;
}


