#pragma once
#include "DesignExtractor.h"
#include <list>
#include <algorithm>

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

int DesignExtractor::extractAffectsBothNum(int stmtNum1, int stmtNum2, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type, vector<vector<int>> parentTCol, vector<vector<int>> childrenCol) {
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
			int startNum = startEndNum.at(firstIndex).first;

			Graph cfg(endNum + 1);

			for (int i = startNum; i <= endNum; i++) {
				vector<int> list = nextCol.at(i);
				for (int j = 0; j < list.size(); j++) {
					cfg.addEdge(i, list.at(j));
				}
			}

			vector<int> path = cfg.DFSOriginal(stmtNum1);
			for (int i = 0; i < path.size(); i++) {
				cout << path.at(i) << " ";
			}
			if ((stmtNum1 != stmtNum2) && (find(path.begin(), path.end(), stmtNum2) == path.end())) {
				return 0;
			}
			else {
				int stmtNum;
				vector<int> modifies;
				int found = 0;
				if (stmtNum1 != stmtNum2) {
					for (int i = 1; i < path.size(); i++) {
						stmtNum = path.at(i);
						if (stmtNum == stmtNum2) {
							break;
						}
						else if (type.at(stmtNum) == Enum::TYPE::WHILE || type.at(stmtNum) == Enum::TYPE::IF) {
							/*if (type.at(stmtNum) == Enum::TYPE::IF && stmtNum>stmtNum1) {
								vector<int> children = childrenCol.at(stmtNum);
								if (find(children.begin(), children.end(), stmtNum2) == children.end()) {
									int max = children.at(0);
									for (int k = 1; k < children.size(); k++) {
										if (children.at(k) > max) {
											max = children.at(k);
										}
									}
									vector<int> start = nextCol.at(stmtNum);
									int elseStmtStart;
									if (start.at(0) == stmtNum + 1) {
										elseStmtStart = start.at(1);
									}
									else {
										elseStmtStart = start.at(0);
									}
									bool check = false;
									for (int j = stmtNum + 1; j < elseStmtStart; j++) {
										if (type.at(j) != Enum::TYPE::IF || type.at(j)!= Enum::TYPE::WHILE) {
											vector<int> modifies = modifiesCol.at(j);
											if (find(modifies.begin(), modifies.end(), modifiedVar) != modifies.end()) {
												check = true;
											}
										}
										
									}
									bool check1 = false;
									for (int j = elseStmtStart; j<=max; j++) {
										if (type.at(j) != Enum::TYPE::IF || type.at(j) != Enum::TYPE::WHILE) {
											vector<int> modifies = modifiesCol.at(j);
											if (find(modifies.begin(), modifies.end(), modifiedVar) != modifies.end()) {
												check = true;
											}
										}
									}
									if (check == true && check1 == true) {
										return 0;
									}
									else if ((check == true && check1 == false) || (check== false && check1 == true)) {
										vector<int> next = nextCol.at(max);
										int num;
										if (next.size() == 1) {
											num = next.at(0);
										}
										else {
											if (next.at(0) < next.at(1)) {
												num = next.at(0);
											}
											else {
												num = next.at(1);
											}
										}
										for (int k = 0; k < path.size(); k++) {
											if (path.at(k) >= num) {
												i = k;
											}
										}
									}
								}
							}*/
							continue;
						}
						else {
							modifies = modifiesCol.at(stmtNum);
							if (find(modifies.begin(), modifies.end(), modifiedVar) != modifies.end()) {
								if ((stmtNum < stmtNum1) && (stmtNum1 < stmtNum2)) {
								} else {
									vector<int> betweenPath;
									betweenPath = cfg.DFSOriginal(stmtNum);
									if (find(betweenPath.begin(), betweenPath.end(), stmtNum2) == betweenPath.end()) {
										continue;
									}
									
									found = 1;
									//cout << "This is it: " << stmtNum;
									break;
								}
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
				else {
					vector<int> parentT = parentTCol.at(stmtNum2);
					vector<int> whileStmt;
					int stmt;
					if (parentT.size() == 0) {
						return 0;
					} 
					for (int i = 0; i < parentT.size(); i++) {
						stmt = parentT.at(i);
						if (type.at(stmt) == Enum::TYPE::WHILE) {
							whileStmt.push_back(stmt);
						}
					}
					if (whileStmt.size() == 0) {
						//cout << "Entered\n";
						return 0;
					}
					int max = whileStmt.at(0);
					for (int j = 1; j < whileStmt.size(); j++) {
						if (whileStmt.at(j) > max) {
							max = whileStmt.at(j);
						}
					}
					
					vector<int> children = childrenCol.at(max);
					
					vector<int> modified;
					for (int j = 0; j <children.size(); j++) {
						if (type.at(children.at(j)) == Enum::TYPE::IF) {
							continue;
						}
						else {
							if ((find(path.begin(), path.end(), children.at(j)) != path.end()) && (children.at(j)!= stmtNum1)) {
								modified = modifiesCol.at(children.at(j));
								if (find(modified.begin(), modified.end(), modifiedVar) != modified.end()) {
									found = 1;
								}
							}
						}

					}

					if(found ==0) {
						return 1;
					}
					else {
						return 0;
					}

				}

			}
		}

	}
}

vector<pair<int, int>> DesignExtractor::extractAffectsFirstNum(int stmtNum1, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type, vector<vector<int>> parentTCol, vector<vector<int>> childrenCol) {

	int procStart, procEnd;
	vector<pair<int, int>> results;
	vector<int> modifies;

	if (type.at(stmtNum1) != Enum::TYPE::ASSIGN) {
		return results;
	}
	for (int i = 0; i < startEndNum.size(); i++) {
		procStart = startEndNum.at(i).first;
		procEnd = startEndNum.at(i).second;
		if ((procStart <= stmtNum1) && (stmtNum1 <= procEnd)) {
			break;
		}
	}

	Graph cfg(procEnd+1);
	for (int i = procStart; i <= procEnd; i++) {
		vector<int> list = nextCol.at(i);
		for (int j = 0; j < list.size(); j++) {
			cfg.addEdge(i, list.at(j));
		}
	}

	int stmtNum2;
	int modifiedVar = modifiesCol.at(stmtNum1).at(0);
	vector<int> usedVar;
	int betweenStmt, found = 0;

	vector<int> path = cfg.DFSOriginal(stmtNum1);
	for (int i = 0; i < path.size(); i++) {
		//cout << path.at(i);
	}
	vector<int> stmts;
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
						if (type.at(betweenStmt) == Enum::TYPE::WHILE || type.at(betweenStmt) == Enum::TYPE::IF) {
							if (type.at(betweenStmt) == Enum::TYPE::IF && betweenStmt>stmtNum1) {
								vector<int> children = childrenCol.at(betweenStmt);
								if (find(children.begin(), children.end(), stmtNum2) == children.end()) {
									int max = children.at(0);
									for (int k = 1; k < children.size(); k++) {
										if (children.at(k) > max) {
											max = children.at(k);
										}
									}
									vector<int> start = nextCol.at(betweenStmt);
									int elseStmtStart;
									if (start.at(0) == betweenStmt + 1) {
										elseStmtStart = start.at(1);
									}
									else {
										elseStmtStart = start.at(0);
									}
									bool check = false;
									vector<int> ifModifies, elseModifies;
									for (int j = betweenStmt + 1; j < elseStmtStart; j++) {
										if (type.at(j) != Enum::TYPE::IF || type.at(j) != Enum::TYPE::WHILE) {
											 ifModifies = modifiesCol.at(j);
											if (find(ifModifies.begin(), ifModifies.end(), modifiedVar) != ifModifies.end()) {
												check = true;
											}
										}

									}
									bool check1 = false;
									for (int j = elseStmtStart; j <= max; j++) {
										if (type.at(j) != Enum::TYPE::IF || type.at(j) != Enum::TYPE::WHILE) {
											elseModifies = modifiesCol.at(j);
											if (find(elseModifies.begin(), elseModifies.end(), modifiedVar) != elseModifies.end()) {
												check = true;
											}
										}
									}
									
									if (check == true && check1 == true) {
										//return results;
									}
									else if ((check == true && check1 == false) || (check == false && check1 == true)) {
										if (check == true && check1 == false) {
											for (int k = betweenStmt + 1; k < elseStmtStart; k++) {
												stmts.push_back(k);
											}
											
										}
										else {
											for (int k = elseStmtStart; k <= max; k++) {
												stmts.push_back(k);
											}
										}
										vector<int> next = nextCol.at(max);
										int num;
										if (next.size() == 1) {
											num = next.at(0);
										}
										else {
											if (next.at(0) < next.at(1)) {
												num = next.at(0);
											}
											else {
												num = next.at(1);
											}
										}
										for (int k = 0; k < path.size(); k++) {
											if (path.at(k) >= num) {
												i = k;
											}
										}
									}
								}
							}
							continue;
						}
						else {
							modifies = modifiesCol.at(betweenStmt);
							if (find(modifies.begin(), modifies.end(), modifiedVar) != modifies.end()) {
								if ((betweenStmt < stmtNum1) && (stmtNum1 < stmtNum2)) {

								}
								else {
									vector<int> betweenPath;
									betweenPath = cfg.DFSOriginal(betweenStmt);
									if (find(betweenPath.begin(), betweenPath.end(), stmtNum2) == betweenPath.end()) {
										continue;
									} if (find(stmts.begin(), stmts.end(), betweenStmt) != stmts.end()) {
										continue;
									}

									cout << "Start";
									for (int k = 0; k < stmts.size(); k++) {
										cout << stmts.at(k);
									}
									found = 1;
									cout << "Stmt: " << betweenStmt;
									break;
								}
							}
						}
					}
					if (found == 0) {
						results.push_back(make_pair(stmtNum1, stmtNum2));
					}
				}
			}
		}
	}

	if (extractAffectsBothNum(stmtNum1, stmtNum1, modifiesCol, usesCol, nextCol, startEndNum, type, parentTCol, childrenCol)== 1) {
		results.push_back(make_pair(stmtNum1, stmtNum1));
	}

	return results;
}

