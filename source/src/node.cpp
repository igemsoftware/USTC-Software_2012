#include "node.h"
#include "reaction.h"

namespace ustc{


void quickSort(vector<Node*> *_components){
    
    vector<Node*> less;                 //Node** less = new Node*[num];
    vector<Node*> greater;              //Node** greater = new Node*[num];
    Node* temp;
    int num = (int)(*_components).size();
    
    //end of recursion, no need to sort
    if(num <= 1) return;
    
    vector<Node*>::iterator iter = (*_components).begin() + 1;
    while (iter != (*_components).end()) {
        if ((*iter)->getNindex() > (*(*_components).begin())->getNindex()) {
            greater.push_back(*iter);
        }
        else {
            less.push_back(*iter);
        }
        iter++;
    }
    
    
    //recursion
    quickSort(&less);
    quickSort(&greater);

    
    //final assignments
    temp = (*_components)[0];
    (*_components).clear();
    //first, for less members
    std::vector<Node*>::iterator iter1 = less.begin();
    while (iter1 != less.end()) {
        (*_components).push_back((*iter1));
        iter1++;
    }
    (*_components).push_back(temp);
    //second, for greater members
    iter1 = greater.begin();
    while (iter1 != greater.end()) {
        (*_components).push_back(*iter1);
        iter1++;
    }
}



void sort(std::vector<Node*> *_components, int num_of_members[3]){
    
    //count each components and put the numbers in members
    for (int i = 0; i < 3; i++) {
        num_of_members[i] = 0;
    }
    vector<Node*>::iterator iter = (*_components).begin();
    if (*iter != NULL) {
        num_of_members[2-1] = 1;                //have one gene
        iter++;
    }
    vector<Node*>::iterator iter_end = (*_components).end();
    
    while (iter != iter_end)  {
        if((*iter) != NULL){
            num_of_members[(*iter)->getNtype()-1]++;
        }
        iter++;
    }
    
    //sort the vector by components' indice

    //delete the first one
    //either gene or NULL, should not be sorted
    Node* temp = *(*_components).begin();
    (*_components).erase((*_components).begin());
    //sort the rest
    quickSort(_components);
    //put the first one back
    (*_components).insert((*_components).begin(), temp);

}


//basic node constructor
Node::Node(int _nindex, node_type _ntype):nindex(_nindex),ntype(_ntype){
    
    std::stringstream ss;
    ss << nindex;

    switch (ntype) {
    case INDUCER:
        nstring = "indu" + ss.str();
        components.push_back(NULL);
        components.push_back(this);
        break;
    case GENE:
        nstring = "g" + ss.str();
        components.push_back(this);
        break;
    case PROTEIN:
        nstring = "P" + ss.str();
        components.push_back(NULL);
        components.push_back(this);
        break;
    default:
        break;
    }
    

}


//complex node constructor
Node::Node(int _nindex, Node* _nleft, Node* _nright)
:nindex(_nindex)
{
    //	merge N_left and N_right
    
    //	gene
    if(_nleft->getNode(0) == NULL) {
        if(_nright->getNode(0) == NULL) {
            components.push_back(NULL);
        }
        else {
            components.push_back(_nright->getNode(0)) ;
        }
    }
    else {
        if(_nright->getNode(0) == NULL) {
            components.push_back(_nleft->getNode(0));
        }
        else{
            std::cerr << "Error: more than one genes appear in a complex!" << std::endl;
            std::exit (1);
        }
    }
    //the first component is set whether its a gene or NULL
    
    //	proteins
    int lsize = _nleft->getNsize ();
    int rsize = _nright->getNsize ();
    
    for(int ileft = 1; ileft < lsize; ileft++) {
        components.push_back(_nleft->getNode(ileft));
    }
    for(int iright = 1; iright < rsize; iright++) {
        components.push_back(_nright->getNode(iright));
    }
    
    //	sort
    //components.sort ();
    int members[3]={0};
    sort(&components,members);
    

    //assign ntype
    if (members[1]) {               //gene
        if (members[2]) {           //protein
            ntype = GENE_PROT_COMPLEX;              //gene/protein complex
        }else {
            ntype = GENE;              //only gene
        }
    }
    else if (members[2]) {          //protein
            if (members[0]) {       //inducer
                ntype = INDUCER_PROT_COMPLEX;          //inducer/protein complex
            }else if(members[2]>=2) {
                    ntype = PROTEIN_COMPLEX;      //pure protein complex
                }else {
                    ntype = PROTEIN;      //only protein
                }
         }else {
            ntype = INDUCER;              //only inducer
    }
    
    
    //	assign representations
    nstring = write();		
}


//complex node constructon of certain type
Node::Node(int _nindex, node_type _ntype, Node* _nleft, Node* _nright):nindex(_nindex){
    
    //	merge N_left and N_right
    
    //	gene
    if(_nleft->getNode(0) == NULL) {
        if(_nright->getNode(0) == NULL) {
            components.push_back(NULL);
        }
        else {
            components.push_back(_nright->getNode(0)) ;
        }
    }
    else {
        if(_nright->getNode(0) == NULL) {
            components.push_back(_nleft->getNode(0));
        }
        else{
            std::cerr << "Error: more than one genes appear in a complex!" << std::endl;
            std::exit (1);
        }
    }
    //the first component is set whether its a gene or NULL
    
    //	proteins
    int lsize = _nleft->getNsize ();
    int rsize = _nright->getNsize ();
    
    for(int ileft = 1; ileft < lsize; ileft++) {
        components.push_back(_nleft->getNode(ileft));
    }
    for(int iright = 1; iright < rsize; iright++) {
        components.push_back(_nright->getNode(iright));
    }
    
    //	sort
    //components.sort ();
    int members[3]={0};
    sort(&components,members);
    
    ntype = _ntype;
    
    //	assign representations
    nstring = write();	
}



//copy constructor
Node::Node(Node &aNode){
    nstring = aNode.nstring;
    nindex = aNode.nindex;
    ntype = aNode.ntype;
    
    /* do not copy components, leave it for cell to finish,
     * because it needs the relationships with other node,
     * with cannot judge right now.
     */
    
}

Node::~Node()
{}


int Node::getNindex () {
    return nindex;
}

node_type Node::getNtype () {
    return ntype;
}
int Node::getNsize () {
    return (int)components.size ();  //including situation when components[0] is NULL
}

Node* Node::getNode (const int& index) {
    if(index >= 0 && index < components.size()) {
        return components[index];
    }
    else return NULL;
}

string Node::getNstring () {
    return nstring;
}

bool Node::operator==(const Node& n1) const {
    return nstring == n1.nstring;
}

string Node::write () {

    string nodestr;
    if(components[0] != NULL) {                     //gene
        nodestr += components[0]->getNstring() + "_";
    }

    std::vector<Node*>::iterator iter = components.begin ();
    std::vector<Node*>::iterator iter_end = components.end();
    //components[0] is for gene
    while (++iter != iter_end) {                    //proteins
        nodestr += (*iter)->getNstring() + "_";
    }

    return nodestr.substr(0, nodestr.length()-1);   //delete last ":"
}


Node* Node::extractFirstGene(){
    return components[0];
}


//for cell to access components vector
void Node::pushNode(Node* aNode){
    components.push_back(aNode);
}




#include "reaction.h"
/*
 ode:
 input: reactionList is the rlist variable in the cell; y contains the current value of each node, x is current time
 basic concepts: ode has the form of dN/dx = ode(Y,x), y is a vector contains current nodes' values, and node index is the subscript i in y[i]
 */
double Node::ode(std::vector<Reaction*> reactionList, double *y, double t){
    double result = 0;
    std::vector<Reaction*>::iterator iter = reactionList.begin();
    std::vector<Reaction*>::iterator iter_end = reactionList.end();
    
    /* iterate over all the reactions in the reactionList, and every reaction that contains this node will have influence on result */
    while (iter != iter_end) {
        bool containThisNode;
        containThisNode = (*iter)->containNode(this);
        
        
        /* current reaction contains this node in reactants, modifiers or products */
        if (containThisNode) {
            int reactionType = (*iter) -> getRtype();
            switch (reactionType) {
                case 0:                 //Transcription of a gene from a free promoter or a bound promoter/protein complex
                    if ((*iter)->containNodeAsProduct(this)) {
                        //this node is in prodcuts of type 0 reaction , that is , this node is a protein, and is to be transcripted
                        int DNAIndex;
                        double forwardRate = (*iter)->forwardRate;
                        DNAIndex = (*iter)->modifiers[0]->nindex;//for protein transcription, a DNA is regarded as a modifier rather than a reactant
                        result += forwardRate * y[DNAIndex];
                    }
                    break;
                    
                case 1:                 //degradation of protein
                    if ((*iter)->containNodeAsReactant(this)) {
                        //this node is in reactants of type 1 reaction , that is , this node is a protein, and is to degrade
                        int proteinIndex;
                        double forwardRate = (*iter)->forwardRate;
                        proteinIndex = this->nindex;
                        result -= forwardRate * y[proteinIndex];
                    }
                    break;
                    
                    
                default:                //all the rest reaction types could have the same routine
                    
                    /* if this node is in reactants */
                    if (int reactantCounts = (*iter)->containNodeAsReactant(this)) {
                        //this node is a reactant
                        double reactantsMultiply = 1., productsMultiply = 1.;
                        
                        /* calculate reactantsMultiply */
                        std::vector<Node*>::iterator reactantsIter = (*iter)->reactants.begin();
                        std::vector<Node*>::iterator reactantsIter_end = (*iter)->reactants.end();
                        while (reactantsIter != reactantsIter_end) {
                            int index = (*reactantsIter)->nindex;
                            reactantsMultiply *= y[index];
                            reactantsIter++;
                        }
                        
                        /* calculate productsMultiply */
                        std::vector<Node*>::iterator productsIter = (*iter)->products.begin();
                        std::vector<Node*>::iterator productsIter_end = (*iter)->products.end();
                        while (productsIter != productsIter_end) {
                            int index = (*productsIter)->nindex;
                            productsMultiply *= y[index];
                            productsIter++;
                        }
                        
                        /* adding modifiers to both reactantMultiply and productsMultiply */
                        std::vector<Node*>::iterator modifierIter = (*iter)->modifiers.begin();
                        std::vector<Node*>::iterator modifierIter_end = (*iter)->modifiers.begin();
                        while (modifierIter != modifierIter_end) {
                            int index = (*modifierIter)->nindex;
                            reactantsMultiply *= y[index];
                            productsMultiply *= y[index];
                            modifierIter++;
                        }
                        
                        double forwardRate = (*iter)->forwardRate;
                        double reverseRate = (*iter)->reverseRate;
                        
                        result = result -  forwardRate * reactantsMultiply * reactantCounts + reverseRate * productsMultiply * reactantCounts;
                    }
                    
                    
                    /* if this node is in products, 
                     *can't include the following block in else of the previous if statement, 
                     *for this node may be in both reactants and products 
                     */
                    if (int productCounts = (*iter)->containNodeAsProduct(this)) {
                            //this node is a reactant
                        double reactantsMultiply = 1., productsMultiply = 1.;
                        
                        /* calculate reactantsMultiply */
                        std::vector<Node*>::iterator reactantsIter = (*iter)->reactants.begin();
                        std::vector<Node*>::iterator reactantsIter_end = (*iter)->reactants.end();
                        while (reactantsIter != reactantsIter_end) {
                            int index = (*reactantsIter)->nindex;
                            reactantsMultiply *= y[index];
                            reactantsIter++;
                        }
                        
                        /* calculate productsMultiply */
                        std::vector<Node*>::iterator productsIter = (*iter)->products.begin();
                        std::vector<Node*>::iterator productsIter_end = (*iter)->products.end();
                        while (productsIter != productsIter_end) {
                            int index = (*productsIter)->nindex;
                            productsMultiply *= y[index];
                            productsIter++;
                        }
                        
                        double forwardRate = (*iter)->forwardRate;
                        double reverseRate = (*iter)->reverseRate;
                        
                        result = result +  forwardRate * reactantsMultiply * productCounts - reverseRate * productsMultiply * productCounts;
                    }
                break;
            }
        }
        iter++;   
    }
    
    return result;
}

}   //namespace ustc

