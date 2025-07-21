#pragma once
#include "post.h"
#include "date.h"
using namespace std;

class Memory : public post
{
private:
    string text;
    post* originalpost;
    date postDate; 
public:
    Memory(post* originalpost, const string& text, Entity* author)
        : post(text, "", author, ""), originalpost(originalpost), text(text)
    {
        postDate.SetDate(1, 1, 2000);
    }
    string GetText() const { return text; }
    date GetDate() const override {
        return postDate;
    }
    void SetDate(int d, int m, int y) { 
        postDate.SetDate(d, m, y); 
    }
    post* getoriginalpost() const { return originalpost; }
    void setoriginalpost(post* newPost) { originalpost = newPost; }
    ~Memory() = default;
};