vector<pair<int, int>> DesignExtractor::extractAffectsSecondNum(int stmtNum2, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> prevCol, vector<pair<int, int>> startEndNum, vector<int> type, vector<vector<int>> parentTCol, vector<vector<int>> childrenCol) {
	int procStart, procEnd;
	vector<pair<int, int>> results;
	vector<int> modifies;

	if (type.at(stmtNum2) != Enum::TYPE::ASSIGN) {
		return results;
	}
	for (int i = 0; i < startEndNum.size(); i++) {
		procStart = startEndNum.at(i).first;
		procEnd = startEndNum.at(i).second;
		if ((procStart <= stmtNum2) && (stmtNum2 <= procEnd)) {
			break;
		}
	}

	Graph cfg(procEnd+1);
	for (int i = procStart; i <= procEnd; i++) {
		vector<int> list = prevCol.at(i);
		for (int j = 0; j < list.size(); j++) {
			cfg.addEdge(i, list.at(j));
		}
	}

	int stmtNum1;
	vector<int> usedVar = usesCol.at(stmtNum2);
	int modifiedVar;
	int betweenStmt, found = 0;
	vector<int> path = cfg.DFSOriginal(stmtNum2);
	for (int i = 1; i < path.size(); i++) {
		stmtNum1 = path.at(i);
			if (type.at(stmtNum1) == Enum::TYPE::ASSIGN) {
				modifiedVar = modifiesCol.at(stmtNum1).at(0);
				if (find(usedVar.begin(), usedVar.end(), modifiedVar) != usedVar.end()) {
					for (int j = i -1; j > 0; j--) {
							betweenStmt = path.at(j);
							if (type.at(betweenStmt) == Enum::TYPE::WHILE || type.at(betweenStmt) == Enum::TYPE::IF) {
								continue;
							}
							else {
								modifies = modifiesCol.at(betweenStmt);
								if (find(modifies.begin(), modifies.end(), modifiedVar) != modifies.end()) {
									if (betweenStmt < stmtNum1 && stmtNum1 < stmtNum2) {
									}
									else {
									vector<int> betweenPath;
									betweenPath = cfg.DFSOriginal(betweenStmt);
									for (int k = 0; k < betweenPath.size(); k++) {
										cout << betweenPath.at(k);
									}
									if (find(betweenPath.begin(), betweenPath.end(), stmtNum1) == betweenPath.end()) {
										cout << "betweenStmt skipped " << betweenStmt << "for" << stmtNum1;
										continue;
									}
										found = 1;
										break;
									}
								}
							}
					}
					if (found == 0){
						results.push_back(make_pair(stmtNum1, stmtNum2));
					}
					found = 0;
				}
		}
	}
	/*
	if (parentTCol.at(stmtNum2).size() != 0) {
		vector<int> parentT = parentTCol.at(stmtNum2);
		vector<int> whileStmt;
		int stmt;

		for (int i = 0; i < parentT.size(); i++) {
			stmt = parentT.at(i);
			if (type.at(stmt) == Enum::TYPE::WHILE) {
				whileStmt.push_back(stmt);
			}
		}

		if (whileStmt.size() > 0) {
			int min = whileStmt.at(0);
			for (int j = 1; j < whileStmt.size(); j++) {
				if (whileStmt.at(j) < min) {
					min = whileStmt.at(j);
				}
			}

			vector<int> children = childrenCol.at(min);
			sort(children.begin(), children.end());
			size_t start = find(children.begin(), children.end(), stmtNum2) - children.begin();
			for (size_t j = start; j < children.size(); j++) {
				if (type.at(children.at(j)) == Enum::TYPE::IF) {
					continue;
				}
				else {
					if (extractAffectsBothNum(children.at(j), stmtNum2, modifiesCol, usesCol, nextCol, startEndNum, type, parentTCol, childrenCol) == 1) {
						results.push_back(make_pair(children.at(j), stmtNum2));
					}
				}
			}
		}
		}
	*/
	if (extractAffectsBothNum(stmtNum2, stmtNum2, modifiesCol, usesCol, prevCol, startEndNum, type, parentTCol, childrenCol) == 1) {
		results.push_back(make_pair(stmtNum2, stmtNum2));
	}
	return results;
}

