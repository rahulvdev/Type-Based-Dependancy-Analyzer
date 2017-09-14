#include "typetable.h"


//Constructor for TypeAnalysis
//Initializes AST,ScopeStack and Toker instances
TypeAnalysis::TypeAnalysis() :ASTref_(Repository::getInstance()->AST()),
scopeStack_(Repository::getInstance()->scopeStack()),
toker_(*(Repository::getInstance()->Toker())) {
	std::cout << "\n";
	std::cout << "---------------------REQUIREMENTS 3 and 4:Created TypeTable package for type analysis------------------------\n\n";
}

//Returns a list of all files from the typetable
//by attempting to match the token against each entry in the type table
std::vector<std::string> TypeAnalysis::returnDependentFiles(std::string token) {
	std::vector<std::string> depFileVect;
	std::vector<std::string> emptyVect;
	if (typeTable.find(token) != typeTable.end()) {
		std::vector<std::unordered_map<std::string, std::string>> mapVect = typeTable[token];
		for (std::vector<std::unordered_map<std::string, std::string>>::iterator it = mapVect.begin(); it != mapVect.end(); it++) {
			std::unordered_map<std::string, std::string> mapForTok = *it;
			depFileVect.push_back(mapForTok["filename"]);
		}
	}
	else if (funcMap.find(token) != funcMap.end() && token!="main") {
		depFileVect = funcMap[token];
	}
	else {
		//std::cout << "Token not found in the type table" << std::endl;
	}
	return depFileVect;
}

//Iniitializes type analysis
void TypeAnalysis::startTypeAnalysis()
{
	std::cout << "\n  starting type analysis:\n";
	std::cout << "\n  scanning AST and displaying important things:";
	std::cout << "\n -----------------------------------------------";
	ASTNode* pRoot = ASTref_.root();
	searchAST(pRoot);
	getGlobalFunctionInfo(Repository::getInstance()->getGlobalScope());
}

//Checks if node is either of class,struct or enum
bool TypeAnalysis::doDisplay(ASTNode* pNode)
{
	static std::string toDisplay[] = {
		"class","struct", "enum"
	};
	for (std::string type : toDisplay)
	{
		if (pNode->type_ == type)
			return true;
	}
	return false;
}

//Displays typetable content 
void TypeAnalysis::showTypeTable() {
	for (auto it : typeTable) {
		std::cout << std::endl;
		std::cout << it.first;
		std::cout << std::endl;
		std::cout << "----------------------------------" << std::endl;
		std::vector<std::unordered_map<std::string, std::string>> valueVect = it.second;
		for (std::vector<std::unordered_map <std::string, std::string>>::iterator it = valueVect.begin(); it != valueVect.end(); it++) {
			std::unordered_map <std::string, std::string> inst = *it;
			for (auto item : inst) {
				std::cout << std::setw(20)<<item.first << std::setw(20)<< item.second << std::endl;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "-------------------------------------" << std::endl;
	}
}

//Used to get global function information
void TypeAnalysis::getGlobalFunctionInfo(ASTNode * globalNode) {
	std::cout << "\n---------------------------------------\n";
	for (auto node : globalNode->children_) {
		if (node->type_ == "function") {
			if ((node->name_ != "main") && (node->name_ != "void"))
				storeGlobalFunction(node);
		}
	}
	std::cout << "\n---------------------------------------\n";
}


//Stores global function information
void TypeAnalysis::storeGlobalFunction(ASTNode* pNode)
{
	std::vector<std::string> filenames;
	if (funcMap.find(pNode->name_) == funcMap.end())
	{
		filenames.push_back(pNode->path_);
	}
	else
	{
		filenames = funcMap[pNode->name_];
		filenames.push_back(pNode->path_);
	}
	funcMap[pNode->name_] = filenames;
}

std::unordered_map<std::string, TypeAnalysis::vectStr>& TypeAnalysis::getGlobFuncMap()
{
	return funcMap;
}


//Depth first search over the abstract syntax tree
//Builds type table
void TypeAnalysis::searchAST(ASTNode* pNode)
{
	static std::string path = "";

	if (pNode->path_ != path)
	{
		std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
		path = pNode->path_;
	}
	if (doDisplay(pNode))
	{
		std::unordered_map<std::string, std::string> mapElem;
		mapElem["type"] = pNode->type_;
		mapElem["filename"] = pNode->path_;
		mapElem["namespace"] = namespaceStack.top();
		std::vector<std::unordered_map<std::string, std::string>> initVect;
		if (typeTable.find(pNode->name_) == typeTable.end()) {
			initVect.push_back(mapElem);
		}
		else {
			initVect = typeTable[pNode->name_];
			initVect.push_back(mapElem);

		}
		typeTable[pNode->name_] = initVect;
	}
	if (pNode->type_ == "namespace") {
		namespaceStack.push(pNode->name_);
	}
	for (auto pChild : pNode->children_) {
		searchAST(pChild);
	}
	if (pNode->type_ == "namespace") {
		namespaceStack.pop();
	}
}


#ifdef TYPETABLE

int main() {
	TypeAnalysis typAnal;
	typAnal.startTypeAnalysis();
	std::cout << "-------------------------------\n";
	std::cout << "Displaying Type Table Content\n";
	typAnal.showTypeTable();
	std::cout << "-------------------------------\n";
}

#endif // TYPETABLE
