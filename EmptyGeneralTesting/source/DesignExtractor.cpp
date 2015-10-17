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
	return ParentT;
}

vector<int> DesignExtractor::extractChildrenT(vector<vector<int>> col,int stmtNum) {
	vector<int> children = col.at(stmtNum);
	int child;
	
	child = children.at(0);
	if (child == 0) {	
	} else {
		extractRec(children, col, "children");
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
			}
			else if (type.compare("calls") == 0) {
				CallsT.push_back(member);
			} else if(type.compare("calledBy")== 0) {
				CalledByT.push_back(member);
			} else if (type.compare("next")) {
				NextT.push_back(member);
			} else {
				PrevT.push_back(member);
			}
			extractRec(col.at(member), col,type);
		}
	}
}

std::vector<int> DesignExtractor::extractFollowsT(vector<int> col, int stmtNum) {
	int index = stmtNum;
	int num;

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
	return FollowsT;
}

std::vector<int> DesignExtractor::extractFollowedByT(vector<int> col, int stmtNum) {
	int index = stmtNum;
	int num;

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

	return FollowedByT;
}

std::vector<int> DesignExtractor::extractCallsT(vector<vector<int>> col, int stmtNum) {
	std::vector<int> calledProcedures = col.at(stmtNum);
	int proc;

	proc = calledProcedures.at(0);
	if (proc == 0) {
	}
	else {
		extractRec(calledProcedures, col,"calls");
	}
	return CallsT;

}

std::vector<int> DesignExtractor::extractCalledByT(vector<vector<int>>col, int stmtNum) {
	vector<int> callingProcedures = col.at(stmtNum);
	int proc;

	proc = callingProcedures.at(0);
	if (proc == 0) {
	}
	else {
		extractRec(callingProcedures, col, "calledBy");
	}
	return CalledByT;
}

vector<int> DesignExtractor::extractExtraProcModifiesUses(vector<int> existingList, vector<int> callsT, vector<vector<int>> col) {
	vector<int> result = existingList;
	for (int i = 0; i < callsT.size(); i++) {
		int proc = callsT.at(i);
		for (int j = 0; j < col.at(proc).size(); j++) {
			int varIndex = col.at(proc).at(j);
			if (find(existingList.begin(), existingList.end(), varIndex) != existingList.end()) {
				existingList.push_back(varIndex);
			}
		
		}
	}
	return result;
}

std::vector<int> DesignExtractor::extractNextT(vector<vector<int>> col, int stmtNum) {
	vector<int> next = col.at(stmtNum);
	int nextStmtNum;

	nextStmtNum = next.at(0);
	if (nextStmtNum == 0) {
	}
	else {
		extractRec(next, col, "next");
	}

	return NextT;
}

std::vector<int> DesignExtractor::extractPrevT(vector<vector<int>> col, int stmtNum) {
	vector<int> prev = col.at(stmtNum);
	int prevStmtNum;

	prevStmtNum = prev.at(0);
	if (prevStmtNum == 0) {
	}
	else {
		extractRec(prev, col, "prev");
	}

	return PrevT;
}