vector<pair<int, int>> DesignExtractor::extractAffectsBothUnspecified(vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type, vector<vector<int>> parentTCol, vector<vector<int>> childrenCol) {
	
	vector<pair<int, int>> results;
	vector < pair<int, int>> returnResults;
	for (int i = 0; i < startEndNum.size(); i++) {

		int startNum = startEndNum.at(i).first;
		int endNum = startEndNum.at(i).second;

		for (int j = startNum; j <= endNum; j++) {
			if (type.at(j) == Enum::TYPE::ASSIGN) {
				returnResults = extractAffectsFirstNum(j, modifiesCol, usesCol, nextCol, startEndNum, type, parentTCol, childrenCol);
			}
			for (int k = 0; k < returnResults.size(); k++) {
				results.push_back(returnResults.at(k));
			}
		}
	}
	return results;
}

int DesignExtractor::extractAffectsTBothNum(int stmtNum1, int stmtNum2, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type, vector<vector<int>> parentTCol, vector<vector<int>> childrenCol) {
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
		int endNum = startEndNum.at(firstIndex).second;
		int startNum = startEndNum.at(firstIndex).first;

		Graph cfg(endNum + 1);
		
		for (int i = startNum; i <= endNum; i++) {
			vector<int> list = nextCol.at(i);
			for (int j = 0; j < list.size(); j++) {
				cfg.addEdge(i, list.at(j));
			}
		}
		//cout << "Entered here\n";
		vector<int> path = cfg.DFSOriginal(stmtNum1);
			if ((stmtNum1 != stmtNum2) && (find(path.begin(), path.end(), stmtNum2) == path.end())) {
			return 0;
		}
		else {
			if (stmtNum1 == stmtNum2) {
				int check;

				check = extractAffectsBothNum(stmtNum1, stmtNum2, modifiesCol, usesCol, nextCol, startEndNum, type, parentTCol, childrenCol);
				if (check == 1) {
					return 1;
				}
			}
			vector<pair<int,int>> affectsIntermediateResults;
			vector<int> list;
			vector <vector<int>> affects(endNum+1);
			for (int i = 0; i < path.size(); i++) {
					affectsIntermediateResults = extractAffectsFirstNum(path.at(i), modifiesCol, usesCol, nextCol, startEndNum, type, parentTCol, childrenCol);
					//cout << "Stmt" << path.at(i) << ": ";
					for (int j = 0; j < affectsIntermediateResults.size(); j++) {
						list.push_back(affectsIntermediateResults.at(j).second);
					}
					for (int k = 0; k < list.size(); k++) {
						//cout << list.at(k) << " ";
					}
					affects.at(path.at(i)) = list;
					list.clear();
			}
			if (stmtNum1 == stmtNum2) {
				int stmt;
				vector<int> nums;
				for (int j = 0; j < path.size(); j++) {
					stmt = path.at(j);
					nums = affects.at(stmt);
					if (find(nums.begin(), nums.end(), stmtNum1) != nums.end()) {

					}
				}
			}
			Graph affectsGraph(endNum+1);
			int stmt;
			vector<int> stmts;
			for (int j = 0; j < path.size(); j++) {
					stmt = path.at(j);
					stmts = affects.at(stmt);
					for (int k = 0; k < stmts.size(); k++) {
						affectsGraph.addEdge(stmt, stmts.at(k));
					}
			}

			vector<int> affectsPath = affectsGraph.DFSOriginal(stmtNum1);
					
			if (stmtNum1 == stmtNum2) {
				int stmt;
				vector<int> nums;
				for (int j = 0; j < affectsPath.size(); j++) {
					stmt = affectsPath.at(j);
					nums = affects.at(stmt);
					if (find(nums.begin(), nums.end(), stmtNum1) != nums.end()) {
						return 1;
					}
				}
			}
			if (find(affectsPath.begin()+1, affectsPath.end(), stmtNum2) != affectsPath.end()) {
				return 1;
			}
			else {
				return 0;
			}
		}
	}

}

