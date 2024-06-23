module;

#include <vector>
#include <string>
#include <math.h>

#include <iostream>

#include <glm/glm.hpp>

module Engine4D.Components;

namespace Engine4D
{
	MeshRenderer::MeshRenderer(GameObject* gameObject) : MonoBehavior(gameObject, 3)
	{
		this->mesh = Mesh();
		this->name = "MeshRenderer";
		//this->material = gameObject->engine->AddMaterial();
		this->material = nullptr;
		if (this->material == nullptr)
		{
			//std::cout << "Material is null" << std::endl;
		}
	}

	MeshRenderer::MeshRenderer(GameObject* gameObject, Material* material) : MonoBehavior(gameObject, 3)
	{
		this->mesh = Mesh();
		this->material = material;
	}

	MeshRenderer::MeshRenderer(GameObject* gameObject, Mesh mesh, Material* material) : MonoBehavior(gameObject, 3)
	{
		this->mesh = mesh;
		this->material = material;
	}

	void MeshRenderer::AddShape(Shape* shape)
	{
		this->mesh.AddShape(shape);
		gameObject->engine->sceneChanged = true;
	}

	std::string MeshRenderer::toString()
	{
		return "MeshRenderer";
	}

	std::vector<InstructionData> MeshRenderer::getInstructions()
	{
		std::vector<InstructionData> instructions = std::vector<InstructionData>();

		int n = this->mesh.shapes.size();

		if (n == 0)
		{
			return instructions;
		}
		else
		{
			Instruction initInstruction = Instruction();
			initInstruction.type = 21;
			initInstruction.valueA = glm::mat4(0);
			initInstruction.valueB = glm::vec4(material->index, 0, 0, 0);
			//std::cout << "\tMaterial Index: " << material->index << std::endl;
			//initInstruction.valueB = (glm::vec4)material.color;
			instructions.push_back(InstructionToData(initInstruction));

			for (int i = 0; i < n; i++)
			{
				if (i < n - 1)
				{
					Instruction unionInstruction = Instruction();
					unionInstruction.type = 13;
					unionInstruction.valueA = glm::mat4(0);
					unionInstruction.valueB = glm::vec4(0);

					instructions.push_back(InstructionToData(unionInstruction));
				}

				Shape* shape = this->mesh.shapes[i];

				Instruction shapeInstruction = Instruction();
				shapeInstruction = shape->getInstruction();
				shapeInstruction.valueA = transform->transformationMatrix * shapeInstruction.valueA;
				shapeInstruction.valueB = (glm::vec4)(transform->position) + shapeInstruction.valueB;
				//shapeInstruction.valueB = (transform->transformationMatrix * shapeInstruction.valueB) + transform->positionTransformed;
				instructions.push_back(InstructionToData(shapeInstruction));
			}
		}
		return instructions;
	}

	RigidBody::RigidBody(GameObject* gameObject) : MonoBehavior(gameObject, 4)
	{
		this->name = "RigidBody";
		//this->gravity = Vector4(0, -9.8f, 0, 0);
		this->gravity = Vector4(0, 0.0f, 0, 0);
		this->velocity = Vector4(0, 0, 0, 0);
		this->acceleration = Vector4(0, 0, 0, 0);
		this->rotationalVelocity = Vector3(0, 0, 0);
		this->rotationalVelocityW = Vector3(0, 0, 0);
		this->mass = 1;
		this->elasticity = 0;
		this->collisionsHandled = std::unordered_set<RigidBody*>();
	}

