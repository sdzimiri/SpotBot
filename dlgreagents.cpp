#include "dlgreagents.h"
#include "ui_dlgreagents.h"

dlgReagents::dlgReagents(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgReagents)
{
    ui->setupUi(this);

    //Initialize string array
    for (int i =0;i<20;i++)
        reagList[i] = "";
}

dlgReagents::~dlgReagents()
{
    delete ui;
}

//If user chooses to save any changes
void dlgReagents::on_bttn_Accept_clicked()
{
    //Pull values from fields and send to main application via signal
    reagList[0] = ui->line_1->text();
    reagList[1] = ui->line_2->text();
    reagList[2] = ui->line_3->text();
    reagList[3] = ui->line_4->text();
    reagList[4] = ui->line_5->text();
    reagList[5] = ui->line_6->text();
    reagList[6] = ui->line_7->text();
    reagList[7] = ui->line_8->text();
    reagList[8] = ui->line_9->text();
    reagList[9] = ui->line_10->text();
    reagList[10] = ui->line_11->text();
    reagList[11] = ui->line_12->text();
    reagList[12] = ui->line_13->text();
    reagList[13] = ui->line_14->text();
    reagList[14] = ui->line_15->text();
    reagList[15] = ui->line_16->text();
    reagList[16] = ui->line_17->text();
    reagList[17] = ui->line_18->text();
    reagList[18] = ui->line_19->text();
    reagList[19] = ui->line_20->text();

    QStringList list;

    for (int i = 0; i<20;i++)
        list.append(reagList[i]);

    emit txUpdatedReagents(list);


    this->close();
}
//Cancel changes
void dlgReagents::on_bttn_Cancel_clicked()
{
    this->close();
}

//Set reagents
void dlgReagents::setReagents(QString list[20])
{
    //Copy list to local list
    for (int i =0;i<20;i++)
        reagList[i] = list[i];

    //Update GUI
    ui->line_1->setText(reagList[0]);
    ui->line_2->setText(reagList[1]);
    ui->line_3->setText(reagList[2]);
    ui->line_4->setText(reagList[3]);
    ui->line_5->setText(reagList[4]);
    ui->line_6->setText(reagList[5]);
    ui->line_7->setText(reagList[6]);
    ui->line_8->setText(reagList[7]);
    ui->line_9->setText(reagList[8]);
    ui->line_10->setText(reagList[9]);
    ui->line_11->setText(reagList[10]);
    ui->line_12->setText(reagList[11]);
    ui->line_13->setText(reagList[12]);
    ui->line_14->setText(reagList[13]);
    ui->line_15->setText(reagList[14]);
    ui->line_16->setText(reagList[15]);
    ui->line_17->setText(reagList[16]);
    ui->line_18->setText(reagList[17]);
    ui->line_19->setText(reagList[18]);
    ui->line_20->setText(reagList[19]);

}
