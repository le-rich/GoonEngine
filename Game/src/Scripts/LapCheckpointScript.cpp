#include "Scripts/LapCheckpointScript.h"

LapCheckpointScript::LapCheckpointScript(Entity* mEntity, LapManagerScript* lapManager,
	TransformComponent* other, int checkpointIndex) 
	: Script(mEntity), m_LapManager(lapManager), m_Other(other), m_Index(checkpointIndex)
{
	m_Self = this->entity->transform;
}

LapCheckpointScript::~LapCheckpointScript()
{
}

void LapCheckpointScript::Update(float deltaTime)
{
	if (!m_Self || !m_Other) { return; }

	glm::vec3 selfPosition = m_Self->getWorldPosition();
	glm::vec3 otherPosition = m_Other->getWorldPosition();

	// make bounding box for this entity
	glm::vec3 boxMin = glm::vec3(selfPosition.x - m_BBOffset, selfPosition.y - m_BBOffset, selfPosition.z - m_BBOffset);
	glm::vec3 boxMax = glm::vec3(selfPosition.x + m_BBOffset, selfPosition.y + m_BBOffset, selfPosition.z + m_BBOffset);
	m_SelfBB = new AABB(boxMin, boxMax);

	// make bounding box for car entity
	boxMin = glm::vec3(otherPosition.x - m_BBOffset, otherPosition.y - m_BBOffset, otherPosition.z - m_BBOffset);
	boxMax = glm::vec3(otherPosition.x + m_BBOffset, otherPosition.y + m_BBOffset, otherPosition.z + m_BBOffset);
	m_OtherBB = new AABB(boxMin, boxMax);

	if (AABBCollision(m_SelfBB, m_OtherBB))
	{
		// TODO call lapManager method
	}
}

bool LapCheckpointScript::AABBCollision(AABB* self, AABB* other)
{
	float d1x = other->min.x - self->max.x;
	float d1y = other->min.y - self->max.y;
	float d1z = other->min.z - self->max.z;
	float d2x = self->min.x - other->max.x;
	float d2y = self->min.y - other->max.y;
	float d2z = self->min.z - other->max.z;

	if (d1x > 0.0f || d1y > 0.0f || d1z > 0.0f)
		return false;
	if (d2x > 0.0f || d2y > 0.0f || d2z > 0.0f)
		return false;

	return true;
}
