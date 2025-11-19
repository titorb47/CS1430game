#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
using namespace std;


//Declared Classes
class Component;
class Entity;
class Manager;

//ComponentID and GroupID is an alias for size_t
using ComponentID = size_t;


//Group is used for grouping objects for collisions and rendering
using Group = size_t;


/* The following two functions (bottom overloads the top) gets
* the type for each component
*/

/* inline tells the compiler the function should be expanded
   at each place it is called  */
inline ComponentID getNewComponentTypeID() {
	static ComponentID lastID = 0u;
	return lastID++;
}

//template <typename T> allows the use of generics
//noexcept tells the compiler no exceptions will be thrown
template <typename T> inline ComponentID getComponentTypeID() noexcept {
	
	static ComponentID typeID = getNewComponentTypeID();
	
	return typeID;
}

//Max number of component types an entity is able to hold
//constexpr are compile-time constants, meaning their value
//is evaluated upon compilation
constexpr size_t maxComponents = 32;
constexpr size_t maxGroups = 32;


/*
* Creates a bitset type alias, then an array of this type
* that is maxComponents long. This type holds bits that 
* each represent a component of the entity
*/
using ComponentBitSet = bitset<maxComponents>;
using GroupBitSet = bitset<maxGroups>;

using ComponentArray = array<Component*, maxComponents>;

class Component {
	public:
		Entity* entity;

		/*
		* virtual means the function can be overriden
		* in derived classes. Setting it equal to 0
		* makes it abstract
		*
		* The virtual functions are overriden by each
		* component (see Components.h)
		*/
		virtual void init() {

		}

		virtual void update() {
			//Used by Entity and Manager, called in Game::Update();
		}

		virtual void draw() {
			//Used by Entity and Manager, called in Game::Update();
		}

		/*
		* Deconstructors are virtual
		* for classes that are derived
		* to prevent memory leaks
		*/
		virtual ~Component() {

		}
};


/* By storing components in a std::vector<std::unique_ptr<Component>>, 
* each entity takes exclusive responsibility for cleaning up 
* its components—no need for manual delete calls.   */
class Entity {
	private:
		Manager& manager;
		bool active = true;

		//This is a vector of unique pointers to each component
		vector<unique_ptr<Component>> components;

		ComponentArray componentArray;
		ComponentBitSet componentBitSet;
		GroupBitSet groupBitSet;

	public:
		
		//This is the only way to set a reference member
		//Using a regular constructor would try to copy the reference member
		Entity(Manager& myManager) : manager(myManager){};


		void update() {
			// Enhanced for loops that update and draw
			// components with their varying types

			//Kind of like [for (std::unique_ptr<Component> c : components)]
			//but that would copy unique ptrs and deallocate them, leaving
			//the component vectors empty, so auto is used instead.
			for(auto& c : components) c->update();
		}

		void draw() {
			for (auto& c : components) c->draw();
		}

		//Putting const ensures active is not modified
		bool isActive() const{
			return active;
		}

		void destroy() {
			active = false;
		}

		bool hasGroup(Group mGroup) {
			return groupBitSet[mGroup];
		}

		void addGroup(Group mGroup);

		void delGroup(Group mGroup) {
			groupBitSet[mGroup] = false;
		}

		template <typename T> bool hasComponent() const{

			//This determines whether the component is in the bitset
			return componentBitSet[getComponentTypeID<T>()];
		}

		//TArgs is used to represent type arguments, and they are needed in
		//both the angle brackets and the parentheses
		template <typename T, typename... TArgs>
		T& addComponent(TArgs&&... mArgs) {

			/* Constructs a new component of type t with any arguments passed through
			* using forwarding */
			T* c(new T(forward<TArgs>(mArgs)...));

			//Sets entity pointer for component to this so component know which entity it belongs to
			c->entity = this;

			//Creates a unique pointer of this component
			unique_ptr<Component> uPtr{c};

			/*Adds this unique pointer to the vector of unique pointers to components
			(second private data member) */
			components.emplace_back(move(uPtr));


			/*Our array at position lastID is set to the type*/

			componentArray[getComponentTypeID<T>()] = c;
			
			/*Our bitset at position lastID is set to true
			* so has component returns true */
			componentBitSet[getComponentTypeID<T>()] = true;

			c->init();

			//We need to return a pointer since components are pointers to types
			return *c;
		}

		template <typename T> T& getComponent() const {
			auto ptr(componentArray[getComponentTypeID<T>()]);
			
			//Cast the ptr as the type that it holds
			return *static_cast<T*>(ptr);
		}
};


class Manager {
	private:
	
		//Each entity is a pointer, so a vector of unique pointers is necessary
		vector<unique_ptr<Entity>> entities;
		
		/*An array of vectors of entity pointers, where each vector
		is stores entities of a particular group*/
		array<vector<Entity*>, maxGroups> groupedEntities;

	public:
		void update() {
			//Call the update method for each entity
			/* Each entity is a pointer, so the auto keyword is necessary
			* to determine the type */
			for (auto& e : entities) e->update();
		}

		void draw() {
			for (auto& e : entities) e->draw();
		}

		void refresh() {

			//Erase (range start, range end)
			//(range start) = remove_if(begin(entities, end(entities), condition)
				
			/* remove_if checks all elements from beginning to end that 
			* return the lambda function, which is our condition */
				
			/*The elements that meet the condition are moved to the end of the vector
			* therefore (range start) is defined */

			//The range is therefore from the first element to be remove to 
			// the end of the vector (range start, range end)
				

			for (unsigned int i = 0; i < maxGroups; ++i) {
					
				//Erase all entities in this group
				//v is the group
				auto& v (groupedEntities[i]);
					
				v.erase(remove_if(begin(v), end(v), [i](Entity* entity) {
						
					return !entity->isActive() || !entity->hasGroup(i);
					}
				), end(v));
			}

			entities.erase(remove_if (begin(entities), end(entities),
				[](const unique_ptr<Entity>& mEntity) {
					return !mEntity->isActive();
				}),
				end(entities));

		}

		void AddToGroup(Entity* mEntity, Group mGroup) {
			groupedEntities[mGroup].emplace_back(mEntity);
		}

		vector<Entity*>& GetGroup(Group mGroup) {
			return groupedEntities[mGroup];
		}

		Entity& addEntity() {
				
			//New entity pointer
			Entity* e = new Entity(*this);
			unique_ptr<Entity> uPtr {e};

			//Emplace back puts the arguments for the entity's
			//constructor inside the vector. We use the move function
			//because we cannot copy the unique pointer
			entities.emplace_back(move(uPtr));

			//Return entity pointer
			return *e;
		}
};

#endif
