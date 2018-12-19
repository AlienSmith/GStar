#include "ShaderComponent.h"
#include "ShaderManger.h"
#include "ConsolePrint.h"
#include "Shader.h"
#include "Scene.h"
#include "UpdateObject.h"
#include "Data.h"
#include "LightManager.h"
bool GStar::ShaderComponent::Initialize(const ShaderParameters & parameters)
{
	my_shader = ShaderManager::Instance()->GetShader(parameters);
	if (my_shader) {
		return true;
	}
	DEBUG_PRINT(GStar::LOGPlatform::Console, GStar::LOGType::Waring, "Shader Component failed to initialize");
	return false;
}

bool GStar::ShaderComponent::Update() const
{
	my_shader->use();
	if (t1) {
		my_shader->setInt("texture1", 0);
	}if (t2) {
		my_shader->setInt("texture2", 1);
	}if (lighting) {
		my_shader->setVec3("lightColor", LightManager::Instance()->GetLight());
	}
	Matrix4 view = Scene::Create()->getview();
	my_shader->setMat4("view", Scene::Create()->getview(), GL_FALSE);
	Matrix4 projection = Scene::Create()->getProjection();
	my_shader->setMat4("projection", Scene::Create()->getProjection(), GL_FALSE);	
	Matrix4 model = UpdateObject::OUT_Instance->GetTransformComponent()->getModel();
	my_shader->setMat4("model", UpdateObject::OUT_Instance->GetTransformComponent()->getModel(), GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, UpdateObject::OUT_Instance->GetMeshComponent()->GetMeshInfo().TriangleIndex());
	
	return true;
}

GStar::ShaderComponent::ShaderComponent(const ShaderParameters & parameters):
	Component(SHADER_WORD),
	my_shader(nullptr),
	t1(parameters.texture1),
	t2(parameters.texture2),
	lighting(parameters.lighting)
{
	Initialize(parameters);
}
