#ifndef DOCUMMENTPDF_H
#define DOCUMMENTPDF_H

#include "../docummentbase.h"
#include <poppler-qt5.h>
#include <QThread>
class DocummentPDF;
class ThreadLoadDoc : public QThread
{
public:
    ThreadLoadDoc();
    void setDoc(DocummentPDF *doc);

protected:
    virtual void run();

private:
    volatile bool brun;       //bStop是易失性变量，需要用volatile进行申明
    DocummentPDF *m_doc;
};

class ThreadLoadWords : public QThread
{
public:
    ThreadLoadWords();
    void setDoc(DocummentPDF *doc);

protected:
    virtual void run();

private:
    volatile bool brun;       //bStop是易失性变量，需要用volatile进行申明
    DocummentPDF *m_doc;
};

class DocummentPDF: public DocummentBase
{
public:
    DocummentPDF(QWidget *parent = nullptr);
    bool openFile(QString filepath) override;
    QPoint global2RelativePoint(QPoint globalpoint) override;
    bool setSelectTextStyle(QColor paintercolor = QColor(72, 118, 255, 100), QColor pencolor = QColor(72, 118, 255, 0), int penwidth = 0) override;
    bool mouseSelectText(QPoint start, QPoint stop) override;
    void mouseSelectTextClear() override;
    bool mouseBeOverText(QPoint point) override;
    void scaleAndShow(double scale, RotateType_EM rotate) override;
    bool getImage(int pagenum, QImage &image, double width, double height) override;
    int getPageSNum() override;
    bool setViewModeAndShow(ViewMode_EM viewmode) override;
    bool showMagnifier(QPoint point) override;

    bool loadPages();
    bool save(const QString &filePath, bool withChanges) override;
    bool loadWords();
    void removeAllAnnotation();
    void removeAnnotation(const QPoint &startpos) override;
    void addAnnotation(const QPoint &starpos, const QPoint &endpos, QColor color = Qt::yellow) override;
    void search(const QString &strtext, QMap<int, stSearchRes> &resmap, QColor color = Qt::yellow) override;
    void clearSearch();
    int currentPageNo() override;
    bool pageJump(int pagenum) override;
    void docBasicInfo(stFileInfo &info) override;
    Page::Link *mouseBeOverLink(QPoint point) override;

private slots:
    void slot_vScrollBarValueChanged(int value) override;
    void slot_hScrollBarValueChanged(int value) override;

private:
    int pointInWhichPage(QPoint &qpoint);
    void loadWordCache(int indexpage, PageBase *page);
    bool abstractTextPage(const QList<Poppler::TextBox *> &text, PageBase *page);
    void showSinglePage();
    void showFacingPage();
    bool pdfsave(const QString &filePath, bool withChanges)const;
    void searchHightlight(Poppler::Page *page, const QString &strtext, stSearchRes &stres, const QColor &color);
    void refreshOnePage(int ipage);
    void setBasicInfo(const QString& filepath);
    Poppler::Document *document;
    ThreadLoadDoc m_threadloaddoc;
    ThreadLoadWords m_threadloadwords;
    double m_scale;
    RotateType_EM m_rotate;
    QList<QWidget *>m_widgets;
    QWidget *pblankwidget;
    QList<Poppler::Annotation *> m_listsearch;
    int m_currentpageno;
    bool donotneedreloaddoc;
    stFileInfo m_fileinfo;
};

#endif // DOCUMMENTPDF_H
