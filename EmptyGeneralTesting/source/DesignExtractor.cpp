#pragma once
#include "DesignExtractor.h"
#include <list>;

using namespace std;

DesignExtractor::DesignExtractor()
{
}

DesignExtractor::~DesignExtractor() {
}

vector<int> DesignExtractor::extractParentT(vector<int> col, int stmtNum) {
	int index = stmtNum;
	vector<int> parentCol = col;
	int parent;

	if (parentCol.size() == 0) {
	}
	else {
		while (true) {
			parent = col.at(stmtNum);
			if (parent == 0) {
				break;
			}
			else {
				ParentT.push_back(parent);
				stmtNum = parent;
			}
		}
	}
	return ParentT;
}

vector<int> DesignExtractor::extractChildrenT(vector<vector<int>> col,int stmtNum) {
	vector<int> children = col.at(stmtNum);
	int child;

	if (children.size() == 0) {
	}
	else {
		child = children.at(0);
		if (child == 0) {
		}
		else {
			extractRec(children, col, "children");
		}
	}

	return ChildrenT;
}

void DesignExtractor::extractRec(std::vector<int> row, vector<vector<int>> col, string type) {
	for (int i = 0; i < (int)row.size(); i++) {
		int	member = row.at(i);
	
		if (member == 0) {
			return;
		}
		else {
			if (type.compare("children") == 0) {
				ChildrenT.push_back(member);
			} else if (type.compare("next")== 0) {
				if (find(NextT.begin(), NextT.end(), member) == NextT.end()) {
					NextT.push_back(member);
				}
				else {
					continue;
				}
			} else {
				if (find(PrevT.begin(), PrevT.end(), member) == PrevT.end()) {
					PrevT.push_back(member);
				}
				else {
					continue;
				}
			}
			extractRec(col.at(member), col,type);
		}
	}
}

void DesignExtractor::extractRecCallsTCalledByT(vector<int> row, vector<vector<int>> col, string type) {
	if (row.size()==0) {
		return;
	}
	else {
		for (int i = 0; i < (int)row.size(); i++) {
			int	member = row.at(i);
			if (type.compare("calls") == 0) {
				CallsT.push_back(member);
			}
			else {
				CalledByT.push_back(member);
			}
			extractRecCallsTCalledByT(col.at(member), col, type);
		}
	}
}

std::vector<int> DesignExtractor::extractFollowsT(vector<int> col, int stmtNum) {
	int index = stmtNum;
	int num;

	if (col.size() == 0) {
	}
	else {
		while (true) {
			num = col.at(stmtNum);
			if (num == 0) {
				break;
			}
			else {
				FollowsT.push_back(num);
				stmtNum = num;
			}
		}
	}
	return FollowsT;
}

std::vector<int> DesignExtractor::extractFollowedByT(vector<int> col, int stmtNum) {
	int index = stmtNum;
	int num;

	if (col.size() == 0) {

	}
	else {
		while (true) {
			num = col.at(stmtNum);
			if (num == 0) {
				break;
			}
			else {
				FollowedByT.push_back(num);
				stmtNum = num;
			}
		}
	}

	return FollowedByT;
}

std::vector<int> DesignExtractor::extractCallsT(vector<vector<int>> col, int stmtNum) {
	std::vector<int> calledProcedures = col.at(stmtNum);

	if (calledProcedures.size() == 0) {
	}
	else {
		
		extractRecCallsTCalledByT(calledProcedures, col, "calls");
	}
	return CallsT;

}

std::vector<int> DesignExtractor::extractCalledByT(vector<vector<int>>col, int stmtNum) {
	vector<int> callingProcedures = col.at(stmtNum);
	
	if (callingProcedures.size() == 0) {

	}
	else {
		
			extractRecCallsTCalledByT(callingProcedures, col, "calledBy");
		
	}
	return CalledByT;
}

vector<vector<int>> DesignExtractor::extractProcModifiesUses(vector<vector<int>>calls, vector<vector<int>> col) {
	Graph graph(calls.size());
	
	for (int i = 0; i < calls.size(); i++) {
		vector<int> list = calls.at(i);
		for (int j = 0; j < list.size(); j++) {
			graph.addEdge(i, list.at(j));
		}
	}

	graph.col = col;
	if (col.size()==0) {
	}
	else {
		for (int i = 0; i < calls.size(); i++) {
			updatedCol = graph.DFS(i);
		}
	}


	return updatedCol;
}

std::vector<int> DesignExtractor::extractNextT(vector<vector<int>> col, int stmtNum) {
	vector<int> next = col.at(stmtNum);
	int nextStmtNum;

	if (next.size() == 0) {
		//cout << "zero size";
	}
	else {
		nextStmtNum = next.at(0);

		if (nextStmtNum == 0) {
		}
		else {
			extractRec(next, col, "next");
		}
	}

	return NextT;
}

std::vector<int> DesignExtractor::extractPrevT(vector<vector<int>> col, int stmtNum) {
	vector<int> prev = col.at(stmtNum);
	int prevStmtNum;

	if (prev.size() == 0) {

	}
	else {}
	prevStmtNum = prev.at(0);
	if (prevStmtNum == 0) {
	}
	else {
		extractRec(prev, col, "prev");
	}

	return PrevT;
}

