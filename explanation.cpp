#include "explanation.h"
#include "ui_explanation.h"

Explanation::Explanation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Explanation)
{
    ui->setupUi(this);
}

Explanation::~Explanation()
{
    delete ui;
}
