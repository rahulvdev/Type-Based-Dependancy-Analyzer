#include "Display.h"

//Display Type Table
void DisplayItems::displayTypeTable(TypeAnalysis & typTable)
{
	std::cout << "\n---------------------------------------------------------------------------------------------\n";
	std::cout << "\n*****************************Type Table******************************\n";
	std::cout << "\n---------------------------------------------------------------------------------------------\n";
	typTable.showTypeTable();
	std::cout << "\n---------------------------------------------------------------------------------------------\n";
	std::cout << "\n\n";
	std::cout << "-----------------------------GlobalFunctionTable------------------------------------------------\n";
	std::cout << std::setw(20) << "TypeName" << std::setw(17) << "Type" << std::setw(25) << "Namespace" << "Filename";
	std::cout << "\n-----------------------------------------------------------------------------------------------\n";
	std::unordered_map<std::string, std::vector<std::string>>& globFuncMap=typTable.getGlobFuncMap();
	for (auto globalFunc : globFuncMap)
	{
		std::cout << std::setw(20) << globalFunc.first << std::setw(20) << "GlobalFunction" << std::setw(20) << "GlobalNamespace";
		for (std::string fname : globalFunc.second)
		{
			std::cout << fname << "  ";
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

//Display Dependancy Table
void DisplayItems::displayDependancyTable(DependancyAnalysis & depTable)
{
	std::cout << "\n---------------------------------------------------------------------------------------------\n";
	std::cout << "\n*****************************Dependency Table******************************\n";
	std::cout << "\n---------------------------------------------------------------------------------------------\n";
	depTable.displayDependancies();
	std::cout << "\n---------------------------------------------------------------------------------------------\n";
}

//Display Strongly Connected Components
void DisplayItems::displayStrongComponents(StronglyConnectedComponents & SCCInst)
{
	std::cout << "-------------REQUIREMENT 6:STrongly Connected Components built from dependancy graph------------------\n";
	std::cout << "\n-------------------------------------------------------------------------------------\n";
	std::cout << "\n*****************************Strongly Connected Components **************************\n";
	std::cout << "\n-------------------------------------------------------------------------------------\n";
	SCCInst.displayStrongComp();
	std::cout << "\n-------------------------------------------------------------------------------------\n";
}

//Constructor for DisplayItems
DisplayItems::DisplayItems()
{
	std::cout << "\n";
	std::cout << "--------------------------------REQUIREMENT 3:Created Display package-------------------------------\n\n";
}


#ifdef DISPLAYITEMS
int main()
{
	DisplayItems disp;
	TypeTable typTab;
	typTab.doTypeAnal();
	DependencyTable depTab("../../xml1");
	depTab.startDependencyAnalysis(fileNames, typTab);
	StronglyConnectedComponents sccComp("../../xml2.xml");
	sccComp.buildSCC(objDepTable);

	disp.displayTypeTable(typTab);
	disp.displaDependancyTable(depTab);
	disp.displayStrongComponents(sccComp);

	return 0;
}
#endif //DISPLAYITEMS

