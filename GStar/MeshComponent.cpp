#include "MeshComponent.h"
#include "MeshManager.h"
#include "Scene.h"
bool GStar::MeshComponent::Initialize(const MeshParameters& parameters)
{
	my_mesh = GStar::MeshManager::Instance()->GetMesh(parameters);
	//TODO add some kind of check for result;
	return true;
}

 bool GStar::MeshComponent::Update() const
{
	glBindVertexArray(my_mesh.myVAO());
	return true;
}
