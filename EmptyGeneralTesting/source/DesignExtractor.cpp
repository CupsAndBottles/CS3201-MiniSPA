#pragma once
#include "DesignExtractor.h"
#include <list>
#include <algorithm>
#include <array>;
using namespace std;

DesignExtractor::DesignExtractor()
{
}

DesignExtractor::~DesignExtractor() {
}

vector<int> DesignExtractor::extractParentT(vector<int> col, int stmtNum) {
	int index = stmtNum;
	vector<int> parentCol = col;
	int parent = -1;

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
	int child = -1;

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
	int num = -1;

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
	int num = -1;

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
	
	int nextStmtNum = -1;

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

	
	//cout << NextT.size();
	return NextT;
}

std::vector<int> DesignExtractor::extractPrevT(vector<vector<int>> col, int stmtNum) {
	vector<int> prev = col.at(stmtNum);
	int prevStmtNum = -1;

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
	int firstIndex = -1;
	int secondIndex = 0;

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

			vector<int> singlePath = cfg.DFSOriginal(stmtNum1);
			//vector<int> path;
			//for (int i = 0; i < allPaths.size(); i++) {
				//path = allPaths.at(i);
				//cout << "Path " << i;
				//for (int j = 0; j < path.size(); j++) {
					//cout << path.at(j) << " ";
				//}
				//cout << "\n";
			//}
			//for (int i = 0; i < path.size(); i++) {
				//cout << path.at(i) << " ";
			//}
			if ((stmtNum1 != stmtNum2) && (find(singlePath.begin(), singlePath.end(), stmtNum2) == singlePath.end())) {
				return 0;
			}
			else {
				
				if (stmtNum1 != stmtNum2) {
					vector<vector<int>> allPaths = cfg.storeAllPaths(stmtNum1, stmtNum2);
					bool* exhaust = new bool[allPaths.size()];

					for (int i = 0; i < allPaths.size(); i++) {
						exhaust[i] = false;
					}
					int stmtNum;
					vector<int> modifies;
					vector<int> path;

					for (int j = 0; j < allPaths.size(); j++) {
						path = allPaths.at(j);
						for (int i = 1; i < path.size(); i++) {
							stmtNum = path.at(i);
							if (stmtNum == stmtNum2) {
								break;
							}
							else if (type.at(stmtNum) == Enum::TYPE::WHILE || type.at(stmtNum) == Enum::TYPE::IF) {
								continue;
							}
							else {
								modifies = modifiesCol.at(stmtNum);
								if (find(modifies.begin(), modifies.end(), modifiedVar) != modifies.end()) {
									if ((stmtNum < stmtNum1) && (stmtNum1 < stmtNum2)) {
									}
									else {
										vector<int> betweenPath;
										betweenPath = cfg.DFSOriginal(stmtNum);
										if (find(betweenPath.begin(), betweenPath.end(), stmtNum2) == betweenPath.end()) {
											continue;
										}

										exhaust[j] = true;
									
										break;
									}
								}
							}
						}
						int found = 0;
						for (int k = 0; k <= j ; k++) {
							if (exhaust[k] == false) {
								found = 1;
								break;
							}
						}
						if (found == 0) {
							return 0;
						}
						else {
							return 1;
						}
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
						return 0;
					}
					int max = whileStmt.at(0);
					for (int j = 1; j < whileStmt.size(); j++) {
						if (whileStmt.at(j) > max) {
							max = whileStmt.at(j);
						}
					}
		
					vector<int> children = childrenCol.at(max);
					int found = 0;
					vector<int> modified;
					int last = children.at(0);
					for (int k = 0; k < children.size(); k++) {
						//cout << "Child: " <<children.at(k) << "\n";
						if (children.at(k) > last) {
							last = children.at(k);
					}
					}
					vector<vector<int>> allPaths = cfg.storeAllPaths(max + 1, last);
					vector<int> path;
					bool* exhaust = new bool[allPaths.size()];
					//cout << last;
					for (int i = 0; i < allPaths.size(); i++) {
						exhaust[i] = false;
					}
					
					int betweenStmt;
					for (int j = 0; j <allPaths.size(); j++) {
						path = allPaths.at(j);
						for (int k = 0; k < path.size(); k++) {
							//cout << children.at(j) << " ";
							betweenStmt = path.at(k);
							if (type.at(betweenStmt) == Enum::TYPE::IF || type.at(betweenStmt) == Enum::TYPE::WHILE) {
								continue;
							}
							else {
								
								if (betweenStmt != stmtNum1) {
									modified = modifiesCol.at(betweenStmt);
									if (find(modified.begin(), modified.end(), modifiedVar) != modified.end()) {
										vector<int> betweenPath = cfg.DFSOriginal(betweenStmt);
										if (find(betweenPath.begin(), betweenPath.end(), stmtNum2) == betweenPath.end()) {
											continue;
										}
										
										exhaust[j] = true;
										break;
									}
								}
							}
						}
						for (int k = 0; k <= j; k++) {
							if (exhaust[k] == false) {
								found = 1;
								break;
							}
						}
						if (found == 0) {
							return 0;
						}
						else {
							return 1;
						}
					}

					

				}

			}
		}

	}
}

