#pragma once
#include "user.h"
#include <string>
#include "post.h"
#include "date.h"
#include "page.h"
#include "memory.h"
#include "fstream"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
class socialNetworkApp {
private:
	user** users;
	post** posts;
	page** pages;
	Memory** memories;
	int pagecount;
	int postcount;
	int usercount;
	int memorycount;
	user* currentuser;
	date currentDate;
	post* feedPostsArr[100];    // max 100 posts in a feed
public:
	int feedCount = 0;
	user* friendProfile = nullptr;
	page* selectedPage = nullptr;  // This is used to track the selected page to view or post
	int currentPostIndex = 0;
	bool feedInitialized = false;
	Entity* postTarget = nullptr;  // Set this before going to CREATEPOST screen

	enum Screen { 
		START,
		LOGIN, 
		REGISTER, 
		MAINMENU,
		SETDATE,
		VIEWHOME,
		ADDFRIEND,
		CREATEPOST,
		PROFILESCREEN,
		PAGESMENU,
		CREATEPAGE,
		VIEWPAGES,
		OWNEDPAGES,
		VIEWFRIENDS,
		FRIENDPROFILE,
		VIEWPAGE,
		POSTTOPAGE
	}currentScreen = START;

	char dayInput[3] = "1";
	char monthInput[3] = "1";
	char yearInput[5] = "2025";

	// Flags to track which date input field is being edited
	bool dayEdit = false;
	bool monthEdit = false;
	bool yearEdit = false;

	socialNetworkApp() : usercount(0), currentuser(NULL) {
		users = new user * [100];
		posts = new post * [100];
		pages = new page * [100];
		memories = new Memory * [100];
		memorycount = 0;
		pagecount = 0;
		postcount = 0;
		currentDate.SetDate(1, 1, 2000);
		
	}
	~socialNetworkApp() {
		for (int i = 0; i < usercount; i++) {
			delete users[i];
		}
		delete[] users;
		for (int i = 0; i < postcount; i++) {
			delete posts[i];
		}
		delete[] posts;
		for (int i = 0; i < pagecount; i++) {
			delete pages[i];
		}
		delete[] pages;
		for (int i = 0; i < memorycount; i++) {
			delete memories[i];
		}
		delete[] memories;
	}
	user* FindUserByName(string name) {
		for (int i = 0; i < usercount; i++) {
			bool found = false;
			string str = users[i]->GetName();
			for (int j = 0; j < str.length(); j++) {
				if (str[j] == name[j]) {
					found = true;
				}
				else {
					found = false;
					break;
				}
			}
			if (found) {
				return users[i];
			}
		}
		return NULL;
	}
	user* FindUserByID(string id) {
		for (int i = 0; i < usercount; i++) {
			if (users[i]->getID() == id) {
				return users[i];
			}
		}
		return NULL;
	}
	post* FindPostByID(string id) {
		for (int i = 0; i < postcount; i++) {
			if (posts[i]->getID() == id) {
				return posts[i];
			}
		}
		return NULL;
	}
	void saveUserToFile(user* newUser) {
		ofstream fout("users.txt", ios::app); // append mode
		if (fout.is_open()) {
			fout << newUser->getID() << " " << newUser->GetName() << endl;
			fout.close();
		}
		else {
			cout << "Error opening users.txt for writing." << endl;
		}
	}
	void savePageToFile(page* newPage) {
		ofstream fout("pages.txt", ios::app); 
		if (fout.is_open()) {
			fout << newPage->getID() << " " << newPage->GetName() << " " << newPage->GetOwner()->getID() << endl;
			fout.close();
		}
		else {
			cout << "Error opening pages.txt for writing." << endl;
		}
	}
	void loadDataFromFiles() {
		// Load Users
		ifstream userFile("users.txt");
		if (userFile.is_open()) {
			string id, name;
			while (userFile >> id) {
				userFile.ignore(); // skip space
				getline(userFile, name);
				user* newUser = new user(name, id);
				users[usercount++] = newUser;
			}
			userFile.close();
		}
		else {
			cout << "Warning: users.txt not found. No users loaded." << endl;
		}

		// Load Pages
		ifstream pageFile("pages.txt");
		if (pageFile.is_open()) {
			string id, title, ownerID;
			while (pageFile >> id) {
				pageFile.ignore(); // skip space
				getline(pageFile, title, ' '); // read title until space
				pageFile >> ownerID;           // read owner ID

				user* owner = FindUserByID(ownerID);
				if (owner == nullptr) {
					cout << "Warning: Owner with ID " << ownerID << " not found. Page skipped." << endl;
					continue;
				}

				page* newPage = new page(title, id, owner);
				pages[pagecount++] = newPage;

				owner->likepage(newPage);
			}
			pageFile.close();
		}
		else {
			cout << "Warning: pages.txt not found. No pages loaded." << endl;
		}
	}

