#pragma once
#include <iostream>
#include "entity.h"
using namespace std;
class page : public Entity
{
private:
	string name;
	Entity* owner;
public:
	page(const string& name = "", const string& id = "", Entity* owner = nullptr) : Entity(id), name(name), owner(owner) {
	}
	page(const page& other) : Entity(other.ID), name(other.name), owner(other.owner) {
	}
	page& operator=(const page& other) {
		if (this != &other) {
			ID = other.ID;
			name = other.name;
			owner = other.owner;
		}
		return *this;
	}

	string getID() const override { return ID; }


	string GetName() const { return name; }
	void SetName(const string& newName) { name = newName; }
	Entity* GetOwner() const { return owner; }
	void SetOwner(Entity* newOwner) { owner = newOwner; }
};