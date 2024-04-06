#ifndef POMOPEAKSETTINGS_H
#define POMOPEAKSETTINGS_H

#include <QWidget>
#include "settings.h"
#include "sqliteHandler.h"
#include <QMediaPlayer>
#include "QObjectInitialization.h"
#include "GPTHelper.h"
#include <QLineEdit>
namespace Ui {
class pomopeaksettings;
}

class pomopeaksettings : public QWidget, public QObjectInitialization
{
    Q_OBJECT

public:
    explicit pomopeaksettings(Settings& _settings, SqliteHandler& _handler, GPTHelper& _gptHelper, QWidget *parent = nullptr);
    ~pomopeaksettings();
    void setVisible(bool visible) override
    {
        QWidget::setVisible(visible);
        IsOpened  = visible;
    }
    bool IsOpened = false;
protected:
    void InitializeDataContainer() override;
    void InitializeObjects() override;
    void SubscribeToEvents() override;
signals:
    void OnClose(const bool alarmStartChanged,const bool alarmBreakChanged);
private slots:
    void OnSliderValueChanged(int value);
    void OnDoubleSpinBoxValueChanged(double value);
    void OnSpinBoxValueChanged(int value);
    void OnExitClicked();
    void OnSelectAudioClicked();
    void OnCheckBoxValueChanged(bool value);
    void OnIndexInComboBoxChanged(int index);
    void OnQuickActionSequenceFinished();
    void OnChangeChatGPTVisibility();
    void OnChatGPTApiChanged(const QString& val);
private:
    Ui::pomopeaksettings *ui;
    Settings& settings;
    SqliteHandler& handler;
    GPTHelper& gptHelper;
    bool isDirty = false;
    bool startAlarmChanged = false;
    bool breakAlarmChanged = false;

    void SetChatGPTLineEditEchoMode(const QLineEdit::EchoMode& mode);
};

#endif // POMOPEAKSETTINGS_H
