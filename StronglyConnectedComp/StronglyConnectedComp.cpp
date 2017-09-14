#include "StronglyConnectedComp.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"

Graph_SCC::Graph_SCC()
{
	
}

//Entry point to graph creation
//Fetches content from dependancy table for graph creation
std::unordered_map<std::string, int> Graph_SCC::createGraph(DependancyAnalysis &dp,Files filesInLoc)
{
	
	fileVect = filesInLoc;
	noOfVertices = createMap();
	adjacency_matrix = new std::list<int>[noOfVertices];
	createAdjacencyMatrix(dp);

	return mapOfFiles;
}

//Maps each file in the dependancy table to a unique integer value
//Used for graph creation
int Graph_SCC::createMap()
{
	int i = 0;
	for (File x : fileVect)
	{
		mapOfFiles[x] = i;
		i++;
	}
	return i;
}

//Creating Adjacency matrix to associate each node in the graph to its
//connected node
void Graph_SCC::createAdjacencyMatrix(DependancyAnalysis &dp)
{
	//adjacency_matrix = new std::list<int>[verticesCount];
	 ;

	Files files = dp.getDbInstance().getKeys();
	for (File file : files)
	{
		Files listFiles = dp.getDbInstance().getValue(file).children.getValue();
		int index = mapOfFiles[file];
		/*int index = getID(file);*/
		for (File x : listFiles)
		{
			adjacency_matrix[index].push_back(mapOfFiles[x]);
		}
	}
}

//returns adjacency matrix instance
std::list<int>* Graph_SCC::getAdjacencyMatrix()
{
	return adjacency_matrix;
}

