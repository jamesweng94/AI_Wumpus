	// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"

MyAI::MyAI() : Agent()
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	visited.push_back(make_pair(1,1));
	safePath.push_back(make_pair(1,1));
	current_position.first = 1;
	current_position.second = 1;
	current_dir = right;		
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}
	
Agent::Action MyAI::getAction
(
	bool stench,
	bool breeze,
	bool glitter,
	bool bump,
	bool scream
)
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================

	bool to_up = true, to_down = true, to_left = true, to_right = true;
	Directions nextMove;
	vector<Directions> moves;

	if(bump)
	{
		current_position = safePath.back();
		if(current_dir == up)
			to_up = false;
		if(current_dir == down)
			to_down = false;
		if(current_dir == left)
			to_left = false;
		if(current_dir == right)
			to_right = false;		
	}

	//erase dup
	safePath.push_back(current_position);
	eraseDup(safePath);
	eraseDup(visited);
	eraseDup(stench_cell);

	if(glitter)
	{
		gold_found = true;
		return GRAB;
	}
	
	if(gold_found)
	{
		if(current_position.first == 1 && current_position.second == 1)
		{			
			return CLIMB;
		}
		else
			return shortestPath(safePath);
	}
	else
	{
		if(!is_safe)
		{
			if(breeze || stench)
				return CLIMB;
			else
				is_safe = true;
		}

		if(stench)
		{			
			stench_cell.push_back(current_position);
			pair<int, int> other_stench;

			if(wumpus_found == false)
			{
				if(findRiskyCell(stench_cell, current_position, other_stench))
				{
					if(current_position.first == other_stench.first)
					{
						int y = (current_position.second + other_stench.second) / 2;
						wumpus_found = true;
						wumpus_location = make_pair(current_position.first,y);					
					}
					if(current_position.second == other_stench.second)
					{
						int x = (current_position.first + other_stench.first) / 2;
						wumpus_found = true;				
						wumpus_location = make_pair(x,current_position.second);										
					}
					// "\"
					if((current_position.first - other_stench.first == 1 && current_position.second - other_stench.second == -1)
					||(current_position.first - other_stench.first == -1 && current_position.second - other_stench.second == 1))
					{
						pair<int, int> location1;
						pair<int, int> location2;
						if((current_position.first > other_stench.first) && (current_position.second < other_stench.second))
						{
							location1 = make_pair(current_position.first, current_position.second + 1);
							location2 = make_pair(current_position.first - 1, current_position.second);
						}
						else
						{
							location1 = make_pair(current_position.first + 1, current_position.second);
							location2 = make_pair(current_position.first, current_position.second - 1);
						}
						if(checkHistory(visited, location1.first, location1.second) == false)
						{
								wumpus_found = true;						
								wumpus_location = location2;	
						}
						else if(checkHistory(visited, location2.first, location2.second) == false)
						{
								wumpus_found = true;																	
								wumpus_location = location1;			
						}
						else
						{
							return backtrack(safePath);						
						}
					}
					// "/"
					if((current_position.first - other_stench.first == 1 && current_position.second - other_stench.second == 1)
					||(current_position.first - other_stench.first == -1 && current_position.second - other_stench.second == -1))
					{
						pair<int, int> location1;
						pair<int, int> location2;
						if(current_position.first > other_stench.first && current_position.second > other_stench.second)
						{
							location1 = make_pair(current_position.first, current_position.second - 1);
							location2 = make_pair(current_position.first - 1, current_position.second);
						}
						else
						{
							location1 = make_pair(current_position.first + 1, current_position.second);
							location2 = make_pair(current_position.first, current_position.second + 1);
						}

						if(checkHistory(visited, location1.first, location1.second) == false)
						{
								wumpus_found = true;						
								wumpus_location = location2;	
						}
						else if(checkHistory(visited, location2.first, location2.second) == false)
						{
								wumpus_found = true;																		
								wumpus_location = location1;
						}
						else
						{
							return backtrack(safePath);						
						}				
					}					
				}
				else
				{
					return backtrack(safePath);					
				}
			}
		}

	 	if(breeze)
		{
			return backtrack(safePath);
		}		
		else
		{	
			//get all valid moves
			getValidMoves(moves, current_position, to_up, to_down, to_left, to_right);

			if(!moves.empty())
			{
				nextMove = moves.at(0);
				return executeMove(nextMove);
			}
			else
			{
				if(scream)
				{
					wumpus_live = false;
					return executeMove(current_dir);
				}
				if(current_position.first == 1 && current_position.second == 1)
					return CLIMB;
				if(wumpus_live && nearWumpus(current_position))
				{
					return killWumpus(current_position);
				}
				return backtrack(safePath);
			}
		}
			
	}
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================
void MyAI::eraseDup(vector<pair<int, int>>& dup_v)
{
	auto it = dup_v.begin();
	for (; it != dup_v.end(); ++it) {
		auto pit = dup_v.begin();
		for (; pit != dup_v.end();) {
			if ((it->first == pit->first && it->second == pit->second) && (pit != it)) {
				pit = dup_v.erase(pit);
			} else {
				++pit;
			}
		}
	}
}

