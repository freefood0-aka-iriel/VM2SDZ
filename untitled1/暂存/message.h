#ifndef MESSAGE_H
#define MESSAGE_H
#include <QMainWindow>
#include <QMessageBox>

class Message
{
public:
    Message();
public slots:
    void help();
    void updateDialog();
    void settingHelp();
};

#endif // MESSAGE_H
