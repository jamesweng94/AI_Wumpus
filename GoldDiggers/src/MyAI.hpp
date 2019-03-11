// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"
#include <iostream>
#include <stack>
#include <map>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
using namespace std;

class MyAI : public Agent
{
public:
	MyAI ( void );
	
	Action getAction
	(
		bool stench,
		bool breeze,
		bool glitter,
		bool bump,
		bool scream
	);
	enum Directions{up, down, left, right};	
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	void eraseDup(vector<pair<int, int>>&);
	bool checkBound(int, int);
	bool checkHistory(vector<pair<int, int>>, int, int);
	bool check_wumpus(int, int);
	bool nearWumpus(pair<int, int>);
	bool findRiskyCell(vector<pair<int, int>>, pair<int, int> const&, pair<int, int>& );
	void getValidMoves(vector<Directions>&, const pair<int, int> position, bool&, bool&, bool&, bool&);
	Action backtrack(vector<pair<int, int>>&);
	Action executeMove(Directions);
	Action killWumpus(const pair<int, int>);
	Action shortestPath(vector<pair<int,int>> &);
	
private:
	const Action actions[6] =
	{
		TURN_LEFT,
		TURN_RIGHT,
		FORWARD,
		SHOOT,
		GRAB,
		CLIMB,
	};
	vector<pair<int,int>> visited;	
	vector<pair<int, int>> safePath;
	vector<pair<int,int>> shortPath;
	vector<pair<int, int>> stench_cell;
	pair<int, int> current_position;
	pair<int, int> wumpus_location;
	Directions current_dir;
	bool is_safe = false;
	bool arrow_fired = false;
	bool gold_found = false;
	bool wumpus_live = true;
	bool wumpus_found = false;

	// =====================================================================
	// YOUR CODE ENDS
	// =====================================================================
};

#endif
