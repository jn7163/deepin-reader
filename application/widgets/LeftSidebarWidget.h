#ifndef LEFTSIDEBARWIDGET_H
#define LEFTSIDEBARWIDGET_H

#include "CustomControl/CustomWidget.h"

/**
 * @brief The LeftShowWidget class
 * @brief   侧边栏显示
 */

class LeftSidebarWidget : public CustomWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(LeftSidebarWidget)

public:
    explicit LeftSidebarWidget(CustomWidget *parent = nullptr);
    ~LeftSidebarWidget() Q_DECL_OVERRIDE;

signals:
//    void sigDealWithData(const int &, const QString &);
//    void sigDealWithKeyMsg(const QString &);

    void sigSearchWidgetState(const int &);

private slots:
    void slotDealWithData(const int &, const QString &);
    void slotDealWithKeyMsg(const QString &);

    void slotUpdateTheme();
    void slotSetStackCurIndex(const int &);

private:
    void initConnections();

    void __DeleteItemByKey();
    void onSetWidgetVisible(const int &);

    void onJumpToPrevPage();
    void onJumpToNextPage();

    void doPrevPage(const int &index);
    void doNextPage(const int &index);

    void forScreenPageing(bool);

    // CustomWidget interface
protected:
    void initWidget() Q_DECL_OVERRIDE;

    // IObserver interface
public:
    int dealWithData(const int &, const QString &) Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
};

#endif  // LEFTSHOWWIDGET_H