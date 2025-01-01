#include <iostream>
using namespace std; 

sql::Connection *con = nullptr; 


void connectDatabase(){
	try{
		sql::mysql::MySQL_Driver* driver; 
		driver = sql::mysql::get_mysql_driver_instance(); 
		con = driver->connect("tcp://127.0.0.1::3306","root","netacad"); 
		con->connectDatabase("SearchSystem"); 

	}catch(sql::SQLException &e){
		cerr << "Unable to connect mysql " << e.what() << endl; 
		con = nullptr; 
	}			
}

void searchhDatabase(string userInput) {	
	input = "%" + userInput + "%"; 
	string query = "select * from search where description = ? " ; 
	sql::PreparedStatement* pstmt = con->prepareStatement(query); 
	pstmt ->setString(1,input); 
       	sql::ResultSet* res = pstmt->executeQuery(); 
	while (res-> next()) {
		string desc = res->getString("description"); 
		cout << description << endl;  
	}	
	delete pstmt ; 
	delete res; 

}

void mainPage(){
	while(true){
		cin.clear();
		string userInput; 
		cout << "Please insert what you would like to search" << endl; 
		cin >> userInput ; 
		searchDatabase(userInput); 		
	}
}
int main () {
	connectDatabase();
	mainPage(); 	
}