vector<pair<int,int>> DesignExtractor::extractAffectsTFirstNum(int stmtNum1, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type, vector<vector<int>> parentTCol, vector<vector<int>> childrenCol) {
	vector<pair<int, int>> results;
	int procStart, procEnd;
	vector<int> modifies;

	if (type.at(stmtNum1) != Enum::TYPE::ASSIGN) {
		return results;
	}
	for (int i = 0; i < startEndNum.size(); i++) {
		procStart = startEndNum.at(i).first;
		procEnd = startEndNum.at(i).second;
		if ((procStart <= stmtNum1) && (stmtNum1 <= procEnd)) {
			break;
		}
	}

	Graph cfg(procEnd + 1);
	for (int i = procStart; i <= procEnd; i++) {
		vector<int> list = nextCol.at(i);
		for (int j = 0; j < list.size(); j++) {
			cfg.addEdge(i, list.at(j));
		}
	}

	vector<int> path = cfg.DFSOriginal(stmtNum1);
	int check;
	for (int i = 0; i < path.size(); i++) {
		if (type.at(path.at(i)) == Enum::TYPE::ASSIGN) {
			check = extractAffectsTBothNum(stmtNum1, path.at(i), modifiesCol, usesCol, nextCol, startEndNum, type, parentTCol, childrenCol);
			if (check == 1) {
				results.push_back(make_pair(stmtNum1, path.at(i)));
			}
		}	
	}
	return results;
}