	void run()
	{
		loadDataFromFiles(); 

		InitWindow(800, 600, "Social Network App");
		SetTargetFPS(60);

		 Image imgLight = LoadImage("resources/1.png");
         Image imgDark = LoadImage("resources/2.png");
		 Image imgPages = LoadImage("resources/3.png");

         Texture2D lightTexture = LoadTextureFromImage(imgLight);
         Texture2D darkTexture = LoadTextureFromImage(imgDark);
		 Texture2D pagesTexture = LoadTextureFromImage(imgPages);

		 UnloadImage(imgPages);
         UnloadImage(imgLight);
         UnloadImage(imgDark);
		// Text fields
		char username[50] = "";
		bool loggedIn = false;
		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(SKYBLUE));       // Default color
		GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(DARKBLUE));  // Hover
		GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(BLUE));   // Click
		GuiSetStyle(DEFAULT, TEXT_SIZE, 24); 


		GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
		GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(YELLOW));
		GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(RED));

		while (!WindowShouldClose()) // Main game loop
		{

			BeginDrawing();
			ClearBackground(RAYWHITE);

			switch (currentScreen)
			{
			case START:
			{
				showstartscreen();
			}
			break;
			case LOGIN:
			{
				ClearBackground(RAYWHITE);

				// Dimensions
				float panelX = 150;
				float panelY = 140;
				int panelWidth = 500;
				int panelHeight = 280;

				// Draw login panel
				DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(LIGHTGRAY, 0.3f));
				DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, GRAY);

				// Title
				DrawText("User Login", 310, 80, 36, BLACK);
				DrawLine(panelX + 20, panelY - 10, panelX + panelWidth - 20, panelY - 10, GRAY);

				// Label
				DrawText("Username:", panelX + 40, panelY + 60, 22, DARKGRAY);

				// Text box
				static bool usernameEdit = true;
				if (GuiTextBox(Rectangle{ panelX + 170, panelY + 55, 250, 35 }, username, 50, usernameEdit)) {
					usernameEdit = !usernameEdit;
				}

				// Login button
				if (GuiButton(Rectangle{ panelX + (panelWidth - 180) / 2, panelY + 130, 180, 45 }, "Login")) {
					currentuser = FindUserByName(string(username));
					if (currentuser != NULL) {
						currentScreen = MAINMENU;
						loggedIn = true;
					}
					else {
						currentScreen = REGISTER;
					}
				}

				// Back button
				if (GuiButton({ 20, 20, 80, 30 }, "< Back")) {
					currentScreen = START;
					usernameEdit = false;
				}
			}
			break;
			case REGISTER:
			{

				static bool showError = false;
				static bool showGoToLogin = false;

				DrawText("Enter username to register:", 255, 100, 20, DARKGRAY);
				GuiTextBox(Rectangle{ 250, 150, 300, 40 }, username, 50, true);

				if (GuiButton(Rectangle{ 300, 250, 200, 50 }, "Register"))
				{
					string name(username);
					user* existingUser = FindUserByName(name);

					if (existingUser != NULL)
					{
						showError = true;
						showGoToLogin = true;
					}
					else
					{
						string id = "u" + to_string(usercount + 1);
						user* newUser = new user(name, id);
						users[usercount++] = newUser;
						saveUserToFile(newUser);
						currentuser = newUser;
						showError = false;
						showGoToLogin = true;
						DrawText("Registered successfully", 250, 400, 30, YELLOW);
						// go to login after successful registration
					}
				}

				if (showError)
				{
					DrawText("User already exists!", 295, 320, 20, RED);
				}

				if (showGoToLogin)
				{
					if (GuiButton(Rectangle{ 300, 370, 200, 40 }, "Go to Login"))
					{
						currentScreen = LOGIN;
						showError = false;
						showGoToLogin = false;
					}
				}
			}
			break;
			case SETDATE:
			{
				ClearBackground(RAYWHITE);

				// Draw title header
				DrawText("Set Current Date", 26032, 60, 32, BLACK);
				DrawLine(250, 95, 550, 95, GRAY);

				// Box background
				DrawRectangle(200, 120, 400, 200, Fade(LIGHTGRAY, 0.3f));
				DrawRectangleLines(200, 120, 400, 200, GRAY);

				// Input fields
				DrawText("Day:", 220, 150, 22, DARKGRAY);
				if (GuiTextBox({ 280, 145, 50, 35 }, dayInput, 3, dayEdit)) {
					dayEdit = true;
					monthEdit = yearEdit = false;
				}

				DrawText("Month:", 340, 150, 22, DARKGRAY);
				if (GuiTextBox({ 430, 145, 50, 35 }, monthInput, 3, monthEdit)) {
					monthEdit = true;
					dayEdit = yearEdit = false;
				}

				DrawText("Year:", 220, 200, 22, DARKGRAY);
				if (GuiTextBox({ 280, 195, 100, 35 }, yearInput, 5, yearEdit)) {
					yearEdit = true;
					dayEdit = monthEdit = false;
				}

				// Confirm Button
				if (GuiButton({ 310, 250, 180, 40 }, "Set Date")) {
					int d = atoi(dayInput);
					int m = atoi(monthInput);
					int y = atoi(yearInput);

					if (d >= 1 && d <= 31 && m >= 1 && m <= 12 && y >= 1900 && y <= 2100) {
						currentDate.SetDate(d, m, y);
						currentScreen = START;

						dayEdit = monthEdit = yearEdit = false;
					}
					else {
						DrawText("Invalid date entered!", 280, 300, 20, RED);
					}
				}

				// Back button
				if (GuiButton({ 20, 20, 80, 30 }, "< Back")) {
					currentScreen = START;
					dayEdit = monthEdit = yearEdit = false;
				}
			}
			break;
			case MAINMENU:
			{
				Vector2 mouse = GetMousePosition();

				Rectangle btnHome = { 190, 180, 188.5, 254.3 };      // Home box
				Rectangle btnPages = { 420, 180, 188.5, 254.3 };     // Pages box
				Rectangle btnAddFriend = { 5, 5, 50, 50 };     // Top left
				Rectangle btnFriends = { 745, 5, 50, 50 };      // Top right
				Rectangle btnLogout = { 15, 550, 40, 40 };       // Bottom left
				Rectangle btnCreatePost = { 380, 550, 40, 40 };  // Bottom center
				Rectangle btnProfile = { 745, 550, 40, 40 };     // Bottom right

				bool hovering = CheckCollisionPointRec(mouse, btnHome) || CheckCollisionPointRec(mouse, btnPages) ||
					CheckCollisionPointRec(mouse, btnAddFriend) || CheckCollisionPointRec(mouse, btnFriends) ||
					CheckCollisionPointRec(mouse, btnLogout) || CheckCollisionPointRec(mouse, btnCreatePost) ||
					CheckCollisionPointRec(mouse, btnProfile);

				DrawTexture(lightTexture, 0, 0, WHITE); 
				if (CheckCollisionPointRec(mouse, btnHome)) DrawRectangleRec(btnHome, Fade(YELLOW, 0.3f));
				if (CheckCollisionPointRec(mouse, btnPages)) DrawRectangleRec(btnPages, Fade(ORANGE, 0.3f));
				if (CheckCollisionPointRec(mouse, btnAddFriend)) DrawRectangleRec(btnAddFriend, Fade(SKYBLUE, 0.3f));
				if (CheckCollisionPointRec(mouse, btnFriends)) DrawRectangleRec(btnFriends, Fade(SKYBLUE, 0.3f));
				if (CheckCollisionPointRec(mouse, btnLogout)) DrawRectangleRec(btnLogout, Fade(SKYBLUE, 0.3f));
				if (CheckCollisionPointRec(mouse, btnCreatePost)) DrawRectangleRec(btnCreatePost, Fade(SKYBLUE, 0.3f));
				if (CheckCollisionPointRec(mouse, btnProfile)) DrawRectangleRec(btnProfile, Fade(SKYBLUE, 0.3f));

				// Check for clicks
				if (CheckCollisionPointRec(mouse, btnHome) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					currentScreen = VIEWHOME;
				}
				else if (CheckCollisionPointRec(mouse, btnPages) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					currentScreen = PAGESMENU;
				}
				else if (CheckCollisionPointRec(mouse, btnAddFriend) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					currentScreen = ADDFRIEND;
				}
				else if (CheckCollisionPointRec(mouse, btnFriends) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					currentScreen = VIEWFRIENDS;
				}
				else if (CheckCollisionPointRec(mouse, btnLogout) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					currentuser = NULL;
					currentScreen = START;
				}
				else if (CheckCollisionPointRec(mouse, btnCreatePost) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					postTarget = currentuser;
					currentScreen = CREATEPOST;
				}
				else if (CheckCollisionPointRec(mouse, btnProfile) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					currentScreen = PROFILESCREEN;
				}

				if (currentuser)
					DrawText(currentuser->GetName().c_str(), 720, 510, 20, DARKGRAY);
			}
			break;
			case VIEWHOME:
			{
				if(currentuser)
				DrawText(TextFormat("user: %s", currentuser->GetName().c_str()), 640, 575, 20, DARKGRAY);

				// Back to menu
				if (GuiButton({ 10, 10, 100, 30 }, "< Back")) {
					currentScreen = MAINMENU;
					feedInitialized = false;  // Reset feed so it reloads next time
				}

				viewhome(); 
			}
			break;
			case ADDFRIEND:
			{
				static char friendInput[50] = "";
				static bool inputEdit = true;
				static string message = "";

				// Background
				ClearBackground(RAYWHITE);

				// panel background
				DrawRectangleRounded({ 180, 130, 440, 250 }, 0.1f, 10, Fade(LIGHTGRAY, 0.4f));
				DrawRectangleLinesEx({ 180, 130, 440, 250 }, 2, GRAY);

				// Heading
				DrawText("Add a New Friend", 280, 140, 28, DARKBLUE);

				// Label
				DrawText("Enter username or ID:", 200, 190, 20, DARKGRAY);

				// Input box
				Rectangle inputBox = { 200, 220, 400, 40 };
				if (CheckCollisionPointRec(GetMousePosition(), inputBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					inputEdit = true;
				}

				GuiTextBox(inputBox, friendInput, 50, inputEdit);

				// Add Friend button
				if (GuiButton({ 300, 280, 200, 40 }, "Add Friend")) {
					string nameOrId(friendInput);
					user* found = FindUserByName(nameOrId);
					if (!found) found = FindUserByID(nameOrId);

					if (!found) {
						message = "Error: User not found.";
					}
					else if (found == currentuser) {
						message = "Warning: You cannot add yourself.";
					}
					else {
						bool alreadyFriend = false;
						for (int i = 0; i < currentuser->GetFriendCount(); i++) {
							if (currentuser->GetFriend(i) == found) {
								alreadyFriend = true;
								break;
							}
						}

						if (alreadyFriend) {
							message = "Warning: Already friends.";
						}
						else {
							currentuser->AddFriend(found);
							found->AddFriend(currentuser);
							message = "Success: Friend added successfully.";
						}
					}

					inputEdit = false;
				}


				// Message feedback
				if (!message.empty()) {
					Color msgColor = GRAY;
					if (message.rfind("Success:", 0) == 0) msgColor = DARKGREEN;
					else if (message.rfind("Error:", 0) == 0) msgColor = RED;
					else if (message.rfind("Warning:", 0) == 0) msgColor = ORANGE;

					DrawText(message.c_str(), 220, 340, 20, msgColor);
				}


				// Back button
				if (GuiButton({ 10, 10, 80, 30 }, "< Back")) {
					currentScreen = MAINMENU;
					inputEdit = false;
					friendInput[0] = '\0';
					message = "";
				}
			}
			break;
			case VIEWFRIENDS:
			{
				static char searchName[50] = "";
				static bool searchBoxEditMode = false;
				static string statusMessage = "";

				ClearBackground(RAYWHITE);

				// Draw rounded card panel for layout
				DrawRectangleRounded({ 150, 60, 500, 460 }, 0.1f, 10, Fade(LIGHTGRAY, 0.4f));
				DrawRectangleLinesEx({ 150, 60, 500, 460 }, 2, GRAY);

				DrawText("Your Friends:", 180, 80, 28, DARKBLUE);

				// Draw friend names inside the panel
				int y = 130;
				for (int i = 0; i < currentuser->GetFriendCount(); i++) {
					string friendName = currentuser->GetFriend(i)->getID() + " - " + currentuser->GetFriend(i)->GetName();
					DrawText(friendName.c_str(), 180, y, 24, BLACK);
					y += 30;
				}

				// Input section to view profile
				DrawText("Enter friend's name:", 180, 370, 20, DARKGRAY);

				Rectangle inputBox = { 180, 400, 440, 40 };
				if (CheckCollisionPointRec(GetMousePosition(), inputBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					searchBoxEditMode = true;
				}
				GuiTextBox(inputBox, searchName, 50, searchBoxEditMode);

				if (GuiButton({ 300, 460, 200, 40 }, "View Profile")) {
					string input(searchName);
					user* target = FindUserByName(input);

					if (target) {
						friendProfile = target;
						currentScreen = FRIENDPROFILE;
						searchName[0] = '\0';
						searchBoxEditMode = false;
						statusMessage = "";
					}
					else {
						statusMessage = "Error: Friend not found.";
					}
				}
					// Feedback
					if (!statusMessage.empty()) {
						Color color = statusMessage.rfind("Error:", 0) == 0 ? RED : GREEN;  
						DrawText(statusMessage.c_str(), 270, 520, 20, color);
					}

				// Back button
				if (GuiButton({ 10, 10, 80, 30 }, "< Back")) {
					currentScreen = MAINMENU;
					searchBoxEditMode = false;
					searchName[0] = '\0';
					statusMessage = "";
				}
			}
			break;
			case FRIENDPROFILE:
			{
				if (!friendProfile) {
					currentScreen = VIEWFRIENDS;
					break;
				}

				viewprofile(friendProfile); // Use modified function

				if (GuiButton({ 10, 10, 80, 30 }, "< Back")) {
					currentScreen = VIEWFRIENDS;
				}
			}
			break;
			case CREATEPOST:
			{
				if (postTarget) {
					createpost(postTarget); // pass post target entity
				}
			}
			break;
			case PROFILESCREEN:
			{
				if (GuiButton({ 10, 10, 30, 30 }, "<")) {
					currentScreen = MAINMENU;
					feedInitialized = false;  // Reset feed so it reloads next time
				}
				viewprofile(currentuser);
			}
			break;
			case PAGESMENU:
			{
				ClearBackground(RAYWHITE);
				DrawTexture(pagesTexture, 0, 0, WHITE);

				Vector2 mouse = GetMousePosition();

				// Define click/hover areas
				Rectangle btnCreatePage = { 60, 175, 188.5, 254.3 };
				Rectangle btnViewPages = { 305, 175, 188.5, 254.3 };
				Rectangle btnOwnedPages = { 550, 175, 188.5, 254.3 };
				Rectangle btnBackHome = { 380, 550, 40, 40 };

				// Draw hover highlights
				if (CheckCollisionPointRec(mouse, btnCreatePage)) DrawRectangleRec(btnCreatePage, Fade(SKYBLUE, 0.3f));
				if (CheckCollisionPointRec(mouse, btnViewPages)) DrawRectangleRec(btnViewPages, Fade(RED, 0.3f));
				if (CheckCollisionPointRec(mouse, btnOwnedPages)) DrawRectangleRec(btnOwnedPages, Fade(GREEN, 0.3f));
				if (CheckCollisionPointRec(mouse, btnBackHome)) DrawRectangleRec(btnBackHome, Fade(SKYBLUE , 0.3f));

				// Handle clicks
				if (CheckCollisionPointRec(mouse, btnCreatePage) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					currentScreen = CREATEPAGE;
				}
				if (CheckCollisionPointRec(mouse, btnViewPages) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					currentScreen = VIEWPAGES;
				}
				if (CheckCollisionPointRec(mouse, btnOwnedPages) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					currentScreen = OWNEDPAGES;
				}
				if (CheckCollisionPointRec(mouse, btnBackHome) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					currentScreen = MAINMENU;
				}
			}
			break;
			case CREATEPAGE:
			{
				static char pageTitle[50] = "";
				static bool pageEditMode = true;
				static string statusMessage = "";

				ClearBackground(RAYWHITE);

				// Title box
				DrawRectangleRounded({ 180, 140, 440, 250 }, 0.2f, 10, Fade(SKYBLUE, 0.2f));
				DrawRectangleRoundedLines({ 180, 140, 440, 250 }, 0.2f, 10, SKYBLUE);

				DrawText("Create New Page", 270, 160, 30, DARKBLUE);
				DrawText("Enter page title:", 230, 210, 20, DARKGRAY);

				if (GuiTextBox({ 230, 240, 340, 40 }, pageTitle, 50, pageEditMode)) {
					pageEditMode = true;
				}

				if (GuiButton({ 310, 300, 180, 40 }, "Create Page")) {
					string name(pageTitle);
					if (!name.empty()) {
						string id = "P" + to_string(pagecount + 1);
						page* newPage = new page(name, id, currentuser);
						pages[pagecount++] = newPage;
						currentuser->likepage(newPage);
						savePageToFile(newPage);
						statusMessage = "Page created successfully!";
						pageTitle[0] = '\0';
					}
					else {
						statusMessage = "Title cannot be empty.";
					}
					pageEditMode = false;
				}

				// Show feedback message
				if (!statusMessage.empty()) {
					Color msgColor = (statusMessage.find("success") != string::npos) ? DARKGREEN : RED;
					DrawText(statusMessage.c_str(), 250, 360, 20, msgColor);
				}

				// Back button
				if (GuiButton({ 10, 10, 80, 30 }, "< Back")) {
					currentScreen = PAGESMENU;
					statusMessage = "";
					pageEditMode = false;
				}
			}
			break;
			case OWNEDPAGES:
			{
				static char pageSearch[50] = "";
				static bool searchEdit = false;
				static string statusMessage = "";

				ClearBackground(RAYWHITE);

				// Draw background panel
				DrawRectangleRounded({ 150, 60, 500, 460 }, 0.1f, 10, Fade(LIGHTGRAY, 0.4f));
				DrawRectangleLinesEx({ 150, 60, 500, 460 }, 2, GRAY);

				DrawText("Your Owned Pages:", 180, 80, 28, DARKBLUE);

				int y = 130;
				bool hasOwned = false;
				for (int i = 0; i < pagecount; i++) {
					if (pages[i]->GetOwner() == currentuser) {
						hasOwned = true;
						string info = pages[i]->getID() + " - " + pages[i]->GetName();
						DrawText(info.c_str(), 180, y, 24, BLACK);
						y += 30;
					}
				}

				if (!hasOwned) {
					DrawText("You don't own any pages yet.", 180, y, 24, DARKGRAY);
				}

				// Input area
				DrawText("Enter page name:", 180, 370, 20, DARKGRAY);

				Rectangle inputBox = { 180, 400, 440, 40 };
				if (CheckCollisionPointRec(GetMousePosition(), inputBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					searchEdit = true;
				}
				GuiTextBox(inputBox, pageSearch, 50, searchEdit);

				if (GuiButton({ 180, 460, 200, 40 }, "View Page")) {
					string input(pageSearch);
					bool found = false;

					for (int i = 0; i < pagecount; i++) {
						if (pages[i]->GetName() == input && pages[i]->GetOwner() == currentuser) {
							selectedPage = pages[i];
							currentScreen = VIEWPAGE;
							searchEdit = false;
							pageSearch[0] = '\0';
							statusMessage = "";
							found = true;
							break;
						}
					}

					if (!found) {
						statusMessage = "Error: Page not found or not owned by you.";
					}
				}

				if (GuiButton({ 420, 460, 200, 40 }, "Post to Page")) {
					string input(pageSearch);
					bool found = false;

					for (int i = 0; i < pagecount; i++) {
						if (pages[i]->GetName() == input && pages[i]->GetOwner() == currentuser) {
							selectedPage = pages[i];
							postTarget = selectedPage;
							currentScreen = CREATEPOST;
							searchEdit = false;
							pageSearch[0] = '\0';
							statusMessage = "";
							found = true;
							break;
						}
					}

					if (!found) {
						statusMessage = "Error: Page not found or not owned by you.";
					}
				}

				// Feedback
				if (!statusMessage.empty()) {
					Color color = statusMessage.rfind("Error:", 0) == 0 ? RED : DARKGREEN;
					DrawText(statusMessage.c_str(), 200, 520, 20, color);
				}

				if (GuiButton({ 10, 10, 80, 30 }, "< Back")) {
					currentScreen = PAGESMENU;
					searchEdit = false;
					pageSearch[0] = '\0';
					statusMessage = "";
				}
			}
			break;
			case VIEWPAGE:
				if (!selectedPage) {
					currentScreen = OWNEDPAGES;
					break;
				}

				viewpage(selectedPage); // Use modified function

				if (GuiButton({ 10, 10, 80, 30 }, "< Back")) {
					currentScreen = PAGESMENU;
				}
			break;
			case VIEWPAGES:
			{
				static char searchPageName[50] = "";
				static bool searchEditMode = false;
				static string statusMessage = "";

				ClearBackground(RAYWHITE);

				// Background panel
				DrawRectangleRounded({ 150, 60, 500, 460 }, 0.1f, 10, Fade(LIGHTGRAY, 0.4f));
				DrawRectangleLinesEx({ 150, 60, 500, 460 }, 2, GRAY);

				DrawText("All Pages:", 180, 80, 28, DARKBLUE);

				// List all pages
				int y = 130;
				for (int i = 0; i < pagecount; i++) {
					string info = pages[i]->getID() + " - " + pages[i]->GetName();
					DrawText(info.c_str(), 180, y, 24, BLACK);
					y += 30;
				}

				// Input for search
				DrawText("Enter page name to view:", 180, 370, 20, DARKGRAY);
				Rectangle inputBox = { 180, 400, 440, 40 };

				if (CheckCollisionPointRec(GetMousePosition(), inputBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					searchEditMode = true;
				}

				GuiTextBox(inputBox, searchPageName, 50, searchEditMode);

				if (GuiButton({ 300, 460, 200, 40 }, "View Page")) {
					string input(searchPageName);
					page* found = nullptr;

					for (int i = 0; i < pagecount; i++) {
						if (pages[i]->GetName() == input) {
							found = pages[i];
							break;
						}
					}

					if (found) {
						selectedPage = found;
						currentScreen = VIEWPAGE;
						statusMessage = "";
						searchPageName[0] = '\0';
						searchEditMode = false;
					}

					else {
						statusMessage = "Error: Page not found.";
					}
				}

				if (!statusMessage.empty()) {
					Color color = (statusMessage.rfind("Error:", 0) == 0) ? RED : GREEN;
					DrawText(statusMessage.c_str(), 250, 520, 20, color);
				}

				// Back
				if (GuiButton({ 10, 10, 80, 30 }, "< Back")) {
					currentScreen = PAGESMENU;
					searchPageName[0] = '\0';
					statusMessage = "";
					searchEditMode = false;
				}
			}
			break;
			}
			EndDrawing();
		}
		CloseWindow();
	}
	void createpost(Entity* author)
	{
		static int selectedStatus = 0;
		static char detail[100] = "";
		static char text[300] = "";
		static bool detailEdit = false;
		static bool textEdit = false;
		static string feedback = "";

		ClearBackground(RAYWHITE);
		DrawText("Create a New Post", 260, 40, 30, BLACK);
		DrawText("Select Status:", 250, 90, 20, BLACK);

		if (GuiButton({ 250, 120, 120, 40 }, "Feeling")) selectedStatus = 1;
		DrawRectangleLinesEx({ 250, 120, 120, 40 }, 2, selectedStatus == 1 ? BLUE : GRAY);

		if (GuiButton({ 380, 120, 170, 40 }, "Thinking About")) selectedStatus = 2;
		DrawRectangleLinesEx({ 380, 120, 170, 40 }, 2, selectedStatus == 2 ? BLUE : GRAY);

		if (GuiButton({ 250, 170, 120, 40 }, "Making")) selectedStatus = 3;
		DrawRectangleLinesEx({ 250, 170, 120, 40 }, 2, selectedStatus == 3 ? BLUE : GRAY);

		if (GuiButton({ 380, 170, 170, 40 }, "Celebrating")) selectedStatus = 4;
		DrawRectangleLinesEx({ 380, 170, 170, 40 }, 2, selectedStatus == 4 ? BLUE : GRAY);

		string prompt = "Enter Status Activity:";
		switch (selectedStatus)
		{
		case 1: prompt = "What are you feeling?"; break;
		case 2: prompt = "What are you thinking about?"; break;
		case 3: prompt = "What are you making?"; break;
		case 4: prompt = "What are you celebrating?"; break;
		}
		DrawText(prompt.c_str(), 250, 230, 20, BLACK);
		if (GuiTextBox({ 250, 260, 300, 40 }, detail, 100, detailEdit)) {
			detailEdit = !detailEdit;
			textEdit = false;
		}

		DrawText("Enter post text:", 250, 320, 20, BLACK);
		if (GuiTextBox({ 250, 350, 300, 100 }, text, 300, textEdit)) {
			textEdit = !textEdit;
			detailEdit = false;
		}

		bool canPost = selectedStatus > 0 && strlen(detail) > 0 && strlen(text) > 0;
		Rectangle postBtn = { 325, 470, 150, 40 };
		GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, canPost ? 0x0077CCFF : 0xAAAAAAFF);
		if (GuiButton(postBtn, "Create Post") && canPost)
		{
			string activitystatus;
			switch (selectedStatus)
			{
			case 1: activitystatus = "feeling "; break;
			case 2: activitystatus = "thinking about "; break;
			case 3: activitystatus = "making "; break;
			case 4: activitystatus = "celebrating "; break;
			}
			activitystatus += detail;

			string id = "p" + to_string(postcount + 1);
			post* newPost = new post(text, id, author, activitystatus);
			newPost->SetDate(currentDate.GetDay(), currentDate.GetMonth(), currentDate.GetYear());
			posts[postcount++] = newPost;

			// Reset form
			selectedStatus = 0;
			detail[0] = '\0';
			text[0] = '\0';
			detailEdit = textEdit = false;

			currentScreen = (author == currentuser) ? MAINMENU : OWNEDPAGES;
		}

		// Feedback
		if (!feedback.empty()) {
			DrawText(feedback.c_str(), 280, 525, 20, feedback == "Post created!" ? DARKGREEN : RED);
		}

		// Back button
		if (GuiButton({ 10, 10, 80, 30 }, "< Back")) {
			selectedStatus = 0;
			detail[0] = '\0';
			text[0] = '\0';
			feedback = "";
			currentScreen = (author == currentuser) ? MAINMENU : OWNEDPAGES;
		}
	}
	void showstartscreen()
	{
		ClearBackground(RAYWHITE);

		// Header Title
		DrawText("FastGram", 275, 100, 50, BLACK);

		// Top-right Date Display
		string dateStr = to_string(currentDate.GetDay()) + "/" +
			to_string(currentDate.GetMonth()) + "/" +
			to_string(currentDate.GetYear());
		DrawText(dateStr.c_str(), 700, 10, 18, DARKGRAY);

		if (GuiButton({ 620, 10, 60, 18 }, "edit")) {
			currentScreen = SETDATE;
		}

		// Center Login/Register Panel
		float panelX = 200;
		float panelY = 180;
		float panelWidth = 400;
		float panelHeight = 200;

		DrawRectangleRounded({ panelX, panelY, panelWidth, panelHeight }, 0.2f, 12, Fade(LIGHTGRAY, 0.3f));
		DrawRectangleRoundedLines(Rectangle{ panelX, panelY, panelWidth, panelHeight }, 0.2f, 12, GRAY);

		if (GuiButton(Rectangle{ panelX + 50, panelY + 40, 300, 45 }, "Log in")) {
			currentScreen = LOGIN;
		}
		if (GuiButton(Rectangle{ panelX + 50, panelY + 110, 300, 45 }, "Sign up")) {
			currentScreen = REGISTER;
		}

		DrawText("@ 2025 FastGram", 320, 500, 18, LIGHTGRAY);

		// Exit Button
		if (GuiButton(Rectangle{ 10, 10, 30, 30 }, "x")) {
			CloseWindow();
			return;
		}
	}
	bool isRecent(date& postDate) {
		return (postDate.GetYear() == currentDate.GetYear() &&
			postDate.GetMonth() == currentDate.GetMonth() &&
			(postDate.GetDay() == currentDate.GetDay() ||
				postDate.GetDay() == (currentDate.GetDay() - 1)));
	}
	void viewhome() {
		static bool showLikeMessage = false;
		static float likeMessageTime = 0;
		static bool showCommentBox = false;
		static char commentInput[200] = "";
		static bool showLikeList = false;
		static bool showComments = false;

		if (!feedInitialized) {
			feedCount = 0;

			for (int i = 0; i < currentuser->GetFriendCount(); i++) {
				user* friendUser = currentuser->GetFriend(i);
				for (int j = 0; j < postcount; j++) {
					if (posts[j]->GetAuthor() == friendUser && isRecent(posts[j]->GetDate())) {
						feedPostsArr[feedCount++] = posts[j];
					}
				}
			}

			for (int i = 0; i < currentuser->GetLikedPageCount(); i++) {
				page* likedPage = currentuser->GetLikedPage(i);
				for (int j = 0; j < postcount; j++) {
					if (posts[j]->GetAuthor() == likedPage && isRecent(posts[j]->GetDate())) {
						feedPostsArr[feedCount++] = posts[j];
					}
				}
			}

			feedInitialized = true;
			currentPostIndex = 0;
			showCommentBox = false;
			showLikeMessage = false;
			showLikeList = false;
			showComments = false;
		}

		if (feedCount == 0) {
			DrawText("No recent posts to show.", 250, 280, 20, DARKGRAY);
			return;
		}

		if (currentPostIndex < 0) currentPostIndex = 0;
		if (currentPostIndex >= feedCount)
			currentPostIndex = feedCount - 1;
		post* p = feedPostsArr[currentPostIndex];

		//Navigation
		if (currentPostIndex > 0 && GuiButton({ 20, 300, 40, 40 }, "<")) {
			currentPostIndex--;
			showCommentBox = false;
			showLikeMessage = false;
			showLikeList = false;
			showComments = false;
		}

		if (currentPostIndex < feedCount - 1 && GuiButton({ 740, 300, 40, 40 }, ">")) {
			currentPostIndex++;
			showCommentBox = false;
			showLikeMessage = false;
			showLikeList = false;
			showComments = false;
		}

		// Identify author
		Entity* author = p->GetAuthor();
		string authorName = "Unknown";
		bool isPagePost = false;

		if (author != nullptr) {
			if (user* u = dynamic_cast<user*>(author)) {
				authorName = u->GetName();
				isPagePost = false;
			}
			else if (page* pg = dynamic_cast<page*>(author)) {
				authorName = pg->GetName();
				isPagePost = true;
			}
		}

		DrawRectangle(100, 150, 600, 300, isPagePost ? Color{ 225, 240, 255, 255 } : LIGHTGRAY);

		string activity = p->GetActivityStatus().empty() ? "shared" : p->GetActivityStatus();

		string dateStr = to_string(p->GetDate().GetDay()) + "/" +
			to_string(p->GetDate().GetMonth()) + "/" +
			to_string(p->GetDate().GetYear());
		DrawText(dateStr.c_str(), 120, 160, 18, DARKGRAY);

		if (isPagePost) {
			string postHeader = authorName + " is " + activity;
			DrawText(postHeader.c_str(), 120, 190, 20, DARKBLUE);
			DrawLine(120, 210, 680, 210, BLUE);
		}
		else {
			string postHeader = authorName + " is " + activity;
			DrawText(postHeader.c_str(), 120, 190, 20, BLACK);
			DrawLine(120, 215, 680, 215, GRAY);
		}

		DrawText(p->GetText().c_str(), 120, 230, 18, DARKGRAY);

		if (GuiButton({ 120, 400, 80, 30 }, "Like")) {
			p->LikePost(currentuser);
			showLikeMessage = true;
			likeMessageTime = GetTime();
		}

		if (GuiButton({ 220, 400, 100, 30 }, "Comment")) {
			showCommentBox = true;
			showLikeMessage = false;
		}

		if (GuiButton({ 340, 400, 120, 30 }, "View Likes")) {
			showLikeList = !showLikeList;
		}

		if (GuiButton({ 480, 400, 165, 30 }, showComments ? "Hide Comments" : "View Comments")) {
			showComments = !showComments;
		}

		if (showLikeMessage && (GetTime() - likeMessageTime < 2.0f)) {
			DrawText("Post liked successfully!", 120, 460, 18, GREEN);
		}

		if (showCommentBox) {
			DrawText("Enter your comment:", 120, 460, 18, BLACK);
			GuiTextBox({ 120, 480, 400, 30 }, commentInput, 200, true);

			if (GuiButton({ 530, 480, 100, 30 }, "Post")) {
				Comment newComment(commentInput, currentuser);
				p->AddComment(newComment);
				commentInput[0] = '\0';
				showCommentBox = false;
			}
		}

		if (showLikeList) {
			DrawText("Liked by:", 120, 460, 18, DARKGRAY);
			for (int i = 0; i < p->GetLikeCount(); i++) {
				user* liker = p->GetLikeUser(i);
				if (liker)
					DrawText(liker->GetName().c_str(), 140, 480 + i * 20, 16, GRAY);
			}
		}

		if (showComments) {
			DrawText("Comments:", 120, 460, 18, DARKGRAY);
			int y = 490;
			for (int i = 0; i < p->GetCommentCount(); i++) {
				Comment* c = &p->GetComments()[i];
				Entity* owner = c->GetOwner();
				string commenter = (owner != nullptr) ? dynamic_cast<user*>(owner)->GetName() : "Unknown";
				DrawText((commenter + ": " + c->GetContent()).c_str(), 140, y, 16, GRAY);
				y += 20;
			}
		}
	}
	void viewOwnedPages()
	{
		cout << "Your Pages:" << endl;
		for (int i = 0; i < pagecount; i++)
		{
			if (pages[i]->GetOwner() == currentuser)
			{
				cout << pages[i]->getID() << "-" << pages[i]->GetName() << endl;
			}
		}
	}
	void viewlikedpages()
	{
		cout << "Your Liked Pages:" << endl;
		for (int i = 0; i < currentuser->GetLikedPageCount(); i++)
		{
			page* likedPage = currentuser->GetLikedPage(i);
			cout << likedPage->getID() << "-" << likedPage->GetName() << endl;
		}
	}
	void posttoaPage()
	{
		cout << "Select a page to post to:" << endl;
		for (int i = 0; i < pagecount; i++)
		{
			if (pages[i]->GetOwner() == currentuser)
			{
				cout << pages[i]->getID() << "-" << pages[i]->GetName() << endl;
			}
		}
		string pageID;
		cout << "Enter the page ID: ";
		cin >> pageID;
		page* targetPage = NULL;
		for (int i = 0; i < pagecount; i++)
		{
			if (pages[i]->getID() == pageID)
			{
				targetPage = pages[i];
				break;
			}
		}
		if (targetPage == NULL)
		{
			cout << "Page not found!" << endl;
			return;
		}
		string content;
		int choice;
		string activitystatus = "";
		do {
			cout << "Whats your status? 1. feeling, 2. thinking about, 3. Making, 4. Celebrating" << endl;
			cin >> choice;
		} while (choice < 1 || choice > 4); // Ensure valid choice
		switch (choice) {
		case 1:
		{
			cout << "What are you feeling?" << endl;
			string feeling;
			cin >> feeling;
			activitystatus += "feeling " + feeling + " ";
			break;
		}
		case 2:
		{
			cout << "What are you thinking about?" << endl;
			string thought;
			cin >> thought;
			activitystatus += "thinking about " + thought + " ";
			break;
		}
		case 3:
		{
			cout << "What are you making?" << endl;
			string making;
			cin >> making;
			activitystatus += "making " + making + " ";
			break;
		}
		case 4:
		{
			cout << "What are you celebrating?" << endl;
			string celebration;
			cin >> celebration;
			activitystatus += "celebrating " + celebration + " ";
			break;
		}
		};
		string text;
		cout << "Enter your post text: ";
		cin.ignore();
		getline(cin, text);
		string id = "p";
		id += to_string(postcount + 1);
		post* newPost = new post(text, id, targetPage, activitystatus);
		posts[postcount++] = newPost;
		newPost->SetDate(currentDate.GetDay(), currentDate.GetMonth(), currentDate.GetYear());
		cout << "Post created successfully!" << endl;
	}
	bool isMemoryPost(post* p) {
		for (int i = 0; i < memorycount; i++) {
			if (memories[i] == dynamic_cast<Memory*>(p)) {
				return true;
			}
		}
		return false;
	}
    void viewprofile(user* targetuser) {
		static bool profileFeedInitialized = false;
		static int lastPostCount = -1;
		static int lastMemoryCount = -1;
		static bool showComments = false;
		static post* userPostsArr[100];
		static int userPostCount = 0;
		static int userPostIndex = 0;

		static bool showLikeMessage = false;
		static float likeMessageTime = 0;
		static bool showCommentBox = false;
		static char commentInput[200] = "";
		static bool showLikeList = false;
		static user* lastViewedUser = nullptr;

		static bool showMemoryBox = false;
		static char memoryText[200] = "";
		static bool showRepostMessage = false;
		static float repostMessageTime = 0;

		// Reinitialize if post or memory count has changed
		if (postcount != lastPostCount || memorycount != lastMemoryCount || targetuser != lastViewedUser) {
			profileFeedInitialized = false;
			lastPostCount = postcount;
			lastMemoryCount = memorycount;
			lastViewedUser = targetuser;  // Update to the new user being viewed

		}

		if (!profileFeedInitialized) {
			userPostCount = 0;     // clear


			for (int i = 0; i < postcount; i++) {
				if (posts[i]->GetAuthor() == targetuser) {
					userPostsArr[userPostCount++] = posts[i];
				}
			}
			for (int i = 0; i < memorycount; i++) {
				if (memories[i]->getoriginalpost()->GetAuthor() == targetuser) {
					userPostsArr[userPostCount++] = dynamic_cast<post*>(memories[i]); // store as post*
				}
			}

			userPostIndex = 0;
			profileFeedInitialized = true;
			showCommentBox = false;
			showLikeMessage = false;
			showLikeList = false;
		}

		ClearBackground(RAYWHITE);

		string username = targetuser->GetName();
		string stats = "Posts: " + to_string(userPostCount) +
			"  |  Friends: " + to_string(targetuser->GetFriendCount());

		DrawText(username.c_str(), 120, 40, 30, BLACK);
		DrawText(stats.c_str(), 120, 80, 20, GRAY);

		if (userPostCount == 0) {
			DrawText("No posts yet.", 325, 280, 20, DARKGRAY);
			return;
		}

		if (userPostIndex < 0) userPostIndex = 0;
		if (userPostIndex >= userPostCount) userPostIndex = userPostCount - 1;

		post* p = userPostsArr[userPostIndex];

		if (userPostIndex > 0 && GuiButton({ 20, 300, 40, 40 }, "<")) {
			userPostIndex--;
			showCommentBox = false;
			showLikeMessage = false;
			showLikeList = false;
			// After changing userPostIndex or postIndex
			showComments = false;

		}

		if (userPostIndex < userPostCount - 1 && GuiButton({ 740, 300, 40, 40 }, ">")) {
			userPostIndex++;
			showCommentBox = false;
			showLikeMessage = false;
			showLikeList = false;
			// After changing userPostIndex or postIndex
			showComments = false;

		}

		DrawRectangle(100, 150, 600, 300, LIGHTGRAY);

		string dateStr = to_string(p->GetDate().GetDay()) + "/" +
			to_string(p->GetDate().GetMonth()) + "/" +
			to_string(p->GetDate().GetYear());
		DrawText(dateStr.c_str(), 120, 160, 18, DARKGRAY);

		if (isMemoryPost(p)) {
			Memory* mem = dynamic_cast<Memory*>(p);
			int years = p->GetDate().GetYear() - mem->getoriginalpost()->GetDate().GetYear();
			string yearsago = to_string(years) + " years ago";
			string memoryHeader = targetuser->GetName() + " shared a memory..................(" + yearsago + ")";
			DrawText(memoryHeader.c_str(), 120, 190, 20, MAROON);
			DrawLine(120, 215, 680, 215, GRAY);

			// Draw memory text in red
			DrawText(mem->GetText().c_str(), 120, 230, 18, RED);
			string datestr = to_string(mem->getoriginalpost()->GetDate().GetDay()) + "/" +
				to_string(mem->getoriginalpost()->GetDate().GetMonth()) + "/" +
				to_string(mem->getoriginalpost()->GetDate().GetYear());

			// Show original post content below
			string activity = mem->getoriginalpost()->GetActivityStatus().empty() ? "shared" : mem->getoriginalpost()->GetActivityStatus();
			string postHeader = targetuser->GetName() + " is " + activity + "..........(" + datestr + ")";
			DrawText(postHeader.c_str(), 120, 260, 18, BLACK);
			DrawText(mem->getoriginalpost()->GetText().c_str(), 120, 285, 18, DARKGRAY);
		}
		else {
			string activity = p->GetActivityStatus().empty() ? "shared" : p->GetActivityStatus();
			string postHeader = targetuser->GetName() + " is " + activity;
			DrawText(postHeader.c_str(), 120, 190, 20, BLACK);
			DrawLine(120, 215, 680, 215, GRAY);
			DrawText(p->GetText().c_str(), 120, 230, 18, DARKGRAY);
		}


		if (GuiButton({ 120, 400, 80, 30 }, "Like")) {
			p->LikePost(currentuser);
			showLikeMessage = true;
			likeMessageTime = GetTime();
		}

		if (GuiButton({ 220, 400, 100, 30 }, "Comment")) {
			showCommentBox = true;
			showLikeMessage = false;
		}

		if (GuiButton({ 340, 400, 120, 30 }, "View Likes")) {
			showLikeList = !showLikeList;
		}

		if (GuiButton({ 480, 400, 165, 30 }, showComments ? "Hide Comments" : "View Comments")) {
			showComments = !showComments;
		}
		if (targetuser == currentuser && !isMemoryPost(p) && GuiButton({ 440, 160, 240, 30 }, "Repost as Memory")) {
			showMemoryBox = true;
			showCommentBox = false;
			showLikeMessage = false;
			showLikeList = false;
		}


		if (showLikeMessage && (GetTime() - likeMessageTime < 2.0f)) {
			DrawText("Post liked successfully!", 120, 460, 18, GREEN);
		}

		if (showCommentBox) {
			DrawText("Enter your comment:", 120, 460, 18, BLACK);
			GuiTextBox({ 120, 480, 400, 30 }, commentInput, 200, true);

			if (GuiButton({ 530, 505, 100, 30 }, "Post")) {
				Comment newComment(commentInput, currentuser);
				p->AddComment(newComment);
				commentInput[0] = '\0'; // clear input
				showCommentBox = false;
			}
		}

		if (showLikeList) {
			DrawText("Liked by:", 120, 460, 18, DARKGRAY);
			for (int i = 0; i < p->GetLikeCount(); i++) {
				user* liker = p->GetLikeUser(i);
				if (liker)
					DrawText(liker->GetName().c_str(), 140, 480 + i * 20, 16, GRAY);
			}
		}

		if (showComments) {
			DrawText("Comments:", 120, 460, 18, DARKGRAY);
			int y = 490;
			for (int i = 0; i < p->GetCommentCount(); i++) {
				Comment* c = &p->GetComments()[i];
				Entity* owner = c->GetOwner();
				string commenter = (owner != nullptr) ? dynamic_cast<user*>(owner)->GetName() : "Unknown";
				DrawText((commenter + ": " + c->GetContent()).c_str(), 140, y, 16, GRAY);
				y += 20;
			}
		}
		if (showMemoryBox) {
			DrawText("Enter memory text:", 120, 480, 18, BLACK);
			GuiTextBox({ 120, 500, 400, 30 }, memoryText, 200, true);

			if (GuiButton({ 530, 500, 100, 30 }, "Post")) {
				Memory* m = new Memory(p, memoryText, currentuser); // Pass currentuser as author

				m->SetDate(currentDate.GetDay(), currentDate.GetMonth(), currentDate.GetYear());

				memories[memorycount++] = m;

				memoryText[0] = '\0';
				showMemoryBox = false;
				showRepostMessage = true;
				repostMessageTime = GetTime();
				profileFeedInitialized = false;

			}
		}
	}
	void viewpage(page* targetPage) {
		static bool feedInitialized = false;
		static int lastPostCount = -1;
		static post* pagePostsArr[100];
		static int pagePostCount = 0;
		static int pagePostIndex = 0;
		static bool showPageLikes = false;

		static bool showCommentBox = false;
		static char commentInput[200] = "";
		static bool showLikeMessage = false;
		static float likeMessageTime = 0;
		static bool showComments = false;
		static bool showLikeList = false;

		// Reinitialize when post count changes or new page is loaded
		static page* lastViewedPage = nullptr;
		if (postcount != lastPostCount || targetPage != lastViewedPage) {
			feedInitialized = false;
			lastPostCount = postcount;
			lastViewedPage = targetPage;
		}

		if (!feedInitialized) {
			pagePostCount = 0;
			for (int i = 0; i < postcount; i++) {
				if (posts[i]->GetAuthor() == targetPage) {
					pagePostsArr[pagePostCount++] = posts[i];
				}
			}

			pagePostIndex = 0;
			showCommentBox = false;
			showLikeMessage = false;
			showLikeList = false;
			feedInitialized = true;
		}

		ClearBackground(RAYWHITE);

		// Page Header
		DrawText(targetPage->GetName().c_str(), 120, 40, 30, DARKBLUE);
		DrawText(("Page ID: " + targetPage->getID()).c_str(), 120, 80, 20, GRAY);

		// Like or Unlike Page
		bool alreadyLiked = false;
		for (int i = 0; i < currentuser->GetLikedPageCount(); i++) {
			if (currentuser->GetLikedPage(i) == targetPage) {
				alreadyLiked = true;
				break;
			}
		}

		if (!alreadyLiked) {
			if (GuiButton({ 500, 40, 130, 30 }, "Like Page")) {
				currentuser->likepage(targetPage);
				showLikeMessage = true;
				likeMessageTime = GetTime();
			}
		}
		else {
			DrawText("You like this page", 500, 40, 20, DARKGRAY);
		}
		if (GuiButton({ 500, 75, 185, 30 }, showPageLikes ? "Hide Page Likes" : "View Page Likes")) {
			showPageLikes = !showPageLikes;
		}

		if (showPageLikes) {
			DrawText("Page liked by:", 120, 450, 20, DARKGRAY);
			int y = 480;
			for (int i = 0; i < usercount; i++) {
				for (int j = 0; j < users[i]->GetLikedPageCount(); j++) {
					if (users[i]->GetLikedPage(j) == targetPage) {
						DrawText(users[i]->GetName().c_str(), 140, y, 18, GRAY);
						y += 24;
						break;
					}
				}
			}
			// if we're only viewing likes, early-out here
			if (pagePostCount == 0) return;
		}

		if (pagePostCount == 0) {
			DrawText("No posts on this page yet.", 250, 280, 20, DARKGRAY);
			return;
		}

		// Clamp index
		if (pagePostIndex < 0) pagePostIndex = 0;
		if (pagePostIndex >= pagePostCount) pagePostIndex = pagePostCount - 1;

		post* p = pagePostsArr[pagePostIndex];

		// Navigation
		if (pagePostIndex > 0 && GuiButton({ 20, 300, 40, 40 }, "<")) {
			pagePostIndex--;
			showCommentBox = false;
			showLikeMessage = false;
			showLikeList = false;
			showComments = false;
		}

		if (pagePostIndex < pagePostCount - 1 && GuiButton({ 740, 300, 40, 40 }, ">")) {
			pagePostIndex++;
			showCommentBox = false;
			showLikeMessage = false;
			showLikeList = false;
			showComments = false;
		}

		// Post Display Area
		DrawRectangle(100, 150, 600, 300, Fade(SKYBLUE, 0.2f));

		string dateStr = to_string(p->GetDate().GetDay()) + "/" +
			to_string(p->GetDate().GetMonth()) + "/" +
			to_string(p->GetDate().GetYear());

		DrawText(dateStr.c_str(), 120, 160, 18, DARKGRAY);

		string activity = p->GetActivityStatus().empty() ? "posted" : p->GetActivityStatus();
		string postHeader = targetPage->GetName() + " is " + activity;
		DrawText(postHeader.c_str(), 120, 190, 20, BLACK);
		DrawLine(120, 215, 680, 215, GRAY);
		DrawText(p->GetText().c_str(), 120, 230, 18, DARKGRAY);

		// Interaction Buttons
		if (GuiButton({ 120, 400, 80, 30 }, "Like")) {
			p->LikePost(currentuser);
			showLikeMessage = true;
			likeMessageTime = GetTime();
		}

		if (GuiButton({ 220, 400, 100, 30 }, "Comment")) {
			showCommentBox = true;
			showLikeMessage = false;
		}

		if (GuiButton({ 340, 400, 120, 30 }, "View Likes")) {
			showLikeList = !showLikeList;
		}

		if (GuiButton({ 480, 400, 165, 30 }, showComments ? "Hide Comments" : "View Comments")) {
			showComments = !showComments;
		}

		// Like Feedback
		if (showLikeMessage && (GetTime() - likeMessageTime < 2.0f)) {
			DrawText("Post liked successfully!", 120, 460, 18, DARKGREEN);
		}

		// Comment Box
		if (showCommentBox) {
			DrawText("Enter your comment:", 120, 460, 18, BLACK);
			GuiTextBox({ 120, 480, 400, 30 }, commentInput, 200, true);

			if (GuiButton({ 530, 505, 100, 30 }, "Post")) {
				Comment newComment(commentInput, currentuser);
				p->AddComment(newComment);
				commentInput[0] = '\0';
				showCommentBox = false;
			}
		}

		// View Likes
		if (showLikeList) {
			DrawText("Liked by:", 120, 460, 18, DARKGRAY);
			for (int i = 0; i < p->GetLikeCount(); i++) {
				user* liker = p->GetLikeUser(i);
				if (liker)
					DrawText(liker->GetName().c_str(), 140, 480 + i * 20, 16, GRAY);
			}
		}

		// View Comments
		if (showComments) {
			DrawText("Comments:", 120, 460, 18, DARKGRAY);
			int y = 490;
			for (int i = 0; i < p->GetCommentCount(); i++) {
				Comment* c = &p->GetComments()[i];
				Entity* owner = c->GetOwner();
				string commenter = (owner != nullptr) ? dynamic_cast<user*>(owner)->GetName() : "Unknown";
				DrawText((commenter + ": " + c->GetContent()).c_str(), 140, y, 16, GRAY);
				y += 20;
			}
		}
	}
};