#include "constantblock.h"

ConstantBlock::ConstantBlock(int i):
    BlockIO(i, 0, 1, BlockItem::BlockType::Constant)
{
    init();
}

void ConstantBlock::init()
{
    leConstantVal = new QLineEdit;

    lblConstVal = new QLabel("Val");

    boxLayout->addWidget(lblConstVal);
    boxLayout->addWidget(leConstantVal);
    boxLayout->rowStretch(1);

    groupBox->setLayout(boxLayout);
    groupBox->setTitle("Constant Block");
}

void ConstantBlock::generateCode(QString dir)
{

}
