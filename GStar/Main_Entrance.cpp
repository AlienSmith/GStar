#include"Scene.h"
#include"View.h"
#include"Controller.h"
#include"World.h"
#include"UpdateObject.h"
#include"MeshComponent.h"
#include"TextureComponent.h"
#include"ShaderComponent.h"
#include"TransformComponent.h"
#include"InterfaceComponent.h"
#include"Data.h"
#include"Assert.h"
#include"GLError.h"
#include"SceneInterface.h"
Scene* myScene = Scene::Create();
Controller myController(myScene);
View myView(myScene);
void mouse_call(GLFWwindow * window, double xpos, double ypos);
void framebuffer_size_call(GLFWwindow * windwo, int width, int height);
void CleanScreen();
void MainEntrance() {
	GStar::World& world = GStar::World::Instance();
	//Game Code
	if (GStar::SceneInterface::Instance) {
		GStar::SceneInterface::Instance->Start();
	}
	Object* tempObject = world.AddObject();
	tempObject->AddComponent(new GStar::MeshComponent(cubeparameters));
	tempObject->AddComponent(new GStar::ShaderComponent(DefaultShader2T));
	GStar::TextureComponent* tempComponent = new GStar::TextureComponent();
	tempComponent->Initialize(Default_Texture_BOX);
	tempComponent->Initialize(Default_TextureFace);
	tempObject->AddComponent(tempComponent);
	GStar::TransformComponent* TransformComponent = new GStar::TransformComponent(tempObject);
	world.AddToRoot(TransformComponent);
	TransformComponent->SetTransform(cubPosition[1]);
	tempObject->AddComponent(TransformComponent);

	Object* tempObject1 = world.AddObject();
	tempObject1->AddComponent(new GStar::MeshComponent(cubeparameters));
	tempObject1->AddComponent(new GStar::ShaderComponent(DefaultShader2T));
	tempComponent = new GStar::TextureComponent();
	tempComponent->Initialize(Default_TextureWall);
	tempComponent->Initialize(Default_TextureFace);
	tempObject1->AddComponent(tempComponent);
	GStar::TransformComponent* TransformComponent1 = new GStar::TransformComponent(tempObject1);
	TransformComponent->AddChildren(TransformComponent1);
	TransformComponent1->SetTransform(GStar::Vector3(0.0f,0.0f,5.0f));
	tempObject1->AddComponent(TransformComponent1);

	TransformComponent1 = reinterpret_cast<GStar::TransformComponent*> (tempObject1->GetComponent(TRANSFORM_WORD));
	
	//Set up 
	//Bind CallBack Function
	glfwSetFramebufferSizeCallback(myScene->Window(), framebuffer_size_call);
	glfwSetCursorPosCallback(myScene->Window(), mouse_call);
	//RenderLoop
	while (!glfwWindowShouldClose(myScene->Window())) {
		if (GStar::SceneInterface::Instance) {
			GStar::SceneInterface::Instance->Update();
		}
		TransformComponent->SetRotation(GStar::Vector3(Scene::Create()->TotalTime() * 100, 0.0f, 0.0f));
		TransformComponent1->SetRotation(GStar::Vector3(0.0f, Scene::Create()->TotalTime() * 100, 0.0f));
		myScene->UpdateTime();
		myController.Update();
		CleanScreen();
		//myScene->Update();
		//myView.Update();
		//TransformComponent->SetTransform(GStar::Vector3(sin(Scene::Create()->TotalTime())*2, cos(Scene::Create()->TotalTime())*2, 0.0f ));
		//TransformComponent1->SetTransform(GStar::Vector3(cos(Scene::Create()->TotalTime()) * 2, 0.0f, sin(Scene::Create()->TotalTime()) * 2));
		world.Update();
		/*tempObject.Update();

		tempObject1.Update();*/

		myScene->UpdateEnd();
	}
	myScene->TerminateWindow();
	if (GStar::SceneInterface::Instance) {
		GStar::SceneInterface::Instance->Terminate();
	}
	delete myScene;
}
void mouse_call(GLFWwindow * window, double xpos, double ypos)
{
	myController.mouse_callBack(window, xpos, ypos);
}
void framebuffer_size_call(GLFWwindow * windwo, int width, int height)
{
	myController.framebuffer_size_callback(windwo, width, height);
}
void CleanScreen() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// set color
	   //glClear(GL_COLOR_BUFFER_BIT); // only clean the color not depth and stencil
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//  clean the color and depth
}