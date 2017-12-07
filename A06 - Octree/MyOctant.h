/*----------------------------------------------
Programmer: Thomas Ryszkiewicz
Date: 2017/12
----------------------------------------------*/
#ifndef __MYOCTANTCLASS_H_
#define __MYOCTANTCLASS_H_
#include "MyEntityManager.h"

namespace Simplex
{
	class MyOctant
	{
		static int m_uOctantCount; //amount of octant squares instantiated
		static int m_uMaxLevel; //Sets the deepest subdivision level the octree can go 
		static int m_uIdealEntityCount; //Sets the amount of entities that should idealy be contained within each subdivision

		int m_iID = 0; //current id for this octant
		int m_iLevel = 0; //The current level the octant is in
		int m_iChildren = 0; //Number of children on the octant (Value of 0 for none, or 8 for full subdivision)

		float m_fSize = 0.0f; //Size of the octant (Each one is a cube)

		MeshManager* m_pMeshMngr = nullptr; //Meshmanager storage to read each mesh 
		MyEntityManager* m_pEntityMngr = nullptr; //Acess to the colliders and the like

		vector3 m_v3Center = vector3(0.0f); //center point of the octant
		vector3 m_v3Min = vector3(0.0f); //minimum point of the octant
		vector3 m_v3Max = vector3(0.0f); //Max point of octant

		MyOctant* m_pParent = nullptr; //Says parent octant of this octant
		MyOctant* m_pChild[8]; //Stores the children of this octant

		std::vector<int> m_EntityList; //List of entities existing in octant

		MyOctant* m_pRoot = nullptr; //The root octant (original overlord)
		std::vector<MyOctant*> m_lChild; //List of the nodes that contain objects (Only for root octant)

	public:
		//Create octant through the Max level allowed and ideal amount of entities contained
		MyOctant(uint maxLevel = 3, uint idealEntityCount = 5);
		//Create octant using center and size
		MyOctant(vector3 octCenter, float octSize);
		//Copy from other octant, copy constructor 
		MyOctant(MyOctant const& other);
		//Simple copy assignment operator
		MyOctant& operator=(MyOctant const& other);
		//Destructor
		~MyOctant();
		//Swaps the content between both octants
		void Swap(MyOctant& other);
		//Get the size of octant
		float GetSize();
		//Gets the center of octant in global space
		vector3 GetCenterGlobal();
		//Gets the Min point in global
		vector3 GetMinGlobal();
		//Gets max
		vector3 GetMaxGlobal();
		//Checks for collisions with entity in question
		bool IsColliding(int rBIndex);
		//Displays the octant volume specified by index in specific color
		void Display(int index, vector3 color = C_YELLOW);
		//Displays volume in color specified
		void Display(vector3 color = C_YELLOW);
		//Displays nonempty leaves in the octree
		void DisplayLeafs(vector3 color = C_YELLOW);
		//Clears entity list for each node
		void ClearEntityList();
		//Allocates 8 smaller octants within this octant
		void Subdivide();
		//Returns the child specified from index
		MyOctant* GetChild(int child);
		//Returns parent of this octant
		MyOctant* GetParent();
		//Does this octant not contain any children?
		bool IsLeaf();
		//Asks if Octant contains more than this many objects
		bool ContainsMoreThan(int entitiesAmount);
		//Deletes all of the children and their children
		void KillBranches();
		//Creates tree that is the specififed subdivisions large
		void ConstructTree(int maxLevel = 3);
		//Traverse the tree to leaves and sets objects in them to index
		void AssignIDtoEntity();
		//Gets the total number of octants existing
		int GetOctantCount();

	private:
		void Release();
		void Init();
		//Creates a list of all leafs that contain objects.
		void ConstructList();
	};
}
#endif //__MYOCTANTCLASS_H_

