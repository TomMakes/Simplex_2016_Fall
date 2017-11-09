#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	//m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);

	//init the camera

	m_pCamera = new MyCamera();
	targetDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	camUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	m_pCamera->SetPositionTargetAndUp(
			vector3(0.0f, 3.0f, 20.0f), //Where my eyes are
			targetDirection, //where what I'm looking at is
			camUpDirection);					//what is up

	//Get the singleton
	m_pMyMeshMngr = MyMeshManager::GetInstance();
	m_pMyMeshMngr->SetCamera(m_pCamera);
	rotation = vector3(0.0f, 0.0f, 1.0f);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	//ArcBall();

	//Getting the key input checks for moving the camera around
	if (wDown == true)
	{
		//Move position fowards in the direction it is facing
		cameraPosition += m_fSpeed * (vector3(0.0f,0.0f,1.0f) * myQuat);
	}
	if (sDown == true)
	{
		cameraPosition -= m_fSpeed * (vector3(0.0f, 0.0f, 1.0f) * myQuat);
	}
	if (aDown == true)
	{
		//Move the camera position sideways, for some reason this also rotates
		cameraPosition -= (vector3(1.0f, 0.0f, 0.0f) * myQuat) * m_fSpeed;
	}
	if (dDown == true)
	{
		cameraPosition += (vector3(1.0f, 0.0f, 0.0f) * myQuat) * m_fSpeed;//glm::normalize(glm::cross(targetDirection, camUpDirection)) * m_fSpeed;
	}

	//xPos = 0 + m_sSpeed; //Controlled by D and A
	//zPos = 19 + m_fSpeed; //Controlled by W and S
	//cameraPosition = vector3(xPos, 3.0f, zPos);

	//Check if user is holding right click
	if (m_bFPC == true)
	{
		rotation.x +=(m_v3Mouse.x - previousRotation.x) * (PI/180)  * mouseSensitivity;  //m_v3Mouse.x* (PI / 180);
		rotation.y +=(m_v3Mouse.y - previousRotation.y) * (PI/180)  * mouseSensitivity; // m_v3Mouse.y* (PI / 180);
		if (previousRotation.x != rotation.x)
		{
			myQuat = glm::quat(rotation);
			//glm::quat myQuat2 = glm::quat(rotation.y, vector3(0, 1, 0));


			rotation = vector3(0.0f, 0.0f, 1.0f) *  (myQuat);
			std::cout << rotation.x << "x\n";
			std::cout << rotation.y << "y\n";
		}
	}

	previousRotation = vector3(m_v3Mouse.x, m_v3Mouse.y, 0.0f);
	targetDirection =  cameraPosition + glm::normalize(rotation);

	//xPos += 0.01f;
	//Make it so you can move around the scene
	m_pCamera->SetPositionTargetAndUp(
		cameraPosition, //Where my eyes are
		targetDirection, //where what I'm looking at is
		((vector3(0.0f, 1.0f, 0.0f) * myQuat) - cameraPosition));					//what is up

	//Is the first person camera active?
	//CameraRotation();
	
	
	//Add objects to the Manager
	for (int j = -50; j < 50; j += 2)
	{
		for (int i = -50; i < 50; i += 2)
		{
			m_pMyMeshMngr->AddConeToRenderList(glm::translate(vector3(i, 0.0f, j)));
		}
	}
}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//Render the list of MyMeshManager
	m_pMyMeshMngr->Render();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the MyMeshManager list
	m_pMyMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the singleton
	MyMeshManager::ReleaseInstance();

	//release the camera
	SafeDelete(m_pCamera);

	//release GUI
	ShutdownGUI();
}