vector<pair<int, int>> DesignExtractor::extractAffectsTSecondNum(int stmtNum2, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> prevCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type, vector < vector<int>> parentTCol, vector<vector<int>> childrenCol) {

	vector<pair<int, int>> results;
	int procStart, procEnd;
	vector<int> modifies;

	for (int i = 0; i < startEndNum.size(); i++) {
		procStart = startEndNum.at(i).first;
		procEnd = startEndNum.at(i).second;
		if ((procStart <= stmtNum2) && (stmtNum2 <= procEnd)) {
			break;
		}
	}

	Graph cfg(procEnd + 1);
	for (int i = procStart; i <= procEnd; i++) {
		vector<int> list = prevCol.at(i);
		for (int j = 0; j < list.size(); j++) {
			cfg.addEdge(i, list.at(j));
		}
	}

	vector<int> path = cfg.DFSOriginal(stmtNum2);
	for (int i = 0; i<path.size(); i++) {
	//	cout << path.at(i) << " ";
	}
	//cout << "\n";
	int check;
	for (int i = 0; i < path.size(); i++) {
		if (type.at(path.at(i)) == Enum::TYPE::ASSIGN) {
			check = extractAffectsTBothNum(path.at(i), stmtNum2, modifiesCol, usesCol, nextCol, startEndNum, type, parentTCol, childrenCol);
			if (check == 1) {
				results.push_back(make_pair(path.at(i), stmtNum2));
			}
		}
		
	}
	return results;
}

vector<pair<int,int>> DesignExtractor::extractAffectsTBothUnspecified(vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type, vector<vector<int>> parentTCol, vector<vector<int>> childrenCol) {
	
	vector<pair<int, int>> results;
	vector < pair<int, int>> returnResults;
	for (int i = 0; i < startEndNum.size(); i++) {

		int startNum = startEndNum.at(i).first;
		int endNum = startEndNum.at(i).second;

		for (int j = startNum; j <= endNum; j++) {
			if(type.at(j)== Enum::TYPE::ASSIGN) {
				returnResults = extractAffectsTFirstNum(j, modifiesCol, usesCol, nextCol, startEndNum, type, parentTCol, childrenCol);
			}
			for (int k = 0; k < returnResults.size(); k++) {
				results.push_back(returnResults.at(k));
			}
		}
	}
	return results;
}