//returns the number of vertices in the graph
int Graph_SCC::getCount()
{
	return noOfVertices;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

// constructor for class StronglyConnectedComponents
//initializes xml path for writing strongly connected components to an XML file
StronglyConnectedComponents::StronglyConnectedComponents(std::string xmlFilePath):xmlPath(xmlFilePath)
{
	std::cout << "\n";
	std::cout << "-----------------REQUIREMENT 3:Created StronglyConnectedComp package for generating strongly connected components-------------------\n\n";
}

//entry point to SCC creation
void StronglyConnectedComponents::createGraph(DependancyAnalysis &dp, Files filesInLoc)
{
	
	nodeToIntMap = graph.createGraph(dp,filesInLoc);
	noOfVertices = graph.getCount();
	adj_matrix = graph.getAdjacencyMatrix();
	findStrongComp();
	std::string toFile=writeSCCToXml();
	writeToXmLFile(toFile, xmlPath);
}

//Traverses through the adjacency matrix
//Generates strongly connected component information
void StronglyConnectedComponents::findStrongComp()
{
	try {
		int *nodeArr = new int[noOfVertices];
		int *strongCompLow = new int[noOfVertices];
		bool *strongCompFlag = new bool[noOfVertices];
		std::stack<int> *strongCompStack = new std::stack<int>();

		for (int i = 0; i < noOfVertices; i++)
		{
			nodeArr[i] = -1;
			strongCompLow[i] = -1;
			strongCompFlag[i] = false;
		}

		for (int i = 0; i < noOfVertices; i++)
			if (nodeArr[i] == -1)
				strongCompRecursive(i, nodeArr, strongCompLow, strongCompStack, strongCompFlag);
	}
	catch(std::exception e){
		std::cout << e.what() << "\n";
	}
}

//Writing SCC content to XML file
void  StronglyConnectedComponents::writeToXmLFile(std::string xmlData, std::string nameOfFile) {
	std::cout << "\n";
	std::cout << "--------------REQUIREMENT 7:Writing SCC information in path: " << nameOfFile << " ------------\n";
	try {
		std::ofstream ost(nameOfFile);
		if (ost.is_open()) {
			ost << xmlData;
			ost.close();
		}
		else
		{
			std::cout << "Can not access file \n";
		}
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
}

//Returns XML file content in string format
std::string & StronglyConnectedComponents::getXmlFileContent()
{
	return xmlFileContent;
}

//Algorithm to find out strongly connected components
//Recursively traveserses through each node and identifies node to node associations
void StronglyConnectedComponents::strongCompRecursive(int x, int strongCompArr[], int strongCompLow[], std::stack<int>* strongcCompStack, bool strongBool[])
{
	try {
		static int time = 0;

		strongCompArr[x] = strongCompLow[x] = ++time;
		strongcCompStack->push(x);
		strongBool[x] = true;

		std::list<int>::iterator i;
		for (i = adj_matrix[x].begin(); i != adj_matrix[x].end(); ++i)
		{
			int v = *i;
			if (strongCompArr[v] == -1)
			{
				strongCompRecursive(v, strongCompArr, strongCompLow, strongcCompStack, strongBool);
				strongCompLow[x] = min(strongCompLow[x], strongCompLow[v]);
			}

			else if (strongBool[v] == true)
				strongCompLow[x] = min(strongCompLow[x], strongCompArr[v]);
		}

		int w = 0;
		if (strongCompLow[x] == strongCompArr[x])
		{
			while (strongcCompStack->top() != x)
			{
				w = (int)strongcCompStack->top();
				tempVect.push_back(fetchVal(w));
				//std::cout << w << " ";
				strongBool[w] = false;
				strongcCompStack->pop();
			}
			w = (int)strongcCompStack->top();
			tempVect.push_back(fetchVal(w));
			scc_matrix[tempVal] = tempVect;
			tempVal++;
			tempVect.clear();
			//std::cout << w << "\n";
			strongBool[w] = false;
			strongcCompStack->pop();
		}
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
}

//Returns the integer value associated with each file in the map
 std::string StronglyConnectedComponents::fetchVal(int k)
{
	for (auto i : nodeToIntMap)
	{
		if (i.second == k)
			return i.first;
	}
	return  std::string();
}

 //Display strongly connected components
void StronglyConnectedComponents::displayStrongComp()
{
	int i = 1;
	for (auto toD : scc_matrix)
	{
		for (auto xx : toD.second)
		{
			std::cout << "\n";
			std::cout << "Strongly Connected Component :" << i << "\n";
			std::cout << "---------------------------------\n";
			std::cout <<xx << "\n";
			i++;
		}
		std::cout << "\n";
	}
}

//Create XML file format 
//from scc_matrix
//scc_matrix contains strongly connected components
std::string StronglyConnectedComponents::writeSCCToXml()
{
	try {
		int i = 1;
		XmlDocument doc;
		SPtr pRoot = makeTaggedElement("StronglyConnectedComp");
		doc.docElement() = pRoot;

		for (auto sc : scc_matrix)
		{
			SPtr subRoot = makeTaggedElement("SCC");
			SPtr name = makeTaggedElement("name");
			SPtr nameText = makeTextElement("Component" + std::to_string(i));
			i++;
			name->addChild(nameText);
			subRoot->addChild(name);
			for (std::string file : sc.second)
			{
				SPtr fileElem = makeTaggedElement("FileName");
				SPtr pfileTextElem = makeTextElement(file);
				fileElem->addChild(pfileTextElem);
				//adding element to subroot
				subRoot->addChild(fileElem);
			}
			//add subroot tag to root tag
			pRoot->addChild(subRoot);
		}
		xmlFileContent = doc.toString();
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
	return xmlFileContent;
}


#ifdef STRONGLYCONNECTEDCOMP
int main() {

	Executive exec;
	StronglyConnectedComponents scc(exec.sccXmlPath);
	scc.createGraph(dp, allsubfiles);
	std::cout << "---------------------------------------\n";
	std::cout << "Strongly Connected Components\n";
	std::cout << "---------------------------------------\n";
	scc.displayStrongComp();
}

#endif // STRONGLYCONNECTEDCOMP

