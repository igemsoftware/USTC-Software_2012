#ifndef POPULATION_H
#define POPULATION_H

#include "consts.h"
#include "cell.h"
#include "scorefunction.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <sbml/SBMLTypes.h>
#pragma comment(lib,"libsbml.lib")
#pragma comment(lib,"libxml2.lib")

namespace ustc{

class Population {

    public:

		Population (const int& _ncell = 100, const int& _evolution = 1000);//_ncell used as a reference to 100 which is the number of total cells
		~Population ();
    
    

		/*
		 * ATT: It is the user's responsibility to ensure the
		 * format of the input dynamics is acceptable by the program.
		 * Thus, we adopt a simple format:
		 * *
		 * numr, numind, numprot
		 * tN(1), tN(2), ..., tN(numc)
		 * tT(1), tT(2), ..., tT(numc)
		 * x(1), y(1), y(2), ..., y(numc)
		 * x(2), y(1), y(2), ..., y(numc)
		 * ...
		 * x(numr), y(1), y(2), ..., y(numc)
		 * *
		 * in which numr is the number of x points and numc (=numind+numprot) is the 
		 * number of y points. tN represents targetName and tT
		 * represents targetType, where 0 is for inducer and 1 is 
		 * for protein. Only dynamics of inducers and proteins are
		 * required as inputs to reverse engineer the underlying 
		 * biochemical networks. 
		 */
		void readDynamics (const string& fn);
        
        void readDynamicsFromConsole();

        //initializer
        void init(std::string);
    
        //get evolution number
        int getEvolution();
    
        //get cells group
        Cell** getCells();
    
        //for growth phase
        void growth ();
    
        //population mutation for topology
        void mutation();
    
        //population mutation for kinetics
        void mut_parameters();
    
        //population mutation for kinetics using simulated annealing algorithm
        void mut_parameters_simAnneal();
    
        //sort all cells after mutate parameters for several generations
        void sort();
    
        //for selection phase
        void selection ();
    
        //classify the surviving cells
        void classification();
    
        //judging whether the evolution is terminated
        bool isTerminate();
    
        //generating certain output format
        void genTikzFormat();
    
        //generating output
        void output();
    
        //generating XML output format
        void genXMLFormat();
       
        //generating HTML output format for display in GUI
        void genHTMLFormat();
    
        //generating SBML output format
        void genSBMLFormat();
        
    private:

        //number of input sets, one species may have multiple sets of inputs
        int numInputSets;
        
		//	dynamics of network nodes
		int numr;               //	number of x points
		double*  xpoints; //dynamic array containing the xpoints

		int numind, numprot;    // number of inducers and proteins
		double*** ypoints;       //	 target dynamic data, 3 demension means it contains several 2-d arrays

		int ncell;              //number of cells in the population
        Cell** cells;           //set of pointers of cells
    
        //classification of cells
        std::vector<std::vector<Cell*> > classifiedCells;
    
        int evolution;
    
        //score function for sorting
        ScoreFunc sfunc;
};

}   //namespace ustc
    
#endif
