#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this); 
    QObject::connect(ui->pushButton_back, SIGNAL(clicked()), this, SLOT(back()));
}

Info::~Info()
{
    delete ui;
}

void Info::back(){
    this->hide();
}
