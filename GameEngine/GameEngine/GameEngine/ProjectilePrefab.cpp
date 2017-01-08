#include "ProjectilePrefab.h"
#include "ProjectileScript.h"
#include "FileManager.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "CollisionManager.h"
#include "MeshRenderer.h"
#include "Texture.h"
#include "Material.h"
#include "DeferredShaderClass.h"

ProjectilePrefab* ProjectilePrefab::s_instance = nullptr;

ProjectilePrefab::ProjectilePrefab()
{
	projectileMesh = FileManager::loadObjMesh(L"projectile.obj");
	projectileTexture = new Texture(L"cubeTexture.bmp");
	projectileMaterial = new Material(DeferredShaderClass::INSTANCE, projectileTexture, PROJECTILE_MAT_SHININESS);
}

GameObject * ProjectilePrefab::instantiate(XMVECTOR position)
{
	GameObject* object = new GameObject();
	object->getTransform()->localPosition->x = position.m128_f32[0];
	object->getTransform()->localPosition->y = position.m128_f32[1];
	object->getTransform()->localPosition->z = position.m128_f32[2];

	MeshRenderer* renderer = new MeshRenderer(projectileMesh, projectileMaterial);
	BoxCollider* collider = new BoxCollider(CollisionManager::instance(), { 0.0F, 0.0F, 0.0F }, true, { 0.25f, 0.25f, 0.5f });
	Rigidbody* rigidbody = new Rigidbody();
	ProjectileScript* script = new ProjectileScript();

	object->addComponent(renderer);
	object->addComponent(rigidbody);
	object->addComponent(collider);
	object->addComponent(script);

	return object;
}
