#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->progressBar->setValue(0);
    /* update the progress bar along with encoding  */
    connect(processParameter, &ProcessParameter::update_Process_Number, this, &Widget::update_Process_Bar);

    connect(ui->toolButton, &QToolButton::clicked,[&](){
        QString filename=QFileDialog::getOpenFileName();
        ui->lineEdit_inputFile->setText(filename);
    });

    connect(ui->pushButton_apply, SIGNAL(clicked(bool)), this, SLOT(apply_Pushed()));
    
    connect(ui->pushButton_convert, SIGNAL(clicked(bool)), this, SLOT(convert_Pushed()));
    
    connect(ui->pushButton_encodeSetting, SIGNAL(clicked(bool)), this, SLOT(encode_Setting_Pushed()));

    //Init QuickInfo
    quickInfo->videoIdx = 0;
    quickInfo->width = 0;
    quickInfo->height = 0;
    quickInfo->colorSpace = "";
    quickInfo->videoCodec = "";
    quickInfo->videoBitRate = 0;
    quickInfo->frameRate = 0;

    quickInfo->audioIdx = 0;
    quickInfo->audioCodec = "";
    quickInfo->audioBitRate = 0;
    quickInfo->channels = 0;
    quickInfo->sampleFmt = "";
    quickInfo->sampleRate = 0;

    quickInfo->subIdx = 0;

}

void Widget::update_Process_Bar()
{
    static int x = 100;
    ui->progressBar->setValue(x);
    ui->label_process->setText(QString("Process: %1%").arg(x));
}

void Widget::encode_Setting_Pushed()
{
    encodeSetting->show();
}


void Widget::apply_Pushed()
{

    QByteArray ba = ui->lineEdit_inputFile->text().toLocal8Bit();
    char *src = ba.data();
    //get info by Decapsulation
    info->send_info(src, quickInfo);

    //display info on window
    info_Display(quickInfo);

}

void Widget::convert_Pushed()
{
    if(encodeSetting->get_Available())
    {
        encodeSetting->get_Encode_Parameter(converter->encodeParameter);
    }

    QMessageBox displayResult;
    if(ui->lineEdit_inputFile->text() == ui->lineEdit_outputFile->text())
    {
        displayResult.setText("The input file can't same as ouput file!");
        displayResult.exec();
        return;
    }



    if(converter->convert_Format(ui->lineEdit_inputFile->text(), ui->lineEdit_outputFile->text()))
    {
        displayResult.setText("Convert success!");
    }else
    {
        displayResult.setText("Convert failed! Please ensure the file path is correct");
    }
    displayResult.exec();

}


void Widget::info_Display(QuickInfo *quickInfo)
{
    //video
    ui->label_videoStream->setText(QString("Video : Stream %1").arg(quickInfo->videoIdx));
    ui->label_width->setText(QString("width: %1").arg(quickInfo->width));
    ui->label_height->setText(QString("height: %1").arg(quickInfo->height));
    ui->label_colorSpace->setText(QString("color_space: %1").arg(quickInfo->colorSpace));
    ui->label_videoCodec->setText(QString("codec: %1").arg(quickInfo->videoCodec));
    ui->label_videoBitRate->setText(QString("bit_rate: %1").arg(quickInfo->videoBitRate));
    ui->label_frameRate->setText(QString("frame_rate: %1").arg(quickInfo->frameRate));
    //audio
    ui->label_audioStream->setText(QString("Audio : Stream %1").arg(quickInfo->audioIdx));
    ui->label_audioCodec->setText(QString("codec: %1").arg(quickInfo->audioCodec));
    ui->label_audioBitRate->setText(QString("bit_rate: %1").arg(quickInfo->audioBitRate));
    ui->label_channels->setText(QString("channels: %1").arg(quickInfo->channels));
    ui->label_sampleFmt->setText(QString("sample_fmt: %1").arg(quickInfo->sampleFmt));
    ui->label_sampleRate->setText(QString("sample_rate: %1").arg(quickInfo->sampleRate));
}


Widget::~Widget()
{
    delete ui;
}
