#ifndef CONSTARRAYBLOCK_H
#define CONSTARRAYBLOCK_H
#include "blockio.h"

class BlockIO;

class ConstArrayBlock : virtual public BlockIO
{
public:
    ConstArrayBlock(int i, int nIn, int nOut);

    void init() override;
    void generateCode(QString dir) override;

    void generateArrayOutputs();
    static int getNumInputs(){return  1;}
    static int getNumOutputs(){return 1;}
    int setSimData(char *data);


    QLineEdit *leCount;
    QLabel *lblCount;
    QLineEdit *leBegin;
    QLabel *lblBegin;
    QLineEdit *leEnd;
    QLabel *lblEnd;
    QLabel *lblType;
    QComboBox *cbType;


};

#endif // CONSTARRAYBLOCK_H
