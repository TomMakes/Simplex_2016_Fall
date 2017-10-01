#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(2.0f, C_BROWN);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_WHITE);

	spaceInvader_movement = 0.0f;

	m_pSpaceInvader = new MyMesh[48];

	for (int i = 0; i < 48; i++)
	{
		m_pSpaceInvader[i].GenerateCube(1.0f, C_BLUE);
	}

}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	//m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3( 3.0f, 0.0f, 0.0f)));

	//Stuff for making the space invader move around the screen.


	//CREATION of the Space Invader!!!!!!!!!!!!!!
	//Have the longest axis made by a for loop
	for (int i = 0; i < 11; i++)
	{
		m_pSpaceInvader[i].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-i) + spaceInvader_movement, 0.0f, 0.0f)));

		//Creating the gaps on the third layer from bottom
		if (i != 9 && i != 1)
		{
			m_pSpaceInvader[i + 11].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-i) + spaceInvader_movement, -1.0f, 0.0f)));
		}
		//Creating fifth layer
		if (i != 10 && i != 0 && i != 3 && i != 7)
		{
			m_pSpaceInvader[i + 11].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-i) + spaceInvader_movement, 1.0f, 0.0f)));
		}
	}
	//Creating sixth layer
	for (int i = 2; i < 9; i++)
	{
		m_pSpaceInvader[i].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-i) + spaceInvader_movement, 2.0f, 0.0f)));
	}
	//Creating 7th and 8th
	m_pSpaceInvader[20].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-3) + spaceInvader_movement, 3.0f, 0.0f)));
	m_pSpaceInvader[12].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-7) + spaceInvader_movement, 3.0f, 0.0f)));
	m_pSpaceInvader[21].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-2) + spaceInvader_movement, 4.0f, 0.0f)));
	m_pSpaceInvader[11].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-8) + spaceInvader_movement, 4.0f, 0.0f)));

	//Creating 2nd and 3rd layer
	m_pSpaceInvader[20].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((0) + spaceInvader_movement, -2.0f, 0.0f)));
	m_pSpaceInvader[20].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-2) + spaceInvader_movement, -2.0f, 0.0f)));
	m_pSpaceInvader[20].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-10) + spaceInvader_movement, -2.0f, 0.0f)));
	m_pSpaceInvader[20].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-8) + spaceInvader_movement, -2.0f, 0.0f)));

	//Creating First layer
	m_pSpaceInvader[20].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-3) + spaceInvader_movement, -3.0f, 0.0f)));
	m_pSpaceInvader[20].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-4) + spaceInvader_movement, -3.0f, 0.0f)));
	m_pSpaceInvader[20].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-6) + spaceInvader_movement, -3.0f, 0.0f)));
	m_pSpaceInvader[20].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3((-7) + spaceInvader_movement, -3.0f, 0.0f)));


	//Increment the movement
	if (spaceInvader_movement < 10.0f)
		spaceInvader_movement += 0.1f;
	else
		spaceInvader_movement -= 0.1f;

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}