#ifndef DEFAULTOPERATIONMENU_H
#define DEFAULTOPERATIONMENU_H

#include "CustomControl/CustomMenu.h"

/**
 * @brief The DefaultOperationWidget class
 * @brief   右键  默认菜单操作， 搜索、添加书签、第一页、上一页、下一页、最后一页
 */

class DefaultOperationMenu : public CustomMenu
{
    Q_OBJECT
    Q_DISABLE_COPY(DefaultOperationMenu)

public:
    explicit DefaultOperationMenu(DWidget *parent = nullptr);

public:
    void execMenu(const QPoint &, const int &);
    inline void setClickpoint(const QPoint &pt)
    {
        m_pointclicked = pt;
    }

    void setDoubleShow(const bool &isDoubleShow);

    // CustomMenu interface
protected:
    void initActions() override;

private:
    QAction *createAction(const QString &, const char *member);

private slots:
    void slotSearchClicked();
    void slotBookMarkClicked();
    void slotFirstPageClicked();
    void slotPrevPageClicked();
    void slotNextPageClicked();
    void slotEndPageClicked();
    void slotExitFullScreenClicked();
    void slotAddIconNote();

private:
    int     m_nRightPageNumber = -1;
    QAction *m_pSearch = nullptr;
    QAction *m_pBookMark = nullptr;
    QAction *m_pAddIconNote = nullptr;
    QAction *m_pFirstPage = nullptr;
    QAction *m_pPrevPage = nullptr;
    QAction *m_pNextPage = nullptr;
    QAction *m_pEndPage = nullptr;
    QAction *m_pExitFullScreen = nullptr;
    QPoint m_pointclicked;
    QPoint m_showPoint;
    bool    m_isDoubleShow = false;
};

#endif // DEFAULTOPERATIONMENU_H
