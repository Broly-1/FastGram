#pragma once
#include <string>
#include <iostream>
class Entity {
protected:
    std::string ID;
public:
    Entity(const std::string& id = "") : ID(id) {}
	Entity(const Entity& other) : ID(other.ID) {}
	Entity& operator=(const Entity& other) {
		if (this != &other) {
			ID = other.ID;
		}
		return *this;
	}
	bool operator==(const Entity& other) const { return ID == other.ID; }

    virtual ~Entity() = default;
    virtual std::string getID() const = 0;
};