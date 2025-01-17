#include "Component.h"

#include "Entity.h"
#include "Components/TransformComponent.h"

#include <Utils/IDUtils.h>

Component::Component(Entity* mEntity)
{
    this->componentID = uuidGen.getUUID();
    this->entity = mEntity;
}

bool Component::operator==(const Component& other) const
{
    return (this->componentDisplayName == other.componentDisplayName)
        &&
        (this->componentID == other.componentID);
}