int DesignExtractor::extractAffectsBothNum(int stmtNum1, int stmtNum2, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum) {
	
	int firstIndex;
	int secondIndex;

	for (int i = 0; i < startEndNum.size(); i++) {
		int start = startEndNum.at(i).first;
		int end = startEndNum.at(i).second;
		if ((start <= stmtNum1) && (stmtNum1 <= end)) {
			firstIndex = i;
		}

		if ((start <= stmtNum2) && (stmtNum2 <= end)) {
			secondIndex = i;
		}
	}

	if (firstIndex != secondIndex) {
		return 0;
	}
	else {
		int modifiedVar = modifiesCol.at(stmtNum1).at(0);
		vector<int> usedVars = usesCol.at(stmtNum2);
		if (find(usedVars.begin(), usedVars.end(), modifiedVar) == usedVars.end()) {
			return 0;
		}
		else {
			//cfg

			int endNum = startEndNum.at(firstIndex).second;
			int startNum =	startEndNum.at(firstIndex).first;
			
			Graph cfg(endNum);

			for (int i = startNum; i <= endNum; i++) {
				vector<int> list = nextCol.at(i);
				for (int j = 0; j < list.size(); j++) {
					cfg.addEdge(i, list.at(j));
				}
			}

			vector<int> path = cfg.DFSOriginal(stmtNum1);
			if (find(path.begin(), path.end(), stmtNum2) == path.end()) {
				return 0;
			}
			else {
				int stmtNum;
				vector<int> modifies;
				int found = 0;
				for (int i = 1; i < path.size(); i++) {
					stmtNum = path.at(i);
					if (stmtNum == stmtNum2) {
						break;
					}
					else {
						modifies = modifiesCol.at(stmtNum);
						if (find(modifies.begin(), modifies.end(), modifiedVar) != modifies.end()) {
							found = 1;
							break;
						}
					}
				}
				if (found == 0) {
					return 1;
				}
				else {
					return 0;
				}

			}
		}

	}
}

vector<pair<int, int>> DesignExtractor::extractAffectsFirstNum(int stmtNum1, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type) {

	int procStart, procEnd;
	vector<pair<int, int>> results;
	vector<int> modifies;

	for (int i = 0; i < startEndNum.size(); i++) {
		procStart = startEndNum.at(i).first;
		procEnd = startEndNum.at(i).second;
		if ((procStart <= stmtNum1) && (stmtNum1 <= procEnd)) {
			break;
		}
	}

	Graph cfg(procEnd);
	for (int i = procStart; i <= procEnd; i++) {
		vector<int> list = nextCol.at(i);
		for (int j = 0; j < list.size(); j++) {
			cfg.addEdge(i, list.at(j));
		}
	}

	int stmtNum2;
	int modifiedVar = modifiesCol.at(stmtNum1).at(0);
	vector<int> usedVar;
	int betweenStmt;

	vector<int> path = cfg.DFSOriginal(stmtNum1);
	for (int i = 1; i < path.size(); i++) {
		stmtNum2 = path.at(i);
		if (type.at(stmtNum2) == Enum::TYPE::ASSIGN) {
			usedVar = usesCol.at(stmtNum2);
			if (find(usedVar.begin(), usedVar.end(), modifiedVar) != usedVar.end()) {
				if (i == 1) {
					results.push_back(make_pair(stmtNum1, stmtNum2));
				}
				else {
					for (int j = 1; j < i; j++) {
						betweenStmt = path.at(j);
						modifies = modifiesCol.at(betweenStmt);
						if (find(modifies.begin(), modifies.end(), modifiedVar) == modifies.end()) {
							results.push_back(make_pair(stmtNum1, stmtNum2));
						}
					}
				}
			}
		}
	}
	return results;
}

vector<pair<int, int>> DesignExtractor::extractAffectsSecondNum(int stmtNum2, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type) {
	int procStart, procEnd;
	vector<pair<int, int>> results;
	vector<int> modifies;

	for (int i = 0; i < startEndNum.size(); i++) {
		procStart = startEndNum.at(i).first;
		procEnd = startEndNum.at(i).second;
		if ((procStart <= stmtNum2) && (stmtNum2 <= procEnd)) {
			break;
		}
	}

	Graph cfg(procEnd);
	for (int i = procStart; i <= procEnd; i++) {
		vector<int> list = nextCol.at(i);
		for (int j = 0; j < list.size(); j++) {
			cfg.addEdge(i, list.at(j));
		}
	}
	int stmtNum1;
	vector<int> usedVar = usesCol.at(stmtNum2);
	int modifiedVar;
	int betweenStmt;

	vector<int> path = cfg.DFSOriginal(procStart);
	for (int i = 0; i < path.size(); i++) {
		stmtNum1 = path.at(i);
		if (type.at(stmtNum1) == Enum::TYPE::ASSIGN) {
			modifiedVar = modifiesCol.at(stmtNum1).at(0);
			if (find(usedVar.begin(), usedVar.end(), modifiedVar) != usedVar.end()) {
				
					for (int j = 0; j <path.size(); j++) {
						betweenStmt = path.at(j);
						modifies = modifiesCol.at(betweenStmt);
						if (find(modifies.begin(), modifies.end(), modifiedVar) == modifies.end()) {
							results.push_back(make_pair(stmtNum1, stmtNum2));
						}
					}
			}
		}
	}
	return results;
}