	void RigidBody::Update()
	{
		//std::cout << "RigidBody Update" << std::endl;

		collisionsHandled.clear();

		velocity += gravity * gameObject->engine->Time->deltaTime;
		transform->position += velocity * gameObject->engine->Time->deltaTime;
		transform->rotation += rotationalVelocity * gameObject->engine->Time->deltaTime;
		transform->rotation.x = std::fmod(transform->rotation.x, 2 * PI);
		transform->rotation.y = std::fmod(transform->rotation.y, 2 * PI);
		transform->rotation.z = std::fmod(transform->rotation.z, 2 * PI);
		transform->rotationW += rotationalVelocityW * gameObject->engine->Time->deltaTime;
		transform->rotationW.x = std::fmod(transform->rotationW.x, 2 * PI);
		transform->rotationW.y = std::fmod(transform->rotationW.y, 2 * PI);
		transform->rotationW.z = std::fmod(transform->rotationW.z, 2 * PI);

		if (transform->position.y < -5 || transform->position.y > 50)
		{
			transform->position.y = 20;
			velocity.y = 0;
		}

		if (transform->position.x < -50 || transform->position.x > 50)
		{
			transform->position.x = 0;
			velocity.x = 0;
		}

		if (transform->position.z < -50 || transform->position.z > 50)
		{
			transform->position.z = 0;
			velocity.z = 0;
		}

		if (transform->position.w < -50 || transform->position.w > 50)
		{
			transform->position.w = 0;
			velocity.w = 0;
		}

		//MeshRenderer* rend = gameObject->GetComponent<MeshRenderer>();
		//if (rend != nullptr) rend->

		transform->setTransformationMatrix();
		gameObject->engine->sceneChanged = true;

		if (countdowntime > 10)
		{
			std::cout << "Position: " << transform->position << std::endl;
			//std::cout << "Velocity: " << velocity << std::endl;
			//std::cout << "Acceleration: " << acceleration << std::endl;
			//std::cout << "Rotation: " << transform->rotation << std::endl;
			//std::cout << "RotationW: " << transform->rotationW << std::endl;

			countdowntime = 0;
		}

		//std::cout << "Position: " << transform->position << std::endl;
		/*std::cout << "Velocity: " << velocity << std::endl;
		std::cout << "Acceleration: " << acceleration << std::endl;
		std::cout << "Rotation: " << transform->rotation << std::endl;
		std::cout << "RotationW: " << transform->rotationW << std::endl;

		std::cout << "RigidBody Update Finished" << std::endl;*/

		countdowntime += gameObject->engine->Time->deltaTime;
	}

	void RigidBody::addForce(Vector4 force)
	{
		acceleration += force / mass;
	}

