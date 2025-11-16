#include "ECS.h"

void Entity::addGroup(Group group) {

	//The bitset knows which group to set because
	//group is probably an enum with numeric values
	groupBitSet[group] = true;
	manager.AddToGroup(this, group);
}