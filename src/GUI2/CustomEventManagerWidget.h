//
// Created by chenkuan on 2018/2/20.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMEVENTTABWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMEVENTTABWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <Logger.h>

/**
 * 事件管理页面，显示事件、添加删除事件等
 */
class CustomEventManagerWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomEventManagerWidget(QWidget *parent = nullptr);

  signals:

    void insertEvent(int index, const QString &eventName, const QString &eventContent);
    void removeEvent(int index);

    void modifyEvent(int index, const QString &eventName, const QString &eventContent);

  protected slots:
    void addEvent();
    void deleteEvent();

    void editListItem(QListWidgetItem *item);

    void afterEditEventName(QListWidgetItem *item);

    void afterEditEventContent();

  private:
    /**
     * 整体采用左右布局
     */
    QHBoxLayout *hBoxLayout = nullptr;

    /**
     * 左侧使用上下布局
     */
    QVBoxLayout *leftVBoxLayout = nullptr;
    QListWidget *eventListWidget = nullptr;
    QPushButton *addButton = nullptr;
    QPushButton *deleteButton = nullptr;

    /**
     * 右侧堆叠CustomEventWidget
     */
    QStackedWidget *eventStackedWidget = nullptr;

    Logger *logger = Logger::getLogger();

};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMEVENTTABWIDGET_H
