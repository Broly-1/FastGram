#pragma once
#include "entity.h"
#include <string>
using namespace std;
class Comment {
private:
    string content;
    Entity* owner;  
public:
    Comment(string c = "", Entity* o = nullptr) : content(c), owner(o) {}
	Comment(const Comment& other) : content(other.content), owner(other.owner) {}
	Comment& operator=(const Comment& other) {
		if (this != &other) {
			content = other.content;
			owner = other.owner;
		}
		return *this;
	}

    Entity* GetOwner() const { return owner; }
    string GetContent() const { return content; }

};