bool MyAI::checkBound(int _row, int _col)
{
	if(_row >= 1 && _col >= 1)
		return true;
	else
		return false;
}

bool MyAI::nearWumpus(pair<int, int> position)
{
	if((position.first + 1 == wumpus_location.first && position.second == wumpus_location.second)
	|| (position.first - 1 == wumpus_location.first && position.second == wumpus_location.second)
	|| (position.first == wumpus_location.first && position.second + 1 == wumpus_location.second)
	|| (position.first == wumpus_location.first && position.second - 1 == wumpus_location.second))
		return true;
	else
		return false;
}

bool MyAI::checkHistory(vector<pair<int, int>> pathVisited, int row, int col)
{
	for(int i=0;i<pathVisited.size();i++)
	{
		if(row == pathVisited[i].first && col==pathVisited[i].second)
		{
			return false;
		}
	}	
		return true;	
}

bool MyAI::check_wumpus(int row, int col)
{
		if(row == wumpus_location.first && col == wumpus_location.second)
			return false;
		else
			return 	true;
}


Agent::Action MyAI::shortestPath( vector<pair<int,int>> & path )
{
	pair<int,int> chosen_cell;
	int best_offset = 100;

	for (int i = 0; i < path.size(); i++)
	{
		int adj_offset = abs(current_position.first - path[i].first) + abs(current_position.second - path[i].second);
		int start_offset = abs(path[i].first - 1) + abs(path[i].second - 1);

		if (adj_offset == 1)
		{
			if ( (start_offset < best_offset) && (find(shortPath.begin(), shortPath.end(), path[i]) == shortPath.end()) )
			{
				chosen_cell = path[i];
				best_offset = start_offset;
			}
		}
	}

	if (current_position.first - chosen_cell.first == 1)
		return executeMove(down);
	if (current_position.first - chosen_cell.first == -1)
		return executeMove(up);
	if (current_position.second - chosen_cell.second == 1)
		return executeMove(left);
	if (current_position.second - chosen_cell.second == -1)
		return executeMove(right);
}

void MyAI::getValidMoves(vector<Directions>& nextMoves, const pair<int, int> position, bool& up_, bool& down_, bool& left_, bool& right_)
{
	if(checkBound(position.first, position.second + 1) && right_)
	{
		if(checkHistory(visited, position.first, position.second + 1)
			&&check_wumpus(position.first, position.second + 1))
			nextMoves.push_back(right);
	}
	if(checkBound(position.first + 1, position.second) && up_)
	{
		if(checkHistory(visited, position.first + 1, position.second)
			&&check_wumpus(position.first + 1, position.second))
			nextMoves.push_back(up);
	}						
	if(checkBound(position.first - 1, position.second) && down_)
	{
		if(checkHistory(visited, position.first - 1, position.second)
			&&check_wumpus(position.first - 1, position.second))
			nextMoves.push_back(down);
	}
	if(checkBound(position.first, position.second - 1) && left_)
	{
		if(checkHistory(visited, position.first, position.second - 1)
			&&check_wumpus(position.first, position.second - 1))
			nextMoves.push_back(left);
	}
}

bool MyAI::findRiskyCell(vector<pair<int, int>> riskCell, pair<int, int> const& position, pair<int, int>& element)
{
	int offset = 0;
	for(int i = 0; i < riskCell.size(); ++i)
	{
		offset = abs(position.first - riskCell.at(i).first) + abs(position.second - riskCell.at(i).second);
		if(offset == 2)
		{
			element = make_pair(riskCell.at(i).first, riskCell.at(i).second);
			return true;
		}
	}
	return false;
}

Agent::Action MyAI::backtrack(vector<pair<int, int>>& _path)
{
	Directions move_choose;

	if(current_position.first - _path.end()[-2].first == 1)
	{
		move_choose = down;
		safePath.pop_back();
		return executeMove(down);		
	}
	if(current_position.first - _path.end()[-2].first == -1)
	{
		move_choose = up;
		safePath.pop_back();
		return executeMove(up);		
	}	
	if(current_position.second - _path.end()[-2].second == 1)
	{
		move_choose = left;
		safePath.pop_back();
		return executeMove(left);		
	}	
	if(current_position.second - _path.end()[-2].second == -1)
	{
		safePath.pop_back();
		move_choose = right;
		return executeMove(right);		
	}	
}

