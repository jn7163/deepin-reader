﻿#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <DMenu>
#include <DImageButton>
#include <QHBoxLayout>
#include "subjectObserver/CustomImageButton.h"
#include "subjectObserver/CustomWidget.h"

#include "pdfControl/font/fontWidget.h"

#include "PublicFunction.h"
#include "subjectObserver/CustomImageButton.h"
/**
 * @brief The TitleWidget class
 * @brief   标题栏的 按钮操作
 */

class TitleWidget : public CustomWidget
{
    Q_OBJECT
public:
    TitleWidget(CustomWidget *parent = nullptr);
    ~TitleWidget() override;

private slots:

    void slotFontWidgetHide();
    void slotHandleMenuHide();

    void on_thumbnailBtn_clicked();
    void on_settingBtn_clicked();
    void on_handleShapeBtn_clicked();
    void on_magnifyingBtn_clicked();

    void SlotActionTrigger(const QString &);

private:
    void initBtns();
    CustomImageButton *createBtn(const QString &btnName, bool bCheckable = false);
    QAction *createAction(const QString &iconName);
    void sendMsgToSubject(const int &, const QString &msgCotent = "");

    void openFileOk();
    void setHandleShapeBtn(const QString &);

    void on_HandleAction_trigger();
    void on_DefaultAction_trigger();

private:
    QHBoxLayout *m_layout = nullptr;

    FontWidget *m_pFontWidget = nullptr;
    int     m_nCurrentState = 0;

    DMenu   *m_pHandleMenu = nullptr;
    QAction *m_pHandleAction = nullptr;
    QAction *m_pDefaultAction = nullptr;

    CustomImageButton *m_pThumbnailBtn = nullptr;
    CustomImageButton *m_pSettingBtn = nullptr;
    CustomImageButton *m_pHandleShapeBtn = nullptr;
    CustomImageButton *m_pMagnifierBtn = nullptr;

    // CustomWidget interface
protected:
    void initWidget() override;

    // IObserver interface
public:
    int dealWithData(const int &, const QString &) override;
};

#endif // TITLEWIDGET_H