	void RigidBody::OnCollisionEnter(Collision collision)
	{
		std::cout << "Collision Entered." << std::endl;
		//std::cout << collision.collider2->gameObject->name << std::endl;

		Vector4 normal;
		Vector4 otherNormal;

		Collider* otherCollider;

		if (collision.collider1->gameObject == gameObject)
		{
			otherCollider = collision.collider2;
			normal = collision.collisionNormalA;
			otherNormal = collision.collisionNormalB;
		}
		else //if (collision.collider2->gameObject == gameObject)
		{
			otherCollider = collision.collider1;
			normal = collision.collisionNormalB;
			otherNormal = collision.collisionNormalA;
		}

		std::cout << "TryGetComponent - " << gameObject->TryGetComponent<RigidBody>() << std::endl;
		std::cout << "TryGetComponentOther - " << otherCollider->gameObject->TryGetComponent<RigidBody>() << std::endl;

		if (normal == Vector4(0, 0, 0, 0))
		{
			if (otherNormal == Vector4(0, 0, 0, 0)) return;
			normal = -otherNormal;
		}
		if (otherNormal == Vector4(0, 0, 0, 0))
		{
			if (normal == Vector4(0, 0, 0, 0)) return;
			otherNormal = -normal;
		}

		

		//std::cout << "Normal - " << normal << std::endl;
		//std::cout << "Other Normal - " << otherNormal << std::endl;

		//std::cout << "Collision Entered - 1" << std::endl;

		Vector4 directionalVelocity = -normal * dot(velocity, -normal);
		float momentum = mass * directionalVelocity.length();

		RigidBody* otherRigidBody;
		if (otherCollider->gameObject->TryGetComponent<RigidBody>(otherRigidBody))
		{
			//std::cout << "Collision Entered - 2" << std::endl;
			if (collisionsHandled.count(otherRigidBody) > 0 || otherRigidBody->collisionsHandled.count(this) > 0)
			{
				std::cout << "Already Did Collision." << std::endl;
				return;
			}

			float combinedElasiticity = elasticity * otherRigidBody->elasticity;
			Vector4 otherDirectionalVelocity = -normal * dot(otherRigidBody->velocity, -normal);
			float otherMomentum = otherRigidBody->mass * otherDirectionalVelocity.length();
			float otherMass = otherRigidBody->mass;

			if (objectType == 0)
			{
				mass = UINT32_MAX;
				momentum = 0;
			}

			if (otherRigidBody->objectType == 0)
			{
				otherMass = UINT32_MAX;
				otherMomentum = 0;
			}

			if (objectType == 1 && otherRigidBody->objectType == 1)
			{

				std::cout << "Both Dynamic" << std::endl;
				//return;
			}

			float newDirectionalVelocityMagnitude;
			float newOtherDirectionalVelocityMagnitude;

			newDirectionalVelocityMagnitude = (momentum + otherMomentum + otherMass * combinedElasiticity * (otherDirectionalVelocity.length() - directionalVelocity.length())) / (mass + otherMass);
			newOtherDirectionalVelocityMagnitude = (momentum + otherMomentum + mass * combinedElasiticity * (directionalVelocity.length() - otherDirectionalVelocity.length())) / (mass + otherMass);

			// old code didnt work? saving in case current doesn't work
			//newDirectionalVelocityMagnitude = (momentum * (mass + 1.0 / combinedElasiticity) + otherMomentum * (mass - 1.0 / combinedElasiticity)) / (2.0f * mass);
			//newOtherDirectionalVelocityMagnitude = (momentum * (otherMass - 1.0 / combinedElasiticity) + otherMomentum * (otherMass + 1.0 / combinedElasiticity)) / (2.0f * otherMass);

			//std::cout << "New Magnitude: " << newDirectionalVelocityMagnitude << std::endl;
			//std::cout << "New Other Magnitude: " << newOtherDirectionalVelocityMagnitude << std::endl;

			Vector4 newDirectionalVelocity = normal * newDirectionalVelocityMagnitude * -1;
			Vector4 newOtherDirectionalVelocity = normal * newOtherDirectionalVelocityMagnitude * -1;

			//Vector4 newDirectionalVelocity = directionalVelocity.normalized() * newDirectionalVelocityMagnitude * -1;
			//Vector4 newOtherDirectionalVelocity = otherDirectionalVelocity.normalized() * newOtherDirectionalVelocityMagnitude * -1;

			//std::cout << "Old Velocity: " << velocity << std::endl;

			velocity += (newDirectionalVelocity - directionalVelocity);
			otherRigidBody->velocity += (newOtherDirectionalVelocity - otherDirectionalVelocity);

			velocity += normal * (collision.collisionPointA - collision.collisionNormalB).length() / 2.0;
			otherRigidBody->velocity += normal * (collision.collisionPointA - collision.collisionNormalB).length() / 2.0;

			collisionsHandled.insert(otherRigidBody);
			otherRigidBody->collisionsHandled.insert(this);

			//std::cout << "New Velocity: " << velocity << std::endl;

			//std::cout << "Collision Entered Almost Finished." << std::endl;

		}
		else
		{
			//std::cout << "Collision Entered - 2" << std::endl;
			float combinedElasiticity = elasticity;

			if (objectType == 0)
			{
				return;
			}

			float otherMass = UINT32_MAX;
			float otherMomentum = 0;

			float newDirectionalVelocityMagnitude = (momentum + otherMomentum + otherMass * combinedElasiticity * (0 - directionalVelocity.length())) / (mass + otherMass);
			// old code didnt work? saving in case current doesn't work
			//float newDirectionalVelocityMagnitude = (momentum * (mass + 1.0 / combinedElasiticity) + otherMomentum * (mass - 1.0 / combinedElasiticity)) / (2.0f * mass);
			std::cout << "1 - New Magnitude: " << newDirectionalVelocityMagnitude << std::endl;

			//Vector4 newDirectionalVelocity = directionalVelocity.normalized() * newDirectionalVelocityMagnitude * -1;

			Vector4 newDirectionalVelocity = normal * newDirectionalVelocityMagnitude;

			/*if (directionalVelocity.length() == 0)
			{
				
			}*/			

			velocity += (newDirectionalVelocity - directionalVelocity);

			velocity += normal * (collision.collisionPointA - collision.collisionNormalB).length() / 2.0;

			/*
			std::cout << "Normal: " << normal << std::endl;
			std::cout << "Other Normal" << otherNormal << std::endl;
			std::cout << "New Velocity: " << velocity << std::endl;
			std::cout << "Position: " << transform->position << std::endl;

			//std::cout << "1 - Collision Entered Almost Finished." << std::endl;*/
		}

		//std::cout << "Collision Entered Finished." << std::endl;

	}