Agent::Action MyAI::executeMove(Directions nextMove)
{
	if(nextMove == up)
	{
		if(current_dir == up)
		{
			if (gold_found)
				shortPath.push_back(current_position);
			current_position.first = current_position.first + 1;
			current_position.second = current_position.second;
			visited.push_back(current_position);	
			return FORWARD;
		}
		if(current_dir == down)
		{
			current_dir = right;
			return TURN_LEFT;	
		}

		if(current_dir == right)
		{
			current_dir = up;
			return TURN_LEFT;
		}
		if(current_dir == left)
		{
			current_dir = up;
			return TURN_RIGHT;
		}
	}	
	if(nextMove == down)
	{
		if(current_dir == down)
		{
			if (gold_found)
				shortPath.push_back(current_position);
			current_position.first = current_position.first - 1;
			current_position.second = current_position.second;
			visited.push_back(current_position);
			return FORWARD;
		}
		if(current_dir == up)
		{
			current_dir = left;
			return TURN_LEFT;
		}
		if(current_dir == left)
		{
			current_dir = down;
			return TURN_LEFT;
		}
		if(current_dir == right)
		{
			current_dir = down;
			return TURN_RIGHT;
		}
	}
	if(nextMove == right)
	{
		if(current_dir == right)
		{
			if (gold_found)
				shortPath.push_back(current_position);
			current_position.second = current_position.second + 1;
			current_position.first = current_position.first;
			visited.push_back(current_position);
			return FORWARD;
		}
		if(current_dir == up)
		{
			current_dir = right;
			return TURN_RIGHT;
		}
		if(current_dir == down)
		{
			current_dir = right;
			return TURN_LEFT;
		}
		if(current_dir == left)
		{
			current_dir = up;
			return TURN_RIGHT;
		}
	}
	if(nextMove == left)
	{
		if(current_dir == left)
		{
			if (gold_found)
				shortPath.push_back(current_position);
			current_position.second = current_position.second - 1;
			current_position.first = current_position.first;
			visited.push_back(current_position);
			return FORWARD;
		}
		if(current_dir == up)
		{	
			current_dir = left;
			return TURN_LEFT;
		}
		if(current_dir == right)
		{
			current_dir = up;
			return TURN_LEFT;
		}
		if(current_dir == down)
		{
			current_dir = left;
			return TURN_RIGHT;
		}		
	}
}

Agent::Action MyAI::killWumpus(const pair<int, int> position)
{
	Directions wumpus_dir;
	if((position.first - wumpus_location.first) == 1)
		wumpus_dir = down;
	if((position.first - wumpus_location.first) == -1)
		wumpus_dir = up;
	if((position.second - wumpus_location.second) == -1)
		wumpus_dir = right;
	if((position.second - wumpus_location.second) == 1)
		wumpus_dir = left;
	
	if(current_dir != wumpus_dir)
	{
		if(wumpus_dir == up)
		{
			if(current_dir == down)
			{
				current_dir = right;
				return TURN_LEFT;	
			}

			if(current_dir == right)
			{
				current_dir = up;
				return TURN_LEFT;
			}
			if(current_dir == left)
			{
				current_dir = up;
				return TURN_RIGHT;
			}
		}	
		if(wumpus_dir == down)
		{
			if(current_dir == up)
			{
				current_dir = left;
				return TURN_LEFT;
			}
			if(current_dir == left)
			{
				current_dir = down;
				return TURN_LEFT;
			}
			if(current_dir == right)
			{
				current_dir = down;
				return TURN_RIGHT;
			}
		}
		if(wumpus_dir == right)
		{
			if(current_dir == up)
			{
				current_dir = right;
				return TURN_RIGHT;
			}
			if(current_dir == down)
			{
				current_dir = right;
				return TURN_LEFT;
			}
			if(current_dir == left)
			{
				current_dir = up;
				return TURN_RIGHT;
			}
		}
		if(wumpus_dir == left)
		{
			if(current_dir == up)
			{	
				current_dir = left;
				return TURN_LEFT;
			}
			if(current_dir == right)
			{
				current_dir = up;
				return TURN_LEFT;
			}
			if(current_dir == down)
			{
				current_dir = left;
				return TURN_RIGHT;
			}		
		}
	}
	else
	{
		if(wumpus_live && arrow_fired == false)
		{
			arrow_fired = true;
			return SHOOT;
		}
	}
}
// ======================================================================
// YOUR CODE ENDS
// ======================================================================
