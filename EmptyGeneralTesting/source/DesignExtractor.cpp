#pragma once
#include "DesignExtractor.h"

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
			} else if (type.compare("next")) {
				NextT.push_back(member);
			} else {
				PrevT.push_back(member);
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

vector<vector<int>> DesignExtractor::extractExtraProcModifies(vector<vector<int>> calls, vector<vector<int>> modifiesCol) {
	vector<int> updated;
	updatedModifiesCol = modifiesCol;

	for (int i = 0; i < calls.size(); i++) {
		updated.push_back(0);
	}

	for (int i = 0; i < calls.size(); i++) {
		if (calls.at(i).size() == 0) {
			updated.at(i) = 1;
		}
	}

	for (int j = 0; j < calls.size(); j++) {
		if (updated.at(j) == 0) {
			extractRecProcModifies(j, calls.at(j), calls, updatedModifiesCol, updated);
		}
	}
			//if (find(result.begin(), result.end(), varIndex) == result.end()) {
				//result.push_back(varIndex);
			//}
		
	return modifiesCol;
}

void extractRecProcModifies(int index, vector<int> currentCallsList, vector<vector<int>> callsCol, vector<vector<int>> updatedModifiesCol, vector<int> updated) {
	if (updated.at(index)) {
		return;
	}

	else {
		for (int i = 0; i < currentCallsList.size(); i++) {
			if (updated.at(currentCallsList.at(i))) {
				vector<int> extra = updatedModifiesCol.at(currentCallsList.at(i));
				vector<int>	existingList = updatedModifiesCol.at(index);
				for (int j = 0; j < extra.size();j++) {
					int varIndex = extra.at(j);
				if (find(existingList.begin(), existingList.end(), varIndex) == existingList.end()) {
				existingList.push_back(varIndex);
				}
			} else {
					extractRecProcModifies(currentCallsList.at(i), callsCol.at(currentCallsList.at(i)), callsCol, updatedModifiesCol, updated);
			}
		}
		updated.at(index) = 1;
	}
}

std::vector<int> DesignExtractor::extractNextT(vector<vector<int>> col, int stmtNum) {
	vector<int> next = col.at(stmtNum);
	int nextStmtNum;

	if (next.size() == 0) {

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
