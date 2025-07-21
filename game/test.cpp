//#pragma once
//#include "user.h"
//#include <string>
//#include "post.h"
//#include "date.h"
//#include "page.h"
//#include "memory.h"
//#include "fstream"
//#include "raylib.h"
//#define RAYGUI_IMPLEMENTATION
//#include "raygui.h"
//#include <vector>
//class socialNetworkApp {
//private:
//	user** users;
//	post** posts;
//	page** pages;
//	int pagecount;
//	int postcount;
//	user* currentuser;
//	int usercount;
//	date currentDate;
//	Memory** memories;
//	int memorycount;
//	// Array of memory posts
//public:
//	int currentPostIndex = 0;
//	std::vector<post*> feedPosts;
//	bool feedInitialized = false;
//
//	enum Screen { START, LOGIN, REGISTER, MAINMENU, VIEWHOME } currentScreen = START;
//	socialNetworkApp() : usercount(0), currentuser(NULL) {
//		users = new user * [100];
//		posts = new post * [100];
//		pages = new page * [100];
//		memories = new Memory * [100];
//		memorycount = 0;
//		pagecount = 0;
//		postcount = 0;
//		currentDate.SetDate(1, 1, 2000); // Default date
//		// Initialize users and posts arrays
//		// simplicity
//		// Assuming a maximum of 100 users for
//	}
//	~socialNetworkApp() {
//		for (int i = 0; i < usercount; i++) {
//			delete users[i];
//		}
//		delete[] users;
//		for (int i = 0; i < postcount; i++) {
//			delete posts[i];
//		}
//		delete[] posts;
//		for (int i = 0; i < pagecount; i++) {
//			delete pages[i];
//		}
//		for (int i = 0; i < memorycount; i++) {
//			delete memories[i];
//		}
//		delete[] pages;
//	}
//	user* FindUserByName(string name) {
//		for (int i = 0; i < usercount; i++) {
//			bool found = false;
//			string str = users[i]->GetName();
//			for (int j = 0; j < str.length(); j++) {
//				if (str[j] == name[j]) {
//					found = true;
//				}
//				else {
//					found = false;
//					break;
//				}
//			}
//			if (found) {
//				return users[i];
//			}
//		}
//		return NULL;
//	}
//	user* FindUserByID(string id) {
//		for (int i = 0; i < usercount; i++) {
//			if (users[i]->getID() == id) {
//				return users[i];
//			}
//		}
//		return NULL;
//	}
//	post* FindPostByID(string id) {
//		for (int i = 0; i < postcount; i++) {
//			if (posts[i]->getID() == id) {
//				return posts[i];
//			}
//		}
//		return NULL;
//	}
//	void saveUserToFile(user* newUser) {
//		ofstream fout("users.txt", ios::app); // append mode
//		if (fout.is_open()) {
//			fout << newUser->getID() << " " << newUser->GetName() << endl;
//			fout.close();
//		}
//		else {
//			cout << "Error opening users.txt for writing." << endl;
//		}
//	}
//	void savePageToFile(page* newPage) {
//		ofstream fout("pages.txt", ios::app); // append mode
//		if (fout.is_open()) {
//			fout << newPage->getID() << " " << newPage->GetName() << endl;
//			fout.close();
//		}
//		else {
//			cout << "Error opening pages.txt for writing." << endl;
//		}
//	}
//	void loadDataFromFiles() {
//		// Load Users
//		ifstream userFile("users.txt");
//		if (userFile.is_open()) {
//			string id, name;
//			while (userFile >> id) {
//				userFile.ignore(); // skip space
//				getline(userFile, name);
//				user* newUser = new user(name, id);
//				users[usercount++] = newUser;
//			}
//			userFile.close();
//		}
//		else {
//			cout << "Warning: users.txt not found. No users loaded." << endl;
//		}
//
//		// Load Pages
//		ifstream pageFile("pages.txt");
//		if (pageFile.is_open()) {
//			string id, title;
//			while (pageFile >> id) {
//				pageFile.ignore(); // skip space
//				getline(pageFile, title);
//				page* newPage = new page(title, id, nullptr); // Owner is set later manually if needed
//				pages[pagecount++] = newPage;
//			}
//			pageFile.close();
//		}
//		else {
//			cout << "Warning: pages.txt not found. No pages loaded." << endl;
//		}
//	}
//	void run()
//	{
//		loadDataFromFiles(); // Load previous users and pages
//
//		InitWindow(800, 600, "Social Network App");
//		SetTargetFPS(60);
//
//
//		// Text fields
//		char username[50] = "";
//		bool loggedIn = false;
//		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(SKYBLUE));       // Default color
//		GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(DARKBLUE));  // Hover
//		GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(BLUE));   // Click
//		GuiSetStyle(DEFAULT, TEXT_SIZE, 24); // Default is usually 10
//
//
//		GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
//		GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(YELLOW));
//		GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(RED));
//
//		while (!WindowShouldClose()) // Main game loop
//		{
//
//			BeginDrawing();
//			ClearBackground(RAYWHITE);
//
//			switch (currentScreen)
//			{
//			case START:
//			{
//				// Background and Header
//				ClearBackground(RAYWHITE);
//				DrawText("FastGram", 300, 120, 40, BLACK);
//				// Login and Register Buttons
//				if (GuiButton(Rectangle{ 250, 240, 300, 50 }, "Log in"))
//				{
//					currentScreen = LOGIN;
//				}
//				if (GuiButton(Rectangle{ 250, 310, 300, 50 }, "Sign up"))
//				{
//					currentScreen = REGISTER;
//				}
//
//				// Footer or Exit
//				DrawText("@ 2025 FastGram", 320, 500, 18, GRAY);
//				if (GuiButton(Rectangle{ 10, 10, 30, 30 }, "x"))
//				{
//					CloseWindow();
//					return;
//				}
//			}
//			break;
//
//			case LOGIN:
//				DrawText("Enter username to login:", 270, 100, 20, DARKGRAY);
//				GuiTextBox(Rectangle{ 250, 150, 300, 40 }, username, 50, true);
//
//				if (GuiButton(Rectangle{ 300, 250, 200, 50 }, "Login"))
//				{
//					currentuser = FindUserByName(string(username));
//					if (currentuser != NULL)
//					{
//						currentScreen = MAINMENU;
//						loggedIn = true;
//					}
//					else
//					{
//						// If not found, redirect to register
//						currentScreen = REGISTER;
//					}
//				}
//				break;
//
//			case REGISTER:
//			{
//
//				static bool showError = false;
//				static bool showGoToLogin = false;
//
//				DrawText("Enter username to register:", 255, 100, 20, DARKGRAY);
//				GuiTextBox(Rectangle{ 250, 150, 300, 40 }, username, 50, true);
//
//				if (GuiButton(Rectangle{ 300, 250, 200, 50 }, "Register"))
//				{
//					string name(username);
//					user* existingUser = FindUserByName(name);
//
//					if (existingUser != NULL)
//					{
//						showError = true;
//						showGoToLogin = true;
//					}
//					else
//					{
//						string id = "u" + to_string(usercount + 1);
//						user* newUser = new user(name, id);
//						users[usercount++] = newUser;
//						saveUserToFile(newUser);
//						currentuser = newUser;
//						showError = false;
//						showGoToLogin = true;
//						DrawText("Registered successfully", 250, 400, 30, YELLOW);
//						// go to login after successful registration
//					}
//				}
//
//				if (showError)
//				{
//					DrawText("User already exists!", 295, 320, 20, RED);
//				}
//
//				if (showGoToLogin)
//				{
//					if (GuiButton(Rectangle{ 300, 370, 200, 40 }, "Go to Login"))
//					{
//						currentScreen = LOGIN;
//						showError = false;
//						showGoToLogin = false;
//					}
//				}
//			}
//			break;
//
//			case MAINMENU:
//				DrawText(TextFormat("user: %s", currentuser->GetName().c_str()), 640, 575, 20, DARKGRAY);
//				if (GuiButton(Rectangle{ 300, 150, 200, 50 }, "Create Post"))
//				{
//					// Call your createPost()
//					createpost();
//				}
//				if (GuiButton(Rectangle{ 300, 290, 200, 50 }, "Logout"))
//				{
//					currentuser = NULL;
//					currentScreen = START;
//				}
//				if (GuiButton(Rectangle{ 300, 220, 200, 50 }, "View Home")) {
//					currentScreen = VIEWHOME;
//				}
//
//				break;
//			case VIEWHOME:
//			{
//				if (currentuser)
//					DrawText(TextFormat("user: %s", currentuser->GetName().c_str()), 640, 575, 20, DARKGRAY);
//
//				// Back to menu
//				if (GuiButton({ 10, 10, 100, 30 }, "< Back")) {
//					currentScreen = MAINMENU;
//					feedInitialized = false;  // Reset feed so it reloads next time
//				}
//
//				viewhome(); // Call the refactored view
//			}
//			break;
//
//
//			}
//
//
//			EndDrawing();
//		}
//
//		CloseWindow();
//	}
//
//	void createpost()
//	{
//		string content;
//		int choice;
//		string activitystatus = "";
//		do {
//			cout << "Whats your status? 1. feeling, 2. thinking about, 3. Making, 4. Celebrating" << endl;
//			cin >> choice;
//		} while (choice < 1 || choice > 4); // Ensure valid choice
//
//		switch (choice) {
//		case 1:
//		{
//			cout << "What are you feeling?" << endl;
//			string feeling;
//			cin >> feeling;
//			activitystatus += "feeling " + feeling + " ";
//			break;
//		}
//		case 2:
//		{
//			cout << "What are you thinking about?" << endl;
//			string thought;
//			cin >> thought;
//			activitystatus += "thinking about " + thought + " ";
//			break;
//		}
//		case 3:
//		{
//			cout << "What are you making?" << endl;
//			string making;
//			cin >> making;
//			activitystatus += "making " + making + " ";
//
//			break;
//		}
//		case 4:
//		{
//			cout << "What are you celebrating?" << endl;
//			string celebration;
//			cin >> celebration;
//			activitystatus += "celebrating " + celebration + " ";
//			break;
//		}
//		};
//		string text;
//		cout << "Enter your post text: ";
//		cin.ignore();
//		getline(cin, text);
//		string id = "p";
//		id += to_string(postcount + 1);
//		post* newPost = new post(text, id, currentuser, activitystatus);
//		posts[postcount++] = newPost;
//		newPost->SetDate(currentDate.GetDay(), currentDate.GetMonth(), currentDate.GetYear());
//		cout << "Post created successfully!" << endl;
//		newPost->DisplayPost();
//	}
//	bool isRecent(date& postDate) {
//		return (postDate.GetYear() == currentDate.GetYear() &&
//			postDate.GetMonth() == currentDate.GetMonth() &&
//			(postDate.GetDay() == currentDate.GetDay() ||
//				postDate.GetDay() == (currentDate.GetDay() - 1)));
//	}
//
//	void viewhome() {
//		// Load feed only once when VIEWHOME screen starts
//		if (!feedInitialized) {
//			feedPosts.clear();
//
//			// Get friend's posts
//			for (int i = 0; i < currentuser->GetFriendCount(); i++) {
//				user* friendUser = currentuser->GetFriend(i);
//				for (int j = 0; j < postcount; j++) {
//					if (posts[j]->GetAuthor() == friendUser && isRecent(posts[j]->GetDate())) {
//						feedPosts.push_back(posts[j]);
//					}
//				}
//			}
//
//			// Get liked pages' posts
//			for (int i = 0; i < currentuser->GetLikedPageCount(); i++) {
//				page* likedPage = currentuser->GetLikedPage(i);
//				for (int j = 0; j < postcount; j++) {
//					if (posts[j]->GetAuthor() == likedPage && isRecent(posts[j]->GetDate())) {
//						feedPosts.push_back(posts[j]);
//					}
//				}
//			}
//
//			feedInitialized = true;
//			currentPostIndex = 0;
//		}
//
//		if (feedPosts.empty()) {
//			DrawText("No recent posts to show.", 250, 280, 20, DARKGRAY);
//
//			return;
//		}
//
//		// Clamp index
//		if (currentPostIndex < 0) currentPostIndex = 0;
//		if (currentPostIndex >= feedPosts.size()) currentPostIndex = feedPosts.size() - 1;
//
//		post* post = feedPosts[currentPostIndex];
//
//		// Arrows to navigate
//		if (GuiButton({ 20, 300, 40, 40 }, "<") && currentPostIndex > 0) currentPostIndex--;
//		if (GuiButton({ 740, 300, 40, 40 }, ">") && currentPostIndex < feedPosts.size() - 1) currentPostIndex++;
//
//		// Draw Post Card
//		DrawRectangle(100, 150, 600, 300, LIGHTGRAY);
//
//		std::string authorName = "Unknown";
//		if (Entity* author = post->GetAuthor()) {
//			if (user* u = dynamic_cast<user*>(author)) authorName = u->GetName();
//			else authorName = author->getID();
//		}
//
//		std::string activity = post->GetActivityStatus().empty() ? "is sharing" : "is " + post->GetActivityStatus();
//		DrawText((authorName + " " + activity).c_str(), 120, 170, 20, BLACK);
//		DrawText(post->GetText().c_str(), 120, 210, 18, DARKGRAY);
//
//		if (GuiButton({ 120, 350, 80, 30 }, "Like")) {
//			// Like logic
//		}
//		if (GuiButton({ 220, 350, 100, 30 }, "Comment")) {
//			// Comment logic
//		}
//	}
//
//
//};