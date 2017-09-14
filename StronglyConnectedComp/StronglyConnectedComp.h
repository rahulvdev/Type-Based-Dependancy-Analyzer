#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//// StrongComp.h - computes Strongly Connected Components for files based on Dependency Table Infoinfo //
////																									 //
//// Rahul Vijaydev,CSE687 - Object Oriented Design, Spring 2017										// 
////																								   //
///////////////////////////////////////////////////////////////////////////////////////////////////////
///*
//Module Operations:
//==================
//This module defines an Graph_SCC Class which and  StronglyConnectedComponents class
//
//Graph class defines functions to
//add an edge
//to intilize the number of vertices of the graph
//and to return the adjacency matric of the graph
//
//StronglyCC class defines functions to:
//to build strongly connected components
//to persist the results of SCC to an xml file
//display the contents of the scc
//
//Public Interface:
//=================
//Graph_SCC();
//std::unordered_map<std::string, int> createGraph(DependancyAnalysis dptemp,Files filesInLoc);
//int createMap();
//void createAdjacencyMatrix();
//std::list<int>* getAdjacencyMatrix();
//int getCount();
//
//StronglyConnectedComponents();
//void createGraph(DependancyAnalysis dp,Files filesInLoc);
//void strongCompRecursive(int x, int strongCompArr[], int strongCompLow[], std::stack<int> *strongcCompStack, bool strongBool[]);
//void findStrongComp();
//std::string fetchVal(int k);
//void displayStrongComp();
//
//* Required Files:
//* ---------------
//*    -DependencyAnalysis.h
//*	   -DBAccess.h
//*    -XmlDocument.h
//*    -XmlElement.h
//*
//*
//* Build Process:
//* --------------
//*   devenv StronglyConnectedComp.sln /debug rebuild
//
// Maintenance History:
// --------------------
//ver 0.1 : 8th March 2017
//
//
//
//

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include "../DBAccess/DBAccess.h"
#include "../DependencyAnalyzer/DependencyAnalysis.h"



class Graph_SCC
{
public:
	using Key = std::string;
	using Children = std::vector<Key>;
	using Item = std::pair<Key, Element<std::string>>;
	using File = std::string;
	using Files = std::vector<File>;

	Graph_SCC();
	std::unordered_map<std::string, int> createGraph(DependancyAnalysis &dp,Files filesInLoc);
	int createMap();
	void createAdjacencyMatrix(DependancyAnalysis &dp);
	std::list<int>* getAdjacencyMatrix();
	int getCount();
	

private:
	int noOfVertices;
	std::vector<File> fileVect;
	std::unordered_map<File, int> mapOfFiles;
	std::list<int> *adjacency_matrix;
	
	

};


class StronglyConnectedComponents
{
public:
	using File = std::string;
	using Files = std::vector<File>;
	using SPtr = std::shared_ptr<AbstractXmlElement>;


	StronglyConnectedComponents(std::string xmlFilePath);
	void createGraph(DependancyAnalysis &dp,Files filesInLoc);
	void strongCompRecursive(int x, int strongCompArr[], int strongCompLow[], std::stack<int> *strongcCompStack, bool strongBool[]);
	void findStrongComp();
	std::string fetchVal(int k);
	void displayStrongComp();
	void  StronglyConnectedComponents::writeToXmLFile(std::string xmlData, std::string nameOfFile);
	std::string& getXmlFileContent();


private:
	Graph_SCC graph;
	int noOfVertices;
	std::list<int>* adj_matrix;
	std::unordered_map<int, std::vector<std::string>> scc_matrix;
	std::unordered_map<File, int> nodeToIntMap;
	std::vector<std::string> tempVect;
	std::string writeSCCToXml();
	int tempVal = 0;
	std::string xmlPath;
	std::string xmlFileContent;

};
