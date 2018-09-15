#include <cstdlib>
#include <list>
#include "aagReader.h"

using namespace std;

int main(int argc, char* argv[])
{  
    /*
    Aig* aig = new Aig();
    aig->setName("C17");

    //creating inputs
    InputNode* in1 = new InputNode(); in1->setName("pi1");
    InputNode* in2 = new InputNode(); in2->setName("pi2");
    InputNode* in3 = new InputNode(); in2->setName("pi3");
    InputNode* in4 = new InputNode(); in2->setName("pi4");
    InputNode* in5 = new InputNode(); in2->setName("pi5");

    //creating ands
    AndNode* and1 = new AndNode();
    AndNode* and2 = new AndNode();
    AndNode* and3 = new AndNode();
    AndNode* and4 = new AndNode();
    AndNode* and5 = new AndNode();
    AndNode* and6 = new AndNode();
   
    //creating outputs
    OutputNode* out1 = new OutputNode(); out1->setName("po1");
    OutputNode* out2 = new OutputNode(); out2->setName("po2");
    and4->connectTo(out1,0,1);
    and6->connectTo(out2,0,1);
   
    //connecting ands
    in1->connectTo(and1,0,0);
    in3->connectTo(and1,1,0);
    
    in3->connectTo(and2,0,0);
    in4->connectTo(and2,1,0);
    
    in2->connectTo(and3,0,0);
    and2->connectTo(and3,1,1);
    
    and1->connectTo(and4,0,1);
    and3->connectTo(and4,1,1);
    
    in5->connectTo(and5,0,0);
    and2->connectTo(and5,1,1);
    
    and3->connectTo(and6,0,1);
    and5->connectTo(and6,1,1);
    
    //populating aig
    aig->insertInputNode(in1); aig->insertNode(in1);
    aig->insertInputNode(in2); aig->insertNode(in2);
    aig->insertInputNode(in3); aig->insertNode(in3);
    aig->insertInputNode(in4); aig->insertNode(in4);
    aig->insertInputNode(in5); aig->insertNode(in5);
    
    aig->insertNode(and1);
    aig->insertNode(and2);
    aig->insertNode(and3);
    aig->insertNode(and4);
    aig->insertNode(and5);
    aig->insertNode(and6);

    aig->insertOutputNode(out1); aig->insertNode(out1);
    aig->insertOutputNode(out2); aig->insertNode(out2);
    */

    string filename;
    if (argv[1] != NULL){filename = argv[1];} else {filename = "C17.aag";}

    AAGReader* reader = new AAGReader(filename);
    
    Aig* aig= reader->readFile();

    //debuging
    cout << "# name of AIG: " << aig->getName() << "\n";
    cout << "  * number of Nodes:   " << aig->getNodes().size() + aig->getInputs().size() << "\n";
    cout << "  * number of Inputs:  " << aig->getInputs().size() << "\n";
    cout << "  * number of Outputs: " << aig->getOutputs().size() << "\n";
    cout << "  * number of Ands: " << aig->getNodes().size() << "\n";

    cout << "\n ins:  ";
    for(AigNode* node : aig->getInputs()){
        cout << ((InputNode*) node)->getName() << " ";
    }
    cout << "\n outs: ";
    for(AigNode* node : aig->getOutputs()){
        cout << ((OutputNode*) node)->getName() << " ";
    }
    cout << "\n ands: ";
    for(AigNode* node : aig->getNodes()){
        if (node->getType() == AND_NODE) {
            cout << ((AndNode*) node)->getName() << " ";
        }
    }
    //End of debugging
    
    return EXIT_SUCCESS;
}