vector<pair<int, int>> DesignExtractor::extractAffectsFirstNum(int stmtNum1, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type, vector<vector<int>> parentTCol, vector<vector<int>> childrenCol) {

	int procStart, procEnd = -1;
	vector<pair<int, int>> results;
	vector<int> modifies;

	if (type.at(stmtNum1) != Enum::TYPE::ASSIGN) {
		return results;
	}
	for (int i = 0; i < startEndNum.size(); i++) {
		procStart = startEndNum.at(i).first;
		//cout << "Start" << procStart;
		procEnd = startEndNum.at(i).second;
		//cout << "End" <<procEnd;
		if ((procStart <= stmtNum1) && (stmtNum1 <= procEnd)) {
			break;
		}
		//cout << "\n";
	}
	//cout << procStart;
	//cout << procEnd;
	Graph cfg(procEnd+1);
	for (int i = procStart; i <= procEnd; i++) {
		//cout << "Stmt " << i;
		vector<int> list = nextCol.at(i);
		for (int j = 0; j < list.size(); j++) {
			cfg.addEdge(i, list.at(j));
			//cout << list.at(j) << " ";
		}
		//cout << "\n";
	}
	
	int stmtNum2 = -1;
	int modifiedVar = modifiesCol.at(stmtNum1).at(0);
	vector<int> usedVar;

	vector<int> singlePath = cfg.DFSOriginal(stmtNum1);
	
	vector<vector<int>> allPaths;
	bool* exhaust;
	vector<int> path;
	//cout << singlePath.size();
	vector<int> list = nextCol.at(stmtNum1);
	for (int j = 0; j < list.size(); j++) {
		cout << list.at(j);
	}
	for (int i = 1; i < singlePath.size(); i++) {
			stmtNum2 = singlePath.at(i);
			//cout << "StmtNum2 " << stmtNum2;
					if (type.at(stmtNum2) == Enum::TYPE::ASSIGN) {
						usedVar = usesCol.at(stmtNum2);
						if (find(usedVar.begin(), usedVar.end(), modifiedVar) != usedVar.end()) {
							if (i == 1) {
								results.push_back(make_pair(stmtNum1, stmtNum2));
							}
							else {
								allPaths = cfg.storeAllPaths(stmtNum1, stmtNum2);
								exhaust = new bool[allPaths.size()];
								//cout << stmtNum2 << " ";
								for (int j = 0; j < allPaths.size(); j++) {
									vector<int> list = allPaths.at(j);
									
								}
								for (int j = 0; j < allPaths.size(); j++) {
									exhaust[j] = false;
								}
								int betweenStmt;
								for (int j = 0; j < allPaths.size(); j++) {
									path = allPaths.at(j);
									for (int k = 1; k < path.size(); k++) {
										betweenStmt = path.at(k);
										if (betweenStmt == stmtNum2) {
											break;
										}
										if (type.at(betweenStmt) == Enum::TYPE::WHILE || type.at(betweenStmt) == Enum::TYPE::IF) {

											continue;
										}
										else {
											//cout << "Between stmt " << betweenStmt;
											modifies = modifiesCol.at(betweenStmt);
											for (int i = 0; i < modifies.size(); i++) {
											//	cout << "Modifies" << modifies.at(i);
											}
											if (find(modifies.begin(), modifies.end(), modifiedVar) != modifies.end()) {
												if ((betweenStmt < stmtNum1) && (stmtNum1 < stmtNum2)) {

												}
												else {
													vector<int> betweenPath;
													betweenPath = cfg.DFSOriginal(betweenStmt);
													if (find(betweenPath.begin(), betweenPath.end(), stmtNum2) == betweenPath.end()) {
														continue;
													}

													exhaust[j] = true;
													break;
												}
											}
										}
									}
									for (int k = 0; k <=j; k++) {
										if (exhaust[k] == false) {
											results.push_back(make_pair(stmtNum1, stmtNum2));
											break;
										}
									}
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
	int procStart, procEnd = -1;
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

	int stmtNum1 = 1;
	vector<int> usedVar = usesCol.at(stmtNum2);
	int modifiedVar;
	int betweenStmt, found = 0;
	vector<int> singlePath = cfg.DFSOriginal(stmtNum2);
	
	for (int i = 1; i < singlePath.size(); i++) {
		stmtNum1 = singlePath.at(i);
			if (type.at(stmtNum1) == Enum::TYPE::ASSIGN) {
				modifiedVar = modifiesCol.at(stmtNum1).at(0);
				if (find(usedVar.begin(), usedVar.end(), modifiedVar) != usedVar.end()) {
					vector<vector<int>> allPaths = cfg.storeAllPaths(stmtNum2, stmtNum1);
					bool* exhaust = new bool[allPaths.size()];
					vector<int> path;
					for (int j = 0; j < allPaths.size(); j++) {
						exhaust[j] = false;
					}

					//cout << allPaths.size();
					for (int j = 0; j<allPaths.size() ;j++ ) {
						path = allPaths.at(j);
						for (int k = 1; k < path.size(); k++) {
							betweenStmt = path.at(k);
							if (betweenStmt == stmtNum1) {
								break;
							}
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
										//for (int k = 0; k < betweenPath.size(); k++) {
										//	cout << betweenPath.at(k);
										//}
										if (find(betweenPath.begin(), betweenPath.end(), stmtNum1) == betweenPath.end()) {
											//	cout << "betweenStmt skipped " << betweenStmt << "for" << stmtNum1;
											continue;
										}
										
										exhaust[j] = true;
										break;
									}
								}
							}
						}
						for (int k = 0; k <=j; k++) {
							if (exhaust[k] == false) {
								results.push_back(make_pair(stmtNum1, stmtNum2));
								break;
							}
						}

					}

					
					
					
				}
		}
	}
	
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
	int firstIndex = -1;
	int secondIndex = 0;
	
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
			
			int check = -1;

				check = extractAffectsBothNum(stmtNum1, stmtNum2, modifiesCol, usesCol, nextCol, startEndNum, type, parentTCol, childrenCol);
				if (check == 1) {
					return 1;
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
					
					affects.at(path.at(i)) = list;
					list.clear();
			}
			
			Graph affectsGraph(endNum+1);
			int stmt = -1;
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
				int stmt = -1;
				vector<int> nums;
				for (int j = 0; j < affectsPath.size(); j++) {
					stmt = affectsPath.at(j);
					nums = affects.at(stmt);
					if (find(nums.begin(), nums.end(), stmtNum1) != nums.end()) {
						return 1;
					}
				}
				return 0;
			}
			else {
				if (find(affectsPath.begin() + 1, affectsPath.end(), stmtNum2) != affectsPath.end()) {
					return 1;
				}
				else {
					return 0;
				}
			}
		}
	}

}

vector<pair<int,int>> DesignExtractor::extractAffectsTFirstNum(int stmtNum1, vector<vector<int>> modifiesCol, vector<vector<int>> usesCol, vector<vector<int>> nextCol, vector<pair<int, int>> startEndNum, vector<int> type, vector<vector<int>> parentTCol, vector<vector<int>> childrenCol) {
	vector<pair<int, int>> results;
	int procStart, procEnd = -1;
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
	int check = -1;
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
	int procStart, procEnd = -1;
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
	int check = -1;
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