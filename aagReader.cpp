#include "aagReader.h"
#include <string.h>

AAGReader::AAGReader(string sourcePath)
{
    source.open(sourcePath.c_str());
    aigname = sourcePath;
    debug.open("aagComentado.txt");
}

Aig* AAGReader::readFile()
{

    cout << "\nReading file: " << aigname << "\n";
    //treating header
    source.getline(buf, 250, '\n'); string s=buf; istringstream line; line.str(s);
    line >> word;

    if(strcmp("aag",word.c_str()) != 0)
    {
        cout << "the file is not an AAG file!";
        return NULL;
    }

    int nNodes, nInputs, nFFs, nOutputs, nAnds;
    line >> word;
    nNodes = atoi(word.c_str());
    line >> word;
    nInputs = atoi(word.c_str());
    line >> word;
    nFFs = atoi(word.c_str());
    line >> word;
    nOutputs = atoi(word.c_str());
    line >> word;
    nAnds = atoi(word.c_str());

    if (nNodes != nInputs + nFFs + nAnds) {
        cout << "Wrong file header";
        return NULL;
    }

    if (nFFs != 0) {
        cout << "FF not supported yet";
        return NULL;
    }

    debug << s << "\nThe file header is ok!\n\n";

    AigNode** nodes = new AigNode*[nNodes + 1];
    OutputNode* outs = new OutputNode[nOutputs];
    InputNode* ins = new InputNode[nInputs];
    AndNode* ands = new AndNode[nAnds];

    int total = 0;

    //treating inputs
    for (int i = 0; i < nInputs; i++, total++) {
        debug << "read the input" << i << " from the file\n";
        debug << "   create in" << i << " and add it to an input list and the all nodes list\n";

        source.getline(buf, 250, '\n'); string s=buf; istringstream line; line.str(s);
        line >> word;

        //cout << "   in" << word << " \n";

        ins[i].setName(word);
        nodes[total] = &ins[i];
    }

    //treating outputs
    debug << "\n";
    for (int i = 0; i < nOutputs; i++, total++) {
        debug << "read the output" << i << " from the file\n";
        debug << "   create out" << i << " and add it to an output list and the all nodes list\n";

        source.getline(buf, 250, '\n'); string s=buf; istringstream line; line.str(s);
        line >> word;

        //cout << "   out" << word << " \n";

        outs[i].setName(word);
        nodes[total] = &outs[i];
    }

    //connecting ands
    debug << "\n";
    for (int i = 0; i < nAnds; i++, total++) {
        debug << "read the and" << i << " output and inputs\n";
        debug << "   connect the and" << i << " and set the inversion of this pins\n";

        source.getline(buf, 250, '\n'); string s=buf; istringstream line; line.str(s);

        // Recover node name
        line >> word;
        ands[i].setName(word);
        nodes[total] = &ands[i];

        // Get node connection #1
        line >> word;
        //cout << ("%d", stoi(word));
        AigNode* node1;
        if (!isInverted(stoi(word))) {node1 = findNode(to_string(stoi(word) -1), nodes, total);}
        else {node1 = findNode(word, nodes, total);}

        ands[i].setFanIn(0, node1, isInverted(stoi(word)));
        node1->connectTo(&ands[i], 0, isInverted(stoi(word)));

        // Get node connection #2
        line >> word;
        //cout << ("%d", stoi(word));
        AigNode* node2;
        if (!isInverted(stoi(word))) {node2 = findNode(to_string(stoi(word) -1), nodes, total);}
        else {node2 = findNode(word, nodes, total);}
        ands[i].setFanIn(1, node2, isInverted(stoi(word)));
        node2->connectTo(&ands[i], 1, isInverted(stoi(word)));

        //debug << ands[i].getName() << " set";

        //exit(1);
    }

    debug << "\n";
    string aigName;
    while(source)
    {
        source.getline(buf, 250, '\n');
        string s=buf;
        istringstream line;
        line.seekg(0);line.str(s);
        line >> word;

        if(strcmp("c",word.substr(0).c_str())==0){
            debug << "the comments began. Ignore the file from here!\n";
            break;
        } else if(strcmp(word.substr(0).c_str(),"i")==0){
            InputNode* input = (InputNode*) findNode(this->word, nodes, nNodes);
            line >> this->word;
            input->setName(this->word);
        } else if(strcmp(word.substr(0).c_str(),"o")==0){
            OutputNode* output = (OutputNode*) findNode(this->word, nodes, nNodes);
            line >> this->word;
            output->setName(this->word);
        } else if(strcmp(word.substr(0).c_str(),"l")==0){
            //??????????????
        }
    }

    debug << "\ncreate the AIG and add all nodes\n";

    Aig *aig_final = new Aig();
    aig_final->setName(aigname);

    for(int i =0; i<nNodes;i++) {
        aig_final->insertNode(nodes[i]);
        switch(nodes[i]->getType()){
            case INPUT_NODE: aig_final->insertInputNode(nodes[i]);
                break;
            case OUTPUT_NODE: aig_final->insertOutputNode(nodes[i]);
                break;
            case AND_NODE: break;
        }
    }

    debug << "return the AIG";
    cout << "Finished reading file \n";

    return aig_final;
}

AigNode* AAGReader::findNode(string name, AigNode** nodes, int aigSize){
    for(int i=0; i< aigSize; i++){
        //cout << ("%d", i) << ": ";
        switch(nodes[i]->getType()){
            case INPUT_NODE:{
                InputNode* input = (InputNode*)nodes[i];
                //cout << ("%s", input->getName()) << "\n";
                if(input->getName() == name){
                    return nodes[i];
                }
                break;
            }
            case OUTPUT_NODE:{
                OutputNode* output = (OutputNode*)nodes[i];
                //cout << ("%s", output->getName()) << "\n";
                if(output->getName() == name){
                    return nodes[i];
                }
                break;
            }
            case AND_NODE:{
                AndNode* andnode = (AndNode*)nodes[i];
                //cout << ("%s", andnode->getName()) << "\n";
                if(andnode->getName() == name){
                    return nodes[i];
                }
                break;
            } default: break;
        }
    }
    cout << "Node " << name << " not found.\n";
    exit(-1);
}

bool AAGReader::isInverted(int value){
    if (value % 2 == 0) {
        return true;
    } else { return false;};
}