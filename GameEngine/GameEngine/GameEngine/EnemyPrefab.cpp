#include "EnemyPrefab.h"
#include "EnemyBehaviorScript.h"
#include "FileManager.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "CollisionManager.h"
#include "MeshRenderer.h"
#include "Texture.h"
#include "Material.h"
#include "DeferredShaderClass.h"

EnemyPrefab* EnemyPrefab::s_instance = nullptr;

EnemyPrefab::EnemyPrefab()
{
	mesh = FileManager::loadObjMesh(L"turret.obj");
	texture = new Texture(L"cubeTexture.bmp");
	material = new Material(DeferredShaderClass::INSTANCE, texture, ENEMY_MAT_SHININESS);
}

GameObject * EnemyPrefab::instantiate(XMVECTOR position)
{
	GameObject* object = new GameObject();
	object->getTransform()->localPosition->x = position.m128_f32[0];
	object->getTransform()->localPosition->y = position.m128_f32[1];
	object->getTransform()->localPosition->z = position.m128_f32[2];

	MeshRenderer* renderer = new MeshRenderer(mesh, material);
	BoxCollider* collider = new BoxCollider(CollisionManager::instance(), { 0.0F, 2.0F, 0.0F }, false, { 1.0f, 2.0f, 1.0f });
	Rigidbody* rigidbody = new Rigidbody();
	EnemyBehaviorScript* script = new EnemyBehaviorScript();

	object->addComponent(renderer);
	object->addComponent(rigidbody);
	object->addComponent(collider);
	object->addComponent(script);

	return object;
}
