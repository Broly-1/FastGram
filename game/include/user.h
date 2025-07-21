#pragma once
#include "entity.h"
#include <string>
#include <iostream>
#include "page.h"
using namespace std;
class user : public Entity
{
private:
	string name;
	user** friends;
	page** likedpages; 
	int likedpagecount;
	int friendcount;
public:
	user(const string& name = "", const string& id = "") : Entity(id), name(name) {
		friends = new user * [100];
		friendcount = 0;
		likedpagecount = 0;
		likedpages = new page * [10]; 
	}
	user(const user& other) : Entity(other.ID), name(other.name) {
		friendcount = other.friendcount;
		friends = new user * [100];
		for (int i = 0; i < friendcount; i++) {
			friends[i] = other.friends[i];
		}
		likedpagecount = other.likedpagecount;
		likedpages = new page * [10];
		for (int i = 0; i < likedpagecount; i++) {
			likedpages[i] = other.likedpages[i];
		}


	}	
	user& operator=(const user& other) {
		if (this != &other) {
			ID = other.ID;
			name = other.name;
			friendcount = other.friendcount;
			for (int i = 0; i < friendcount; i++) {
				friends[i] = other.friends[i];
			}
			likedpagecount = other.likedpagecount;
			for (int i = 0; i < likedpagecount; i++) {
				likedpages[i] = other.likedpages[i];
			}

		}

		return *this;
	}
	void AddFriend(user* newFriend) {
		if (friendcount < 100) {
			friends[friendcount++] = newFriend;
		}
	}
	void RemoveFriend(user* friendToRemove) {
		for (int i = 0; i < friendcount; i++) {
			if (friends[i] == friendToRemove) {
				for (int j = i; j < friendcount - 1; j++) {
					friends[j] = friends[j + 1];
				}
				friendcount--;
				break;
			}
		}
	}
	int GetFriendCount() const { return friendcount; }
	user* GetFriend(int index) const {
		if (index >= 0 && index < friendcount) {
			return friends[index];
		}
		return nullptr;
	}
	void likepage(page* p) {
		if (likedpagecount < 10) {
			likedpages[likedpagecount++] = p;
		}
	}

	int GetLikedPageCount() const { return likedpagecount; }
	page* GetLikedPage(int index) const {
		if (index >= 0 && index < likedpagecount) {
			return likedpages[index];
		}
		return nullptr;
	}
	~user()
	{
		delete[] friends;
	}
	string getID() const override { return ID; }
	string GetName() const { return name; }
	void SetName(const string& newName) { name = newName; }
};