	void RigidBody::OnCollisionStay(Collision collision)
	{
		//std::cout << "Collision Stayed." << std::endl;
		//std::cout << collision.collider2->gameObject->name << std::endl;

		Vector4 normal;
		Vector4 otherNormal;

		Collider* otherCollider;

		if (collision.collider1->gameObject == gameObject)
		{
			otherCollider = collision.collider2;
			normal = collision.collisionNormalA;
			otherNormal = collision.collisionNormalB;
		}
		else //if (collision.collider2->gameObject == gameObject)
		{
			otherCollider = collision.collider1;
			normal = collision.collisionNormalB;
			otherNormal = collision.collisionNormalA;
		}

		//std::cout << "Normal - " << normal << std::endl;
		//std::cout << "Other Normal - " << otherNormal << std::endl;

		//std::cout << "Collision Entered - 1" << std::endl;

		Vector4 directionalVelocity = -normal * dot(velocity, -normal);
		float momentum = mass * directionalVelocity.length();

		RigidBody* otherRigidBody;
		if (otherCollider->gameObject->TryGetComponent<RigidBody>(otherRigidBody))
		{
			//std::cout << "Collision Entered - 2" << std::endl;
			if (collisionsHandled.count(otherRigidBody) > 0 || otherRigidBody->collisionsHandled.count(this) > 0)
			{
				//std::cout << "Already Did Collision." << std::endl;
				return;
			}

			float combinedElasiticity = elasticity * otherRigidBody->elasticity;
			Vector4 otherDirectionalVelocity = -normal * dot(otherRigidBody->velocity, -normal);
			float otherMomentum = otherRigidBody->mass * otherDirectionalVelocity.length();
			float otherMass = otherRigidBody->mass;

			if (objectType == 0)
			{
				mass = UINT32_MAX;
				momentum = 0;
			}

			if (otherRigidBody->objectType == 0)
			{
				otherMass = UINT32_MAX;
				otherMomentum = 0;
			}

			if (objectType == 1 && otherRigidBody->objectType == 1)
			{

				std::cout << "Both Dynamic" << std::endl;
				//return;
			}

			float newDirectionalVelocityMagnitude;
			float newOtherDirectionalVelocityMagnitude;

			newDirectionalVelocityMagnitude = (momentum + otherMomentum + otherMass * combinedElasiticity * (otherDirectionalVelocity.length() - directionalVelocity.length())) / (mass + otherMass);
			newOtherDirectionalVelocityMagnitude = (momentum + otherMomentum + mass * combinedElasiticity * (directionalVelocity.length() - otherDirectionalVelocity.length())) / (mass + otherMass);

			// old code didnt work? saving in case current doesn't work
			//newDirectionalVelocityMagnitude = (momentum * (mass + 1.0 / combinedElasiticity) + otherMomentum * (mass - 1.0 / combinedElasiticity)) / (2.0f * mass);
			//newOtherDirectionalVelocityMagnitude = (momentum * (otherMass - 1.0 / combinedElasiticity) + otherMomentum * (otherMass + 1.0 / combinedElasiticity)) / (2.0f * otherMass);

			//std::cout << "New Magnitude: " << newDirectionalVelocityMagnitude << std::endl;
			//std::cout << "New Other Magnitude: " << newOtherDirectionalVelocityMagnitude << std::endl;

			Vector4 newDirectionalVelocity = normal * newDirectionalVelocityMagnitude * -1;
			Vector4 newOtherDirectionalVelocity = normal * newOtherDirectionalVelocityMagnitude * -1;

			//Vector4 newDirectionalVelocity = directionalVelocity.normalized() * newDirectionalVelocityMagnitude * -1;
			//Vector4 newOtherDirectionalVelocity = otherDirectionalVelocity.normalized() * newOtherDirectionalVelocityMagnitude * -1;

			//std::cout << "Old Velocity: " << velocity << std::endl;

			velocity += (newDirectionalVelocity - directionalVelocity);
			otherRigidBody->velocity += (newOtherDirectionalVelocity - otherDirectionalVelocity);

			velocity += normal * (collision.collisionPointA - collision.collisionNormalB).length() / 2.0;
			otherRigidBody->velocity += normal * (collision.collisionPointA - collision.collisionNormalB).length() / 2.0;

			collisionsHandled.insert(otherRigidBody);
			otherRigidBody->collisionsHandled.insert(this);

			//std::cout << "New Velocity: " << velocity << std::endl;

			//std::cout << "Collision Entered Almost Finished." << std::endl;

		}
		else
		{
			//std::cout << "Collision Entered - 2" << std::endl;
			float combinedElasiticity = elasticity;

			if (objectType == 0)
			{
				return;
			}

			float otherMass = UINT32_MAX;
			float otherMomentum = 0;

			float newDirectionalVelocityMagnitude = (momentum + otherMomentum + otherMass * combinedElasiticity * (0 - directionalVelocity.length())) / (mass + otherMass);
			// old code didnt work? saving in case current doesn't work
			//float newDirectionalVelocityMagnitude = (momentum * (mass + 1.0 / combinedElasiticity) + otherMomentum * (mass - 1.0 / combinedElasiticity)) / (2.0f * mass);
			//std::cout << "1 - New Magnitude: " << newDirectionalVelocityMagnitude << std::endl;

			//Vector4 newDirectionalVelocity = directionalVelocity.normalized() * newDirectionalVelocityMagnitude * -1;

			Vector4 newDirectionalVelocity = normal * newDirectionalVelocityMagnitude;

			/*if (directionalVelocity.length() == 0)
			{

			}*/

			velocity += (newDirectionalVelocity - directionalVelocity);

			//std::cout << "New Velocity: " << velocity << std::endl;

			velocity += normal * (collision.collisionPointA - collision.collisionNormalB).length() / 2.0;

			/*
			std::cout << "Normal: " << normal << std::endl;
			std::cout << "Other Normal" << otherNormal << std::endl;
			std::cout << "New Velocity: " << velocity << std::endl;
			std::cout << "Position: " << transform->position << std::endl;

			//std::cout << "2 - Collision Entered Almost Finished." << std::endl;*/
		}

		//std::cout << "Collision Entered Finished." << std::endl;

	}

	void RigidBody::OnCollisionExit(Collision collision)
	{
		std::cout << "Collision Exited." << std::endl;
		//std::cout << collision.collider2->gameObject->name << std::endl;
	}

	std::string RigidBody::toString()
	{
		return "RigidBody";
	}
}