#include <iostream>
#include <mysql_driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <SDL2/SDL.h>

#include <string>
using namespace std; 

sql::Connection *con = nullptr; 
const int SCREEN_WIDTH = 800; 
const int SCREEN_HEIGHT = 600 ;

void connectDatabase(){
	try{
		sql::mysql::MySQL_Driver *driver; 
		driver = sql::mysql::get_mysql_driver_instance(); 
		con = driver->connect("tcp://127.0.0.1:3306","root","netacad"); 
		con->setSchema("SearchSystem"); 

	}catch(sql::SQLException &e){
		cerr << "Unable to connect mysql " << e.what() << endl; 
		con = nullptr; 
	}			
}

string searchDatabase(string userInput) {	
	string result = "";
	try {
		string input = "%" + userInput + "%"; 
		string query = "select * from search where description  LIKE  ?; " ; 
		sql::PreparedStatement* pstmt = con->prepareStatement(query); 
		pstmt->setString(1,input); 
		sql::ResultSet* res = pstmt->executeQuery(); 
		while (res-> next()) {
			string desc = res->getString("description"); 
			result += desc + "\n";
		}
		delete res; 
		delete pstmt; 
	}catch(sql::SQLException &e) {
		cerr << "Unable to process input " << endl ;	
				
	}	
	return result ;
	

}

void mainPage(SDL_Renderer* renderer, SDL_Window* window){
	string userInput = "" ; 
	string output= ""; 
	SDL_Event e ; 
	bool quit = false ; 
	while (!quit) {
		SDL_SetRenderDrawColor(renderer,255,255,255,255); 
		SDL_RenderClear(renderer); 
		SDL_Rect inputBox = {50,50,500,50}; 
		SDL_Rect outputBox = {50,150,500,300};
		SDL_SetRenderDrawColor(renderer, 200,200,200,255); 
		SDL_RenderFillRect(renderer, &inputBox); 

		SDL_SetRenderDrawColor(renderer,230,230,230,255); 
		SDL_RenderFillRect(renderer, &outputBox); 
		SDL_RenderPresent(renderer); 
		while(SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT) {
				quit = true; 
			}else if (e.type == SDL_TEXTINPUT ||e.type == SDL_KEYDOWN) {

				if(e.key.keysym.sym == SDLK_RETURN){
					output = searchDatabase(userInput); 
					userInput = ""; 
				}else if (e.key.keysym.sym == SDLK_BACKSPACE && !userInput.empty()){
					userInput.pop_back(); 
				}else if (e.type == SDL_TEXTINPUT){
					userInput += e.text.text;
				}
			}		
		}
	}
}

int main () {

	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		cerr << "SDL cannot be connected " << SDL_GetError() << endl ;
		return -1 ;

	}
	SDL_Window* window = SDL_CreateWindow("Search System" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); 
	if (!window){
		cerr << "Winidow cannot be opened " << SDL_GetError() << endl; 
		SDL_Quit(); 
		return -1; 
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1  ,SDL_RENDERER_ACCELERATED) ; 
	if (!renderer) {
		cerr << "Renderer cannot be created " << SDL_GetError() << endl ;
		SDL_DestroyWindow(window); 
		SDL_Quit();


	}	

	connectDatabase();
	if (con) {
		mainPage(renderer, window);
	}
	SDL_DestroyRenderer(renderer); 
	SDL_DestroyWindow(window);
	SDL_Quit();
}
