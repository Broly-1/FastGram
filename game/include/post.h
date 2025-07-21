#pragma once
#include <iostream>
#include "entity.h"
#include "date.h"
#include "comment.h"
#include "user.h"
using namespace std;
class post : public Entity {
private:
	string text;
	Entity* author;
	string activitystatus;
	date postDate;
	user** likedusers; 
	int likedusercount;
	Comment comments[10];
	int commentcount = 0; 
public:
	post(const string& text = "", const string& id = "", Entity* author = nullptr, const string& activitystatus = "")
		: Entity(id), text(text), author(author), activitystatus(activitystatus) {
		postDate.SetDate(1, 1, 2000); 
	
		for (int i = 0; i < 10; i++) {
			comments[i] = Comment("", nullptr);
		}
		commentcount = 0;
		likedusercount = 0;
		likedusers = new user * [10]; 

	}
	post(const post& other) : Entity(other.ID), text(other.text), author(other.author), activitystatus(other.activitystatus) {
		postDate = other.postDate;

		commentcount = other.commentcount;
		for (int i = 0; i < commentcount; i++) {
			comments[i] = other.comments[i];
		}
		likedusercount = other.likedusercount;
		likedusers = new user * [10];
		for (int i = 0; i < likedusercount; i++) {
			likedusers[i] = other.likedusers[i];
		}

	}
	post& operator=(const post& other) {
		if (this != &other) {
			ID = other.ID;
			text = other.text;
			author = other.author;
			activitystatus = other.activitystatus;
			postDate = other.postDate;
			commentcount = other.commentcount;
			for (int i = 0; i < commentcount; i++) {
				comments[i] = other.comments[i];
			}
			likedusercount = other.likedusercount;
			likedusers = new user * [10]; 
			for (int i = 0; i < likedusercount; i++) {
				likedusers[i] = other.likedusers[i];
			}

		}
		return *this;
	}

	bool operator ==(const post& other) const { return ID == other.ID; }
	~post() override = default;
	string getID() const override { return ID; }
	string GetText() const { return text; }
	void SetText(const string& newText) { text = newText; }
	Entity* GetAuthor() const { return author; }
	void SetAuthor(Entity* newAuthor) { author = newAuthor; }
	string GetActivityStatus() const { return activitystatus; }
	void SetActivityStatus(const string& newStatus) { activitystatus = newStatus; }
	virtual void SetDate(int d, int m, int y) { postDate.SetDate(d, m, y); }
	virtual date GetDate() const { return postDate; }
	virtual Comment* GetComments() { return comments; }
	virtual int GetCommentCount() const { return commentcount; }
	virtual void AddComment(const Comment& comment) {
		if (commentcount < 10) {
			comments[commentcount++] = comment;
		}
		else {
			cout << "Comment limit reached for this post!" << endl;
		}
	}
	virtual void LikePost(user* user) {
		// Check if user already liked the post
		for (int i = 0; i < likedusercount; i++) {
			if (likedusers[i] == user) {
				cout << "You already liked this post!" << endl;
				return;
			}
		}

		// Add user if like limit not reached
		if (likedusercount < 10) {
			likedusers[likedusercount++] = user;
			cout << "You liked the post!" << endl;
		}
		else {
			cout << "Like limit reached for this post!" << endl;
		}
	}

	virtual int GetLikeCount() const {
		return likedusercount;
	}

	virtual user* GetLikeUser(int index) const {
		if (index >= 0 && index < likedusercount)
			return likedusers[index];
		return nullptr;
	